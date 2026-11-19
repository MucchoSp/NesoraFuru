// NesoraFuru.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <nlohmann/json.hpp>

#include "NesoraWave.h"

#ifndef NESORA_FURU
#define NESORA_FURU

namespace NesoraFuru {

	constexpr double PI = 3.14159265358979323846;

	template<typename T>
	inline T pow2(T x) { return x * x; }

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


	inline void from_json(const nlohmann::json& j, FormantParameter& p) {
		if (!j.contains("frequency")) {
			std::cout << "frequencyがないよ" << std::endl;
			return;
		}
		if (!j.contains("intensity")) {
			std::cout << "intensityがないよ" << std::endl;
			return;
		}
		if (!j.contains("width")) {
			std::cout << "widthがないよ" << std::endl;
			return;
		}
		p.frequency = j.at("frequency").get<double>();
		p.intensity = j.at("intensity").get<double>();
		p.width = j.at("width").get<double>();
	}




	// フォルマントの管理とスペクトルの計算
	class NesoraFormants {
	public:

		NesoraFormants() {}
		NesoraFormants(std::vector<FormantParameter> in_formants) {
			formants = in_formants;
		}

		std::vector<FormantParameter> GetFormants() {
			return formants;
		}
		const std::vector<FormantParameter> GetFormants() const {
			return formants;
		}

		// フォルマントを追加
		void Add(const FormantParameter& formant) {
			formants.push_back(formant);
		}
		// フォルマントを設定
		void SetFormants(const std::vector<FormantParameter>& formant) {
			formants = formant;
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

		size_t number_of_multiples = 80;
		double samplingFrequency = 48000.0;

	};









	struct NesoraFuruVoiceData {
		std::string name;
		std::string version;
		double f0;
		std::map<std::string, std::vector<FormantParameter>> formants;
	};

	class NesoraFuruVoice {
	public:

		NesoraFuruVoice() {}

		void LoadVoice(std::string filepath);

		void SetNesoraFuru();

		NesoraFuruVoiceData GetData() {
			return data;
		}


	private:

		NesoraFuruVoiceData data;

	};



	// 合成器
	// 最終目標
	class NesoraFuru {
	public:

		NesoraFuru() {};

		// 日本語の文章を入力して合成をする
		std::vector<double> Synthesize(std::string script);
		void SetVoice(NesoraFuruVoice nesoraFuruVocie) {
			voice = nesoraFuruVocie;
		}
		void LoadVoice(std::string filepath) {
			voice.LoadVoice(filepath);
		}

	private:

		std::string script;
		NesoraFuruVoice voice;
		NesoraSynthesizer synthesizer;

	};

}// namespace NesoraFuru

#endif // NESORA_FURU
