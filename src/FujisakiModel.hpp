// 藤崎モデルの実装
// 参考:https://www.gavo.t.u-tokyo.ac.jp/tokutei_pub/houkoku/model/fujisaki.pdf
// この論文の式8を実装しただけ

#include <cmath>
#include <vector>


#ifndef NESORA_FUJISAKI_MODEL
#define NESORA_FUJISAKI_MODEL

namespace NesoraFujisakiModel {

	// フレーズ指定
	struct ApParameter {
		double size;	// インパルスの大きさ
		size_t start;	// 生起時点(秒)
	};

	// アクセント指令
	struct AaParameter {
		double amplitude;	// ステップの振幅
		size_t start;		// 始点(秒)
		size_t end;			// 終点(秒)
	};

	// 藤崎モデル
	class FujisakiModel {
	public:

		double alpha;	// フレーズ制御機構の固有角周波数
		double beta;	// アクセント制御機構の固有角周波数
		double ganma;	// アクセント成分の相対飽和レベル

		// フレーズ制御機構のインパルス応答
		inline double Gp(double t) const {
			if (t < 0) return 0;
			else return alpha * alpha * t * std::exp(-alpha * t);
		}
		// アクセント制御機構のステップ応答
		inline double Ga(double t) const  {
			if (t < 0) return 0;
			else return std::min(1.0 - (1.0 + beta * t) * std::exp(-beta * t), ganma);
		}

		// 生成する
		std::vector<double> CalculateF0(std::vector<double>Fb, std::vector<ApParameter> Ap, std::vector<AaParameter> Aa, double samplingFrequency) const {
			std::vector<double> out;
			out.resize(Fb.size());

			for (size_t idx = 0;idx < Fb.size();idx++) {
				double t = (double)idx / samplingFrequency;
				double Ap_section = 0.0;
				double Aa_section = 0.0;
				for (size_t i = 0;i < Ap.size();i++) {
					Ap_section += Ap[i].size * Gp(t - Ap[i].start);
				}
				for (size_t j = 0;j < Aa.size();j++) {
					Aa_section += Aa[j].amplitude * (Ga(t - Aa[j].start) - Ga(t - Aa[j].end));
				}

				out[idx] = std::exp(std::log(Fb[idx]) + Ap_section + Aa_section);
			}
		}
	};
}


#endif //NESORA_FUJISAKI_MODEL
