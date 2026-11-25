#include "NesoraFuru.h"
#include <cstdio>
#include <exception>

extern "C" const char* nesora_synthesize_wav(
    const char* dictionary,  // Open JTalk の辞書フォルダ
    const char* model,       // .htsvoice ファイル
    const char* json,        // NesoraFuru.json
    const char* text,        // 読み上げる文章（UTF-8）
    const char* wav) noexcept
{
    if (!dictionary || !*dictionary || !model || !*model ||
        !json || !*json || !text || !*text || !wav || !*wav)
        return "A path or the input text is empty.";

    try {
        nesora::NesoraFuru furu(dictionary, model);

        if (furu.LoadVoice(json))
            return "Could not load voice parameters from JSON.";

        const auto samples = furu.Synthesize(text);
        if (!writeWavFile(wav, samples, 48000))
            return "Could not write the WAV file.";

        return nullptr;  // 成功
    }
    catch (const std::exception& e) {
        static thread_local char message[1024];
        std::snprintf(message, sizeof(message), "%s", e.what());
        return message;
    }
    catch (...) {
        return "Unknown native error.";
    }
}