#include "NesoraFuru.h"

namespace NesoraFuru {


	// MARK: NesoraFormants

	std::vector<SinWaveParameter> NesoraFormants::Calculate(double pitch, int outSize) {
		std::vector<SinWaveParameter> out;

		for (int i = 1;i <= outSize; i++) {
			out.push_back({ (double)i * pitch, 0.0 });
		}

		// ガウス分布を使ったフォルマント計算機
		for (auto& i : out) {
			for (auto& formant : formants) {
				i.amplitude += std::pow(10.0, formant.intensity * std::exp(-pow2((i.frequency - formant.frequency) / formant.width)) / 10.0);
			}
		}
		for (auto& i : out) {
			i.amplitude = (i.amplitude / (double)formants.size() - 1.0) * 8.0;
		}

		for (auto& formant : formants) {
			std::cout << "  (" << formant.frequency << ", " << formant.intensity << ", " << formant.width << ")" << std::endl;
			for (auto& i : out) {
				std::cout << "(" << i.frequency << ", " << i.amplitude << "), ";
			}
			std::cout << std::endl;
		}

		return out;
	}




	// MARK: NesoraSynthesizer

	NesoraSynthesizer::NesoraSynthesizer() {
		phase_buffer.resize(number_of_multiples);
	}

	std::vector<double> NesoraSynthesizer::Synthesize(std::vector<double> pitch, std::vector<double> amplitude, std::vector<NesoraFormants> formants, std::vector<double> fricative) {
		std::vector<double> out;

		size_t size = std::min({ pitch.size(), amplitude.size(), formants.size() });
		out.resize(size);

		for (size_t i = 0;i < size;i++) {

			std::vector<SinWaveParameter> multiples = formants[i].Calculate(pitch[i], number_of_multiples);

			for (size_t j = 0;j < number_of_multiples;j++) {
				phase_buffer[j] += (samplingFrequency / multiples[j].frequency) * PI;
				out[i] += std::sin(phase_buffer[j]) * multiples[j].amplitude;
			}

			out[i] = out[i] * amplitude[i] * (1.0 - fricative[i]) + ((double)rand() / (double)RAND_MAX) * fricative[i];
		}

		return out;
	}




	std::vector<double> NesoraFuru::Synthesize(std::string script) {
		std::vector<double> pitch, amplitude, fricative;
		std::vector<NesoraFormants> formants;
		pitch.resize(1, voice.GetData().f0);
		amplitude.resize(48000, 1.0);
		fricative.resize(48000, 0.0);
		formants.resize(48000, voice.GetData().formants);
		return synthesizer.Synthesize(pitch, amplitude, formants, fricative);
	}

} // namespace NesoraFuru
