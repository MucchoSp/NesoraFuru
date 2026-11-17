// Im going to create a new Nesora Furu!!!
//

#include "NesoraFuru.h"

using namespace NesoraFuru;


// MARK: NesoraFormants

std::vector<SinWaveParameter> NesoraFormants::Calculate(double pitch, int outSize) {
	std::vector<SinWaveParameter> out;

	for (int i = 0;i < outSize; i++) {
		out.push_back({(double)i * pitch, 0.0});
	}

	// ガウス分布を使ったフォルマント計算機
	for (auto& formant : formants) {
		for (auto& i : out) {
			i.amplitude += std::exp(-((i.frequency - formant.frequency) * (i.frequency - formant.frequency)) / (2 * formant.width * formant.width)) * formant.intensity;
		}
	}

	return out;
}




// MARK: NesoraSynthesizer

NesoraSynthesizer::NesoraSynthesizer() {
	phase_buffer.resize(number_of_multiples);
}

std::vector<double> NesoraSynthesizer::Synthesize(std::vector<double> pitch, std::vector<double> amplitude, std::vector<NesoraFormants> formants) {
	std::vector<double> out;

	size_t size = std::min({ pitch.size(), amplitude.size(), formants.size() });
	out.resize(size);

	for (size_t i = 0;i < size;i++) {

		std::vector<SinWaveParameter> multiples = formants[i].Calculate(pitch[i], number_of_multiples);

		for (size_t j = 0;j < number_of_multiples;j++) {
			phase_buffer[j] += samplingFrequency / multiples[j].frequency;
			out[i] += std::sin(phase_buffer[j]) * multiples[j].amplitude;
		}

		out[i] *= amplitude[i];
	}

	return out;
}
