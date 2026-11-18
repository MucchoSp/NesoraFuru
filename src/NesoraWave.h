#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cmath>

#ifndef NESORA_WAVE
#define NESORA_WAVE

// WAVヘッダー構造体（44バイト）
#pragma pack(push, 1)
struct WavHeader {
    // RIFF Chunk Descriptor
    char     chunkId[4] = { 'R', 'I', 'F', 'F' };
    uint32_t chunkSize;       // 36 + subchunk2Size
    char     format[4] = { 'W', 'A', 'V', 'E' };

    // fmt Subchunk
    char     subchunk1Id[4] = { 'f', 'm', 't', ' ' };
    uint32_t subchunk1Size = 16;  // PCMなら16
    uint16_t audioFormat = 1;   // PCM = 1
    uint16_t numChannels = 1;   // モノラル = 1, ステレオ = 2
    uint32_t sampleRate = 44100;
    uint32_t byteRate;        // sampleRate * numChannels * bitsPerSample / 8
    uint16_t blockAlign;      // numChannels * bitsPerSample / 8
    uint16_t bitsPerSample = 16;  // 16bit PCM

    // data Subchunk
    char     subchunk2Id[4] = { 'd', 'a', 't', 'a' };
    uint32_t subchunk2Size;   // データ本体のバイト数
};
#pragma pack(pop)

// wavファイル出力関数
inline bool writeWavFile(const std::string& filename, const std::vector<double>& audioData, uint32_t sampleRate = 44100) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
        return false;
    }

    uint16_t numChannels = 1;
    uint16_t bitsPerSample = 16;

    // ヘッダー情報の計算
    WavHeader header;
    header.sampleRate = sampleRate;
    header.numChannels = numChannels;
    header.bitsPerSample = bitsPerSample;
    header.blockAlign = numChannels * (bitsPerSample / 8);
    header.byteRate = sampleRate * header.blockAlign;
    header.subchunk2Size = static_cast<uint32_t>(audioData.size() * header.blockAlign);
    header.chunkSize = 36 + header.subchunk2Size;

    // 1. ヘッダーの書き込み
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // 2. double (-1.0 ～ 1.0) を int16_t (-32768 ～ 32767) に変換して書き込み
    for (double sample : audioData) {
        // クリッピング処理（-1.0〜1.0 の範囲外を安全に制限）
        sample = std::clamp(sample, -1.0, 1.0);

        // 16bit整数にスケール変換
        int16_t pcmSample = static_cast<int16_t>(sample * 32767.0);

        file.write(reinterpret_cast<const char*>(&pcmSample), sizeof(pcmSample));
    }

    return true;
}

#endif // NESORA_WAVE
