#include "NesoraFuru.h"

namespace NesoraFuru {


	// MARK: NesoraFormants

	std::vector<SinWaveParameter> NesoraFormants::Calculate(double pitch, int outSize) {
		std::vector<SinWaveParameter> out;

		for (int i = 1;i <= outSize; i++) {
			out.push_back({ (double)i * pitch, 0.0 });
		}

		// ガウス分布を使ったフォルマント計算機
		for (auto& formant : formants) {
			for (auto& i : out) {
				i.amplitude += formant.intensity * std::exp(-pow2((i.frequency - formant.frequency) / formant.width));
			}
		}
		for (auto& i : out) {
			i.amplitude = std::pow(10.0, 4.5 * (i.amplitude - 1.0));
		}

		return out;
	}




	// MARK: NesoraSynthesizer

	NesoraSynthesizer::NesoraSynthesizer() {
		phase_buffer.resize(number_of_multiples);
		for (int i = 0;i < number_of_multiples;i++) {
			phase_buffer[i] = ((double)rand() / (double)RAND_MAX) * 2.0 * PI;
		}
	}

	std::vector<double> NesoraSynthesizer::Synthesize(std::vector<double> pitch, std::vector<double> amplitude, std::vector<NesoraFormants> formants, std::vector<double> fricative) {
		std::vector<double> out;

		size_t size = std::min({ pitch.size(), amplitude.size(), formants.size() });
		out.resize(size);

		for (size_t i = 0;i < size;i++) {

			std::vector<SinWaveParameter> multiples = formants[i].Calculate(pitch[i], number_of_multiples);

			for (size_t j = 0;j < number_of_multiples;j++) {
				phase_buffer[j] += (multiples[j].frequency / samplingFrequency) * 2.0 * PI;
				out[i] += std::sin(phase_buffer[j]) * multiples[j].amplitude;
			}

			out[i] = out[i] * amplitude[i] * (1.0 - fricative[i]) + ((double)rand() / (double)RAND_MAX) * fricative[i];
		}

		return out;
	}




	// MARK: NesoraFuru

	std::vector<double> NesoraFuru::Synthesize(std::string script) {
		std::vector<double> pitch, amplitude, fricative;
		std::vector<NesoraFormants> formants;

		auto r = analyzer.Analyze(script, 1.0);

		int len = r.f0.size() * r.frame_size;
		for (size_t i = 0; i < r.f0.size(); i++) {
			for (size_t j = 0;j < r.frame_size;j++)
				pitch.push_back(r.f0[i]);
		}
		std::cout << pitch.size() << "," << len << std::endl;
		amplitude.resize(len, 0.2);
		fricative.resize(len, 0.0);

		formants = voice.GenerateFromants(r.phones, r.frame_size);

		return synthesizer.Synthesize(pitch, amplitude, formants, fricative);
	}

} // namespace NesoraFuru
