// NesoraFuru.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "NesoraWave.h"

namespace NesoraFuru {

	// 正弦波一つ分のパラメータ
	struct SinWaveParameter {
		double frequency;	// 角速度
		double amplitude;		// 振幅
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

		void Add(const FormantParameter& formant) {
			formants.push_back(formant);
		}

		std::vector<SinWaveParameter> Calculate(double pitch, int outSize);		// フォルマントから各倍音の振幅を計算する

	private:

		std::vector<FormantParameter> formants;

	};

	// 音声合成器
	class NesoraSynthesizer {
	public:

		NesoraSynthesizer();

		std::vector<double> Synthesize(std::vector<double> pitch, std::vector<double> amplitude, std::vector<NesoraFormants> formants);		// 音声を合成する

		void SetNumberOfMultiples(size_t num) {
			number_of_multiples = num;
			phase_buffer.resize(number_of_multiples);
		}
		void SetSamplingFrequency(double smpl) {
			samplingFrequency = smpl;
		}

	private:

		std::vector<double> phase_buffer;

		size_t number_of_multiples = 400;
		double samplingFrequency = 48000.0;

	};


	// 

}
