#include "NesoraSynthesize.h"


#include <iostream>



namespace nesora {



	//	+---------------------------------------------------------------------------------------+
	//	|	音諳ベクトル合成		NesoraSynthesizeVector										|
	//	+---------------------------------------------------------------------------------------+

	NesoraSynthesizeVector::NesoraSynthesizeVector() {



	}

	void NesoraSynthesizeVector::SetVoice(const NesoraVoice& inVoice) {
		voice = inVoice;
	}

	void NesoraSynthesizeVector::SetVoice(const nsint idx, const NesoraSynthesizeFormantData& inVoice) {
		voice.vowelFormantCube[idx] = inVoice;
	}


	void NesoraSynthesizeVector::SetVowelVectors(NesoraTimeArray<NesoraSynthesizeVowelVector> vowelVectorDatas) {
		vowelVector = vowelVectorDatas;
	}

	void NesoraSynthesizeVector::SetFrequencys(NesoraTimeArray<nsfloat> frequencyDatas) {
		freq = frequencyDatas;
	}

	void NesoraSynthesizeVector::SetVolumes(NesoraTimeArray<nsfloat> volumeDatas) {
		vol = volumeDatas;
	}

	void NesoraSynthesizeVector::SetFricatives(NesoraTimeArray<nsfloat> fricativeDatas) {
		fric = fricativeDatas;
	}

	void NesoraSynthesizeVector::SetNasal(NesoraTimeArray<nsfloat> nasalDatas) {
		nasal = nasalDatas;
	}


	void NesoraSynthesizeVector::AddVowelVectors(NesoraTimeVariable<NesoraSynthesizeVowelVector> vowelVectorDatas) {
		vowelVector.insert(vowelVectorDatas);
	}

	void NesoraSynthesizeVector::AddFrequencys(NesoraTimeVariable<nsfreq_t> frequencyDatas) {
		freq.insert(frequencyDatas);
	}

	void NesoraSynthesizeVector::AddVolumes(NesoraTimeVariable<nsfloat> volumeDatas) {
		vol.insert(volumeDatas);
	}

	void NesoraSynthesizeVector::AddFricatives(NesoraTimeVariable<nsfloat> fricativeDatas) {
		fric.insert(fricativeDatas);
	}

	void NesoraSynthesizeVector::AddNasal(NesoraTimeVariable<nsfloat> nasalDatas) {
		nasal.insert(nasalDatas);
	}


	void NesoraSynthesizeVector::AddVowelVector(nsfloat t, NesoraSynthesizeVowelVector vowelVectorData) {
		vowelVector.insert(t, vowelVectorData);
	}

	void NesoraSynthesizeVector::AddFrequency(nsfloat t, nsfreq_t frequencyData) {
		freq.insert(t, frequencyData);
	}

	void NesoraSynthesizeVector::AddVolume(nsfloat t, nsfloat volumeData) {
		vol.insert(t, volumeData);
	}

	void NesoraSynthesizeVector::AddFricative(nsfloat t, nsfloat fricativeDatas) {
		fric.insert(t, fricativeDatas);
	}

	void NesoraSynthesizeVector::AddNasal(nsfloat t, nsfloat nasalDatas) {
		nasal.insert(t, nasalDatas);
	}


	NesoraSynthesizeFormant NesoraSynthesizeVector::SynthesizeFormant() {
		outputFormant.SetFrequencys(freq);
		outputFormant.SetVolumes(vol);
		outputFormant.SetFricatives(fric);

		for (const auto& vv : vowelVector.GetBuffer()) {
			NesoraSynthesizeFormantData formant = WeightCalc(
				WeightCalc(
					WeightCalc(voice.vowelFormantCube[0], voice.vowelFormantCube[2], vv.v.x),
					WeightCalc(voice.vowelFormantCube[1], voice.vowelFormantCube[3], vv.v.x),
					vv.v.y),
				WeightCalc(
					WeightCalc(voice.vowelFormantCube[4], voice.vowelFormantCube[6], vv.v.x),
					WeightCalc(voice.vowelFormantCube[5], voice.vowelFormantCube[7], vv.v.x),
					vv.v.y), 
				vv.v.z);
			outputFormant.AddFormant(vv.time, formant);
		}

		return outputFormant;
	}

	void NesoraSynthesizeVector::push_back(const NesoraSynthesizeVector& invec) {
		nsfloat backtime = vowelVector.GetMaxTime();
		vowelVector.push_back(invec.vowelVector, backtime);
		freq.push_back(invec.freq, backtime);
		vol.push_back(invec.vol, backtime);
		fric.push_back(invec.fric, backtime);
		nasal.push_back(invec.nasal, backtime);
		std::cout << vowelVector.GetMaxTime() << std::endl;
	}







	NesoraSynthesizeVowelVector::NesoraSynthesizeVowelVector() {

	}

	NesoraSynthesizeVowelVector::NesoraSynthesizeVowelVector(std::array<double, 3> v) {
		x = v[0];
		y = v[1];
		z = v[2];
	}

	const NesoraSynthesizeVowelVector NesoraSynthesizeVowelVector::operator*(const nsfloat& t) const {
		NesoraSynthesizeVowelVector out = *this;
		out.x *= t;
		out.y *= t;
		out.z *= t;
		return out;
	}

	const NesoraSynthesizeVowelVector NesoraSynthesizeVowelVector::operator/(const nsfloat& t) const {
		NesoraSynthesizeVowelVector out = *this;
		out.x /= t;
		out.y /= t;
		out.z /= t;
		return out;
	}

	const NesoraSynthesizeVowelVector NesoraSynthesizeVowelVector::operator+(const NesoraSynthesizeVowelVector& t) const {
		NesoraSynthesizeVowelVector out = *this;
		out.x += t.x;
		out.y += t.y;
		out.z += t.z;
		return out;
	}

	const NesoraSynthesizeVowelVector NesoraSynthesizeVowelVector::operator-(const NesoraSynthesizeVowelVector& t) const {
		NesoraSynthesizeVowelVector out = *this;
		out.x -= t.x;
		out.y -= t.y;
		out.z -= t.z;
		return out;
	}

	const bool NesoraSynthesizeVowelVector::operator==(const NesoraSynthesizeVowelVector& t) const {
		if ((*this).x != t.x) return false;
		if ((*this).y != t.y) return false;
		if ((*this).z != t.z) return false;
		return true;
	}

	const bool NesoraSynthesizeVowelVector::operator!=(const NesoraSynthesizeVowelVector& t) const {
		if ((*this).x != t.x) return true;
		if ((*this).y != t.y) return true;
		if ((*this).z != t.z) return true;
		return false;
	}















	//	+---------------------------------------------------------------------------------------+
	//	|	音諳フォルマント合成	NesoraSynthesizeFormant										|
	//	+---------------------------------------------------------------------------------------+

	NesoraSynthesizeFormant::NesoraSynthesizeFormant() {

	}



	void NesoraSynthesizeFormant::SetFormants(NesoraTimeArray<NesoraSynthesizeFormantData> formantDatas) {
		formants = formantDatas;
	}

	void NesoraSynthesizeFormant::SetFrequencys(NesoraTimeArray<nsfloat> frequencyDatas) {
		freq = frequencyDatas;
	}

	void NesoraSynthesizeFormant::SetVolumes(NesoraTimeArray<nsfloat> volumeDatas) {
		vol = volumeDatas;
	}

	void NesoraSynthesizeFormant::SetFricatives(NesoraTimeArray<nsfloat> fricativeDatas) {
		fric = fricativeDatas;
	}

	void NesoraSynthesizeFormant::SetNasal(NesoraTimeArray<nsfloat> nasalDatas) {
		nasal = nasalDatas;
	}


	void NesoraSynthesizeFormant::AddFormant(NesoraTimeVariable<NesoraSynthesizeFormantData> formantDatas) {
		formants.insert(formantDatas);
	}

	void NesoraSynthesizeFormant::AddFrequency(NesoraTimeVariable<nsfreq_t> frequencyDatas) {
		freq.insert(frequencyDatas);
	}

	void NesoraSynthesizeFormant::AddVolume(NesoraTimeVariable<nsfloat> volumeDatas) {
		vol.insert(volumeDatas);
	}

	void NesoraSynthesizeFormant::AddFricative(NesoraTimeVariable<nsfloat> fricativeDatas) {
		fric.insert(fricativeDatas);
	}

	void NesoraSynthesizeFormant::AddNasal(NesoraTimeVariable<nsfloat> nasalDatas) {
		nasal.insert(nasalDatas);
	}


	void NesoraSynthesizeFormant::AddFormant(nsfloat t, NesoraSynthesizeFormantData formantData) {
		formants.insert(t, formantData);
	}

	void NesoraSynthesizeFormant::AddFrequency(nsfloat t, nsfreq_t frequencyData) {
		freq.insert(t, frequencyData);
	}

	void NesoraSynthesizeFormant::AddVolume(nsfloat t, nsfloat volumeData) {
		vol.insert(t, volumeData);
	}

	void NesoraSynthesizeFormant::AddFricative(nsfloat t, nsfloat fricativeData) {
		fric.insert(t, fricativeData);
	}

	void NesoraSynthesizeFormant::AddNasal(nsfloat t, nsfloat nasalDatas) {
		nasal.insert(t, nasalDatas);
	}


	std::vector<nsfloat> NesoraSynthesizeFormant::MakeVoiceWave(const nsint& samplingFrequency) {
		const nsfloat samplingFrequency_inv = (nsfloat)1 / (nsfloat)samplingFrequency;
		const nsfloat maxFrequency = (nsfloat)samplingFrequency / (nsfloat)2;
		const size_t formantNum = formants.GetValue(0).buffer.size();
		const nsint time = (nsint)formants.GetMaxTime();
		std::vector<nsfloat> out(time);

		formants.StartSequentialOrder();
		freq.StartSequentialOrder();
		vol.StartSequentialOrder();
		fric.StartSequentialOrder();

		std::vector<NesoraBandpassFilter> bpfilter;
		bpfilter.resize(formantNum);
		for (auto& b : bpfilter)b.SetSamplingFreq(samplingFrequency);

		std::vector<double> omega(maxFrequency / 100);
		double num = 0;
		for (nsint i = 0;i < time;i++) {
			NesoraSynthesizeFormantData formant = formants.SequentialOrder();
			nsfloat v = vol.SequentialOrder();
			nsfloat fct = fric.SequentialOrder();
			nsfloat f = freq.SequentialOrder();

			if (v == 0)
				out[i] = 0;
			else {
				nsfloat fricativeOut = 0, voicedOut = 0;
				if (fct < 1) {
					for (nsfloat x = 0;x < maxFrequency / f and x < omega.size();x++) {
						omega[x] += std::modf(f * x * samplingFrequency_inv, &num);
						nsfloat fx = 0.0;
						for (nsuint j = 0;j < formant.buffer.size();j++) {
							const nsfloat x_minus_f_per_w = ((x * f) - formant.buffer[j].f) / formant.buffer[j].w;
							fx += formant.buffer[j].h * std::exp(-x_minus_f_per_w * x_minus_f_per_w);
						}
						voicedOut += std::pow(10, (nsfloat)9 * (fx - (nsfloat)1) * (nsfloat)0.5) * std::sin(PI2 * (nsfloat)omega[x]);
					}
				}

				if (fct > 0) {
					nsfloat whiteNoise = (nsfloat)rand() / (nsfloat)RAND_MAX;
					for (nsuint j = 0;j < formantNum;j++) {
						bpfilter[j].SetParameter(formant.buffer[j].f, formant.buffer[j].w / (formant.buffer[j].f * 4));
						fricativeOut += bpfilter[j].Filter(whiteNoise) * formant.buffer[j].h;
					}
				}
				out[i] = (voicedOut * (1 - fct) + fricativeOut * fct) * v;
			}
		}



		return out;
	}

	std::vector<nsfloat> NesoraSynthesizeFormant::MakePhonemeWave(const nsint& samplingFrequency) {
		const nsfloat samplingFrequency_inv = (nsfloat)1 / (nsfloat)samplingFrequency;
		const nsfloat maxFrequency = (nsfloat)samplingFrequency / (nsfloat)2;
		const size_t formantNum = formants.GetValue(0).buffer.size();
		const nsint time = (nsint)formants.GetMaxTime();
		std::vector<nsfloat> out(time);

		std::vector<NesoraBandpassFilter> bpfilter;
		bpfilter.resize(formantNum);
		for (auto& b : bpfilter)b.SetSamplingFreq(samplingFrequency);

		NesoraSynthesizeFormantData formant = formants.GetValue(0);
		nsfloat v = vol.GetValue(0);
		nsfloat fct = fric.GetValue(0);
		nsfloat f = freq.GetValue(0);

		std::vector<nsfloat> height;
		nsuint numx = 0;
		for (nsfloat i = 0;i < maxFrequency / f;i++) {
			nsfloat fx = 0.0;
			for (nsuint j = 0;j < formant.buffer.size();j++) {
				const nsfloat x_minus_f_per_w = ((i + 1) * f - formant.buffer[j].f) / formant.buffer[j].w;
				fx += formant.buffer[j].h * std::exp(-x_minus_f_per_w * x_minus_f_per_w);
			}
			height.push_back(std::pow(10, (nsfloat)9 * (fx - (nsfloat)1) * (nsfloat)0.5));
		}

		for (nsint i = 0;i < time;i++) {
			nsfloat fricativeOut = 0, voicedOut = 0;
			if (fct < 1) {
				nsuint numx = 0;
				for (nsfloat x = 0;x < maxFrequency / f;x++) {
					voicedOut += height[x] * std::sin((nsfloat)i * samplingFrequency_inv * PI2 * (x + 1) * f);
				}
			}

			if (fct > 0) {
				nsfloat whiteNoise = (nsfloat)rand() / (nsfloat)RAND_MAX;
				for (nsuint j = 0;j < formantNum / 2;j++) {
					bpfilter[j].SetParameter(formant.buffer[j].f, formant.buffer[j].w / (formant.buffer[j].f * 4));
					fricativeOut += bpfilter[j].Filter(whiteNoise) * formant.buffer[j].h;
				}

			}
			out[i] = (voicedOut * (1 - fct) + fricativeOut * fct) * v;
		}



		return out;
	}






	NesoraSynthesizeFormantData::NesoraSynthesizeFormantData()
		: buffer{}
	{
	}

	NesoraSynthesizeFormantData::NesoraSynthesizeFormantData(const NesoraSynthesizeFormantData& nssfd)
		: buffer(nssfd.buffer)
	{
	}

	NesoraSynthesizeFormantData& NesoraSynthesizeFormantData::operator=(const NesoraSynthesizeFormantData& nssfd) {
		if (this != &nssfd) {
			buffer = nssfd.buffer;
		}
		return *this;
	}

	NesoraVoice::NesoraVoice()
		: vowelFormantCube{}, maxFrequency(0), minFrequency(0)
	{
	}

	const NesoraSynthesizeFormantData NesoraSynthesizeFormantData::operator*(const nsfloat& t) const {
		NesoraSynthesizeFormantData out = *this;
		for (auto& b: out.buffer) {
			b.f *= t;
			b.h *= t;
			b.w *= t;
		}
		return out;
	}

	const NesoraSynthesizeFormantData NesoraSynthesizeFormantData::operator/(const nsfloat& t) const {
		nsfloat out = (nsfloat)1 / t;
		return *this * out;
	}

	const NesoraSynthesizeFormantData NesoraSynthesizeFormantData::operator+(const NesoraSynthesizeFormantData& t) const {
		NesoraSynthesizeFormantData out = *this;
		for (nsuint i = 0; i < t.buffer.size();i++) {
			out.buffer[i].f += t.buffer[i].f;
			out.buffer[i].h += t.buffer[i].h;
			out.buffer[i].w += t.buffer[i].w;
		}
		return out;
	}

	const NesoraSynthesizeFormantData NesoraSynthesizeFormantData::operator-(const NesoraSynthesizeFormantData& t) const {
		NesoraSynthesizeFormantData out = *this;
		for (nsuint i = 0; i < t.buffer.size();i++) {
			out.buffer[i].f -= t.buffer[i].f;
			out.buffer[i].h -= t.buffer[i].h;
			out.buffer[i].w -= t.buffer[i].w;
		}
		return out;
	}

	const bool NesoraSynthesizeFormantData::operator==(const NesoraSynthesizeFormantData& t) const {
		for (nsuint i = 0; i < t.buffer.size();i++) {
			if ((*this).buffer[i].f != t.buffer[i].f) return false;
			if ((*this).buffer[i].h != t.buffer[i].h) return false;
			if ((*this).buffer[i].w != t.buffer[i].w) return false;
		}
		return true;
	}

	const bool NesoraSynthesizeFormantData::operator!=(const NesoraSynthesizeFormantData& t) const {
		for (nsuint i = 0; i < t.buffer.size();i++) {
			if ((*this).buffer[i].f != t.buffer[i].f) return true;
			if ((*this).buffer[i].h != t.buffer[i].h) return true;
			if ((*this).buffer[i].w != t.buffer[i].w) return true;
		}
		return false;
	}






}//namespace nesora

