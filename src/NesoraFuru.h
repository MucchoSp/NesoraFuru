// 音諳零号機四型
// 音諳ふる

#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include <nlohmann/json.hpp>

#include "NesoraSynthesize.h"
#include "NesoraWave.h"
#include "NesoraType.h"

#ifndef NESORA_FURU
#define NESORA_FURU

namespace nesora {

	template<typename T>
	inline T pow2(T x) { return x * x; }

	// 音声波形のデータ
	struct WaveData {
		std::vector<double> wave;
		double samplingFrequency;
	};


	inline void from_json(const nlohmann::json& j, NesoraFormantData& p) {
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
		p.f = j.at("frequency").get<double>();
		p.h = j.at("intensity").get<double>();
		p.w = j.at("width").get<double>();
	}

	inline void from_json(const nlohmann::json& j, NesoraTimeVariable<double>& p) {
		if (!j.contains("time")) {
			std::cout << "timeがないよ" << std::endl;
			return;
		}
		if (!j.contains("value")) {
			std::cout << "valueがないよ" << std::endl;
			return;
		}

		p.time = j.at("time").get<double>() * 0.25;
		p.v = j.at("value").get<double>();
	}

	inline void from_json(const nlohmann::json& j, NesoraTimeVariable<NesoraSynthesizeVowelVector>& p) {
		if (!j.contains("time")) {
			std::cout << "timeがないよ" << std::endl;
			return;
		}
		if (!j.contains("vector")) {
			std::cout << "vectorがないよ" << std::endl;
			return;
		}

		p.time = j.at("time").get<double>() * 0.25;
		p.v = j.at("vector").get<std::array<double, 3>>();
	}









	struct NesoraSegment {
		std::string text;
		size_t begin_frame, end_frame;
	};

	struct NesoraAccentPhrase {
		int nucleus;
		bool starts_breath_group;
		std::vector<NesoraSegment> moras;
	};

	struct NesoraProsodyData {
		double frame_seconds;
		size_t frame_size;
		std::vector<NesoraSegment> phones;
		std::vector<NesoraAccentPhrase> phrases;
		std::vector<double> f0;
	};

	// OpenJTalkとのやり取り
	class NesoraAnalyzer {
	public:

		NesoraAnalyzer(const std::string& dictionaly, const std::string& voice);
		~NesoraAnalyzer();

		NesoraProsodyData Analyze(const std::string& script, double speed = 1.0);

	private:

		struct State;
		std::unique_ptr<State> state_;

	};






	namespace NesoraVowelVectorConstant {
		const NesoraSynthesizeVowelVector A = std::array<double, 3>{ 1.0, 0.0, 0.0 };
		const NesoraSynthesizeVowelVector I = std::array<double, 3>{ 0.0, 1.0, 0.0 };
		const NesoraSynthesizeVowelVector U = std::array<double, 3>{ 0.0, 0.0, 0.0 };
		const NesoraSynthesizeVowelVector E = std::array<double, 3>{ 0.5, 1.0, 0.0 };
		const NesoraSynthesizeVowelVector O = std::array<double, 3>{ 0.5, 0.0, 0.0 };
		const NesoraSynthesizeVowelVector nasalN = std::array<double, 3>{ 1.0, 0.0, 1.0 };
		const NesoraSynthesizeVowelVector nasalM = std::array<double, 3>{ 0.0, 0.0, 1.0 };
		const NesoraSynthesizeVowelVector flapR = std::array<double, 3>{ 0.0, 1.0, 1.0 };
	}


	struct NesoraFuruConstantData {
		NesoraFuruConstantData()
			: vectors{}, volumes{}, fricatives{}, frequencys{}
		{
		}

		std::vector<NesoraTimeVariable<NesoraSynthesizeVowelVector>> vectors;
		std::vector<NesoraTimeVariable<double>> volumes, fricatives, frequencys;
		
		NesoraSynthesizeVector GenerateVector(double len) const;
	};


	inline void from_json(const nlohmann::json& j, NesoraFuruConstantData& p) {
		if (!j.contains("vowelVetors")) {
			std::cout << "vowelVetorsがないよ" << std::endl;
			return;
		}
		if (!j.contains("volumes")) {
			std::cout << "volumesがないよ" << std::endl;
			return;
		}
		if (!j.contains("fricatives")) {
			std::cout << "fricativesがないよ" << std::endl;
			return;
		}
		if (!j.contains("frequencys")) {
			std::cout << "frequencysがないよ" << std::endl;
			return;
		}

		p.vectors = j.at("vowelVetors").get<std::vector<NesoraTimeVariable<NesoraSynthesizeVowelVector>>>();
		p.volumes = j.at("volumes").get<std::vector<NesoraTimeVariable<double>>>();
		p.fricatives = j.at("fricatives").get<std::vector<NesoraTimeVariable<double>>>();
		p.frequencys = j.at("frequencys").get<std::vector<NesoraTimeVariable<double>>>();
	}


	struct NesoraFuruVoiceData {
		NesoraFuruVoiceData()
			: name{}, version{}, voice{}, constants{}, f0(0.0)
		{
		}

		std::string name;
		std::string version;
		NesoraVoice voice;
		std::map<std::string, NesoraFuruConstantData> constants;
		double f0;
	};

	// 声
	class NesoraFuruVoice {
	public:

		NesoraFuruVoice()
			: data{}
		{
		}

		void LoadVoice(std::string filepath);

		void SetNesoraFuru();

		NesoraFuruVoiceData GetData() {
			return data;
		}


		NesoraTimeArray<NesoraSynthesizeVowelVector> GenerateFromants(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency);
		NesoraTimeArray<double> GenerateFricatives(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency, double fric);
		NesoraTimeArray<double> GenerateVolumes(std::vector<NesoraSegment> segments, size_t frame_size, double sampling_frequency, double volume);
		NesoraSynthesizeVector GenerateSynthesizeVector(NesoraProsodyData prosodyData);

	private:

		NesoraFuruVoiceData data;

	};



	// 合成器
	// 最終目標
	class NesoraFuru {
	public:

		NesoraFuru() : analyzer("D:\\open_jtalk_dic_utf_8-1.11", "D:\\MMDAgent_Example-1.8\\Voice\\mei\\mei_normal.htsvoice") {}
		NesoraFuru(const std::string& dictionaly, const std::string& voice) : analyzer(dictionaly, voice) {}

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
		NesoraSynthesizeFormant synthesizer;
		NesoraAnalyzer analyzer;

		double samplingFrequency = 48000.0;

	};

}// namespace NesoraFuru

#endif // NESORA_FURU
