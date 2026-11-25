#include "NesoraFuru.h"

namespace nesora {


    void savePhonesAsTextGrid(const std::vector<NesoraSegment>& phones,
        const std::filesystem::path& output_path,
        double seconds_per_frame)
    {
        if (!std::isfinite(seconds_per_frame) || seconds_per_frame <= 0.0) {
            throw std::invalid_argument("seconds_per_frame must be finite and positive");
        }

        std::size_t interval_count = 0;
        std::size_t previous_end = 0;
        for (const auto& phone : phones) {
            if (phone.begin_frame < previous_end ||
                phone.end_frame <= phone.begin_frame) {
                throw std::invalid_argument(
                    "Phone intervals must be ordered, non-overlapping, and non-empty");
            }

            interval_count += (phone.begin_frame > previous_end) ? 2 : 1;
            previous_end = phone.end_frame;
        }

        const auto to_seconds = [seconds_per_frame](std::size_t frame) {
            return static_cast<double>(frame) * seconds_per_frame;
            };

        const double xmax = to_seconds(previous_end);
        if (!std::isfinite(xmax)) {
            throw std::invalid_argument("TextGrid duration is out of range");
        }

        std::ofstream out;
        out.exceptions(std::ios::failbit | std::ios::badbit);
        out.open(output_path, std::ios::out | std::ios::binary);
        out.imbue(std::locale::classic());
        out << std::setprecision(std::numeric_limits<double>::max_digits10);

        out << "File type = \"ooTextFile\"\n"
            << "Object class = \"TextGrid\"\n\n"
            << "xmin = 0\n"
            << "xmax = " << xmax << '\n'
            << "tiers? <exists>\n"
            << "size = 1\n"
            << "item []:\n"
            << "    item [1]:\n"
            << "        class = \"IntervalTier\"\n"
            << "        name = \"phones\"\n"
            << "        xmin = 0\n"
            << "        xmax = " << xmax << '\n'
            << "        intervals: size = " << interval_count << '\n';

        std::size_t index = 1;
        const auto write_interval = [&](std::size_t begin, std::size_t end,
            const std::string& label) {
                out << "        intervals [" << index++ << "]:\n"
                    << "            xmin = " << to_seconds(begin) << '\n'
                    << "            xmax = " << to_seconds(end) << '\n'
                    << "            text = \"";

                for (const char ch : label) {
                    if (ch == '"') {
                        out.put('"');
                    }
                    out.put(ch);
                }
                out << "\"\n";
            };

        previous_end = 0;
        for (const auto& phone : phones) {
            if (phone.begin_frame > previous_end) {
                write_interval(previous_end, phone.begin_frame, "");
            }
            write_interval(phone.begin_frame, phone.end_frame, phone.text);
            previous_end = phone.end_frame;
        }

        out.close();
    }




	// MARK: NesoraFuru

	std::vector<double> NesoraFuru::Synthesize(std::string script) {
        NesoraProsodyData prosody = analyzer.Analyze(script, 1.0);
        //savePhonesAsTextGrid(prosody.phones, "Phones.textgrid", prosody.frame_seconds);
		return voice.GenerateSynthesizeVector(prosody).SynthesizeFormant().MakeVoiceWave(samplingFrequency);
	}

} // namespace nesora
