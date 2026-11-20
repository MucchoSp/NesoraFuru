#include "NesoraFuru.h"

#include <HTS_engine.h>
#include <mecab.h>
#include <njd.h>
#include <jpcommon.h>
#include <text2mecab.h>
#include <mecab2njd.h>
#include <njd_set_pronunciation.h>
#include <njd_set_digit.h>
#include <njd_set_accent_phrase.h>
#include <njd_set_accent_type.h>
#include <njd_set_unvoiced_vowel.h>
#include <njd_set_long_vowel.h>
#include <njd2jpcommon.h>

namespace NesoraFuru {

	struct NesoraAnalyzer::State {
        Mecab mecab;
        NJD njd;
        JPCommon jp;
        HTS_Engine engine;

        State() {
            Mecab_initialize(&mecab);
            NJD_initialize(&njd);
            JPCommon_initialize(&jp);
            HTS_Engine_initialize(&engine);
        }
        ~State() {
            HTS_Engine_refresh(&engine);  // 1.10 の clear は発話バッファを解放しない
            HTS_Engine_clear(&engine);
            JPCommon_clear(&jp);
            NJD_clear(&njd);
            Mecab_clear(&mecab);
        }
        void reset_utterance() {
            HTS_Engine_refresh(&engine);
            JPCommon_refresh(&jp);
            NJD_refresh(&njd);
            Mecab_refresh(&mecab);
        }
	};

	NesoraAnalyzer::~NesoraAnalyzer() = default;

	NesoraAnalyzer::NesoraAnalyzer(const std::string& dictionary, const std::string& voice)
        : state_(std::make_unique<State>()) {
        if (!Mecab_load(&state_->mecab, dictionary.c_str()))
            throw std::runtime_error("Cannot load Open JTalk dictionary: " + dictionary);
        std::string path = voice;
        char* models[] = { path.data() };  // hts_engine 1.10 takes char**
        if (!HTS_Engine_load(&state_->engine, models, 1))
            throw std::runtime_error("Cannot load HTS voice: " + voice);
        if (std::strcmp(HTS_Engine_get_fullcontext_label_format(&state_->engine), "HTS_TTS_JPN") != 0)
            throw std::invalid_argument("Expected a Japanese HTS_TTS_JPN voice");
    }

    NesoraProsodyData NesoraAnalyzer::Analyze(const std::string& utf8, double speed) {
        // 1 回につき有効な UTF-8 の短い発話を入力する。
        if (utf8.size() > 1023 || utf8.find('\0') != std::string::npos)
            throw std::invalid_argument("Expected one short UTF-8 utterance without NUL");
        if (!std::isfinite(speed) || speed < 0.25 || speed > 4.0)
            throw std::invalid_argument("Sample supports speed from 0.25 to 4.0");

        auto& s = *state_;
        s.reset_utterance();
        auto* engine = &s.engine;

        //HTS_Engine_add_half_tone(engine, 1.0);

        NesoraProsodyData out{};
        out.frame_seconds = static_cast<double>(HTS_Engine_get_fperiod(engine))
            / HTS_Engine_get_sampling_frequency(engine);
        out.frame_size = HTS_Engine_get_fperiod(engine);
        if (utf8.empty()) return out;

        // ASCII の全角化で 1 バイトが最大 3 バイトに増える。
        std::vector<char> normalized(3 * utf8.size() + 1);
        text2mecab(normalized.data(), utf8.c_str());
        if (std::strlen(normalized.data()) > 1023)
            throw std::invalid_argument("Split long input into shorter utterances");
        if (!Mecab_analysis(&s.mecab, normalized.data()))
            throw std::runtime_error("MeCab analysis failed");
        if (!Mecab_get_size(&s.mecab)) return out;

        mecab2njd(&s.njd, Mecab_get_feature(&s.mecab), Mecab_get_size(&s.mecab));
        njd_set_pronunciation(&s.njd);
        njd_set_digit(&s.njd);
        njd_set_accent_phrase(&s.njd);
        njd_set_accent_type(&s.njd);
        njd_set_unvoiced_vowel(&s.njd);
        njd_set_long_vowel(&s.njd);
        njd2jpcommon(&s.jp, &s.njd);
        JPCommon_make_label(&s.jp);
        const int count = JPCommon_get_label_size(&s.jp);
        if (count <= 2) return out;

        HTS_Engine_set_speed(engine, speed);
        if (!HTS_Engine_synthesize_from_strings(engine, JPCommon_get_label_feature(&s.jp), count))
            throw std::runtime_error("HTS synthesis failed");

        const auto frame_count = HTS_Engine_get_total_frame(engine);
        out.f0.reserve(frame_count);
        for (std::size_t i = 0; i < frame_count; ++i) {
            const double lf0 = HTS_Engine_get_generated_parameter(engine, 1, i, 0);
            out.f0.push_back(lf0 == HTS_NODATA ? 0.0 : std::exp(lf0));
        }

        // 1 ラベル = 1 音素。ラベル列の両端には sil が追加される。
        auto* phone = s.jp.label->phoneme_head;
        const auto states_per_phone = HTS_Engine_get_nstate(engine);
        std::size_t frame = 0, state = 0;
        for (int i = 0; i < count; ++i) {
            const auto begin = frame;
            for (std::size_t j = 0; j < states_per_phone; ++j)
                frame += HTS_Engine_get_state_duration(engine, state++);
            if (i == 0 || i == count - 1) {
                out.phones.push_back({ "sil", begin, frame });
                continue;
            }
            out.phones.push_back({ phone->phoneme, begin, frame });
            auto* mora = phone->up;   // pau には所属モーラがない
            if (mora) {
                auto* phrase = mora->up->up;
                if (phone == phrase->head->head->head)
                    out.phrases.push_back({ phrase->accent, phrase == phrase->up->head,{} });
                auto& moras = out.phrases.back().moras;
                if (phone == mora->head)
                    moras.push_back({ mora->mora, begin, frame });
                else
                    moras.back().end_frame = frame;
            }
            phone = phone->next;
        }
        if (frame != frame_count)
            throw std::runtime_error("Phone durations and F0 length disagree");
        return out;
    }



} // namespace NesoraFuru