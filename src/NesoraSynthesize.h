/*
* Nesora 0 type IV
* Copyright (c) MucchoSP. All right reserved.
*
*
* 音諳用に作成した音声合成用プログラムです。
*
*/
#pragma once


#ifndef NESORA_SYNTHESIZE
#define NESORA_SYNTHESIZE

/*
* Nesora 0 type IV
* 音諳零号機四型
* 
* 音諳の低レイヤー担当のプログラム群。
* 低レイヤーでの音響学的な音声生成の部分を抽象化し、
* より高レイヤーでの音声学、音韻学的なところを補助する役割を持つ。
* 
* そうなれるように頑張る。
*/

#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

#include "NesoraType.h"
#include "NesoraGenericClass.h"


namespace nesora {

	//	+---------------------------------------------------------------------------------------+
	//	|	音諳フォルマント合成	NesoraSynthesizeFormant										|
	//	+---------------------------------------------------------------------------------------+

	// フォルマントを表現するための構造体。
	// 正規表現を応用したフォルマント関数用の変数がまとまっている。
	// 実際に使う時の式を以下に示す。
	// xは周波数、yは振幅を表す。
	// 
	// y = h*e^((x-f)/w)^2
	//
	struct NesoraFormantData {
		nsfreq_t f;		//フォルマント周波数
		nsfloat h;		//振幅
		nsfloat w;		//幅
	};

	//フォルマント合成用の情報をまとめた構造体
	//八つのフォルマントデータがある。
	struct NesoraSynthesizeFormantData {
		std::vector<NesoraFormantData>buffer;

		NesoraSynthesizeFormantData();
		NesoraSynthesizeFormantData(const NesoraSynthesizeFormantData& nssfd);
		NesoraSynthesizeFormantData& operator=(const NesoraSynthesizeFormantData& nssfd);

		const NesoraSynthesizeFormantData operator*(const nsfloat&) const;
		const NesoraSynthesizeFormantData operator/(const nsfloat&) const;
		const NesoraSynthesizeFormantData operator+(const NesoraSynthesizeFormantData&) const;
		const NesoraSynthesizeFormantData operator-(const NesoraSynthesizeFormantData&) const;

		const bool operator==(const NesoraSynthesizeFormantData&) const;
		const bool operator!=(const NesoraSynthesizeFormantData&) const;
	};




	//音諳のフォルマント合成用のクラス
	class NesoraSynthesizeFormant {
	private:

		NesoraTimeArray<NesoraSynthesizeFormantData> formants;	//フォルマント
		NesoraTimeArray<nsfreq_t> freq;		//声の高さ
		NesoraTimeArray<nsfloat> vol;		//声の大きさ
		NesoraTimeArray<nsfloat> fric;		//声のかすれ率
		NesoraTimeArray<nsfloat> nasal;		//声の鼻音率

	public:

		NesoraSynthesizeFormant();

		void SetFormants(NesoraTimeArray<NesoraSynthesizeFormantData> formantDatas);
		void SetFrequencys(NesoraTimeArray<nsfreq_t> frequencyDatas);
		void SetVolumes(NesoraTimeArray<nsfloat> volumeDatas);
		void SetFricatives(NesoraTimeArray<nsfloat> fricativeDatas);
		void SetNasal(NesoraTimeArray<nsfloat> nasalDatas);

		void AddFormant(NesoraTimeVariable<NesoraSynthesizeFormantData> formantDatas);
		void AddFrequency(NesoraTimeVariable<nsfreq_t> frequencyDatas);
		void AddVolume(NesoraTimeVariable<nsfloat> volumeDatas);
		void AddFricative(NesoraTimeVariable<nsfloat> fricativeDatas);
		void AddNasal(NesoraTimeVariable<nsfloat>nasalDatas);

		void AddFormant(nsfloat t, NesoraSynthesizeFormantData formantData);
		void AddFrequency(nsfloat t, nsfreq_t frequencyData);
		void AddVolume(nsfloat t, nsfloat volumeData);
		void AddFricative(nsfloat t, nsfloat fricativeData);
		void AddNasal(nsfloat t, nsfloat nasalData);

		std::vector<nsfloat> MakeVoiceWave(const nsint& samplingFrequency);
		std::vector<nsfloat> MakePhonemeWave(const nsint& samplingFrequency);

	};











	//	+---------------------------------------------------------------------------------------+
	//	|	音諳ベクトル合成		NesoraSynthesizeVector										|
	//	+---------------------------------------------------------------------------------------+










	//音諳の声データ
	//音諳の個体別の声のデータを保存するためのやつ
	struct NesoraVoice {
		NesoraVoice();

		std::array<NesoraSynthesizeFormantData, 8> vowelFormantCube;
		nsfreq_t maxFrequency = 0, minFrequency = 0;
	};



	//音諳の母音ベクトル用のクラス
	//音諳の時系列順の台本データを保存するためのやつ。
	struct NesoraSynthesizeVowelVector {
		nsfloat x = (nsfloat)0, y = (nsfloat)0, z = (nsfloat)0;

		NesoraSynthesizeVowelVector();
		NesoraSynthesizeVowelVector(std::array<double, 3>);

		const NesoraSynthesizeVowelVector operator*(const nsfloat&) const;
		const NesoraSynthesizeVowelVector operator/(const nsfloat&) const;
		const NesoraSynthesizeVowelVector operator+(const NesoraSynthesizeVowelVector&) const;
		const NesoraSynthesizeVowelVector operator-(const NesoraSynthesizeVowelVector&) const;

		const bool operator==(const NesoraSynthesizeVowelVector&) const;
		const bool operator!=(const NesoraSynthesizeVowelVector&) const;
	};




	//音諳のベクトル合成用のクラス
	class NesoraSynthesizeVector {
	private:

		NesoraVoice voice;	//声

		NesoraTimeArray<NesoraSynthesizeVowelVector> vowelVector;	//声の母音ベクトル
		NesoraTimeArray<nsfreq_t> freq;		//声の高さ
		NesoraTimeArray<nsfloat> vol;		//声の大きさ
		NesoraTimeArray<nsfloat> fric;		//声のかすれ率
		NesoraTimeArray<nsfloat> nasal;		//声の鼻音率

		NesoraSynthesizeFormant outputFormant;

	public:

		NesoraSynthesizeVector();

		void SetVoice(const NesoraVoice&);
		void SetVoice(const nsint idx, const NesoraSynthesizeFormantData&);

		void SetVowelVectors(NesoraTimeArray<NesoraSynthesizeVowelVector> vowelVectorDatas);
		void SetFrequencys(NesoraTimeArray<nsfreq_t> frequencyDatas);
		void SetVolumes(NesoraTimeArray<nsfloat> volumeDatas);
		void SetFricatives(NesoraTimeArray<nsfloat> fricativeDatas);
		void SetNasal(NesoraTimeArray<nsfloat> nasalDatas);

		void AddVowelVectors(NesoraTimeVariable<NesoraSynthesizeVowelVector> vowelVectorDatas);
		void AddFrequencys(NesoraTimeVariable<nsfreq_t> frequencyDatas);
		void AddVolumes(NesoraTimeVariable<nsfloat> volumeDatas);
		void AddFricatives(NesoraTimeVariable<nsfloat> fricativeDatas);
		void AddNasal(NesoraTimeVariable<nsfloat>nasalDatas);

		void AddVowelVector(nsfloat t, NesoraSynthesizeVowelVector vowelVectorData);
		void AddFrequency(nsfloat t, nsfreq_t frequencyData);
		void AddVolume(nsfloat t, nsfloat volumeData);
		void AddFricative(nsfloat t, nsfloat fricativeDatas);
		void AddNasal(nsfloat t, nsfloat nasalData);

		NesoraSynthesizeFormant SynthesizeFormant();

		void push_back(const NesoraSynthesizeVector&);

	};


}//namespace nesora



#endif//NESORA_SYNTHESIZE


