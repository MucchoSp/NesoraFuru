// NesoraFuru.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "NesoraWave.h"

#ifndef NESORA_FURU
#define NESORA_FURU

namespace NesoraFuru {

	// 正弦波一つ分のパラメータ
	struct SinWaveParameter {
		double frequency;	// 角速度
		double amplitude;	// 振幅
	};

	// フォルマント一つ分のパラメータ
	struct FormantParameter {
		double frequency;	// 周波数
		double intensity;	// 強度
		double width;		// 幅
	};

	// 音声波形のデータ
	struct WaveData {
		std::vector<double> wave;
		double samplingFrequency;
	};




	// フォルマントの管理とスペクトルの計算
	class NesoraFormants {
	public:

		NesoraFormants() {}

		// フォルマントを追加
		void Add(const FormantParameter& formant) {
			formants.push_back(formant);
		}

		// フォルマントから各倍音の振幅を計算する
		std::vector<SinWaveParameter> Calculate(double pitch, int outSize);

	private:

		std::vector<FormantParameter> formants;

	};

	// 音声合成器
	class NesoraSynthesizer {
	public:

		NesoraSynthesizer();

		// 音声を合成する
		std::vector<double> Synthesize(std::vector<double> pitch, std::vector<double> amplitude, std::vector<NesoraFormants> formants, std::vector<double> fricative);

		// 倍音の数
		void SetNumberOfMultiples(size_t num) {
			number_of_multiples = num;
			phase_buffer.resize(number_of_multiples);
		}
		// 標本化周波数
		void SetSamplingFrequency(double smpl) {
			samplingFrequency = smpl;
		}

	private:

		std::vector<double> phase_buffer;

		size_t number_of_multiples = 400;
		double samplingFrequency = 48000.0;

	};















	// 合成器
	// 最終目標
	class NesoraFuru {
	public:

		NesoraFuru() {};

		// 日本語の文章を入力して合成をする
		std::vector<double> Synthesize(std::string script) {}
		void SetVoice() {}

	private:

		std::string script;

	};

}// namespace NesoraFuru

#endif // NESORA_FURU
