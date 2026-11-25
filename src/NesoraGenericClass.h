/*
* Nesora 0 type IV
* Copyright (c) MucchoSP. All right reserved.
*
*
* 音諳用に作成した音声合成用プログラムです。
*
*/
#pragma once


#ifndef NESORA_GENERIC_CLASS
#define NESORA_GENERIC_CLASS

#include <list>
#include <vector>
#include <iostream>

namespace nesora {

	//重り付き平均
	template<class T>
	constexpr T WeightCalc(T v1, T v2, nsfloat weight) {
		return v1 + (v2 - v1) * weight;
	}



	//時間付き変数
	template<class T>
	struct NesoraTimeVariable {
		nsfloat time;
		T v;
	};

	// 音諳の時間付き線形配列
	// 添字演算子に時間を入れると、その時間の値をとれる。
	// 代入も添字演算子を使うことができる。
	// 値は前後の値から線形に計算する。
	// vector配列に出力することもできる。
	// 
	// このクラスに渡す型やクラスには、nsfloatとの乗算と、型同士の足し算、比較を用意する必要があります。
	//
	template<class T>
	class NesoraTimeArray {
	private:

		std::list<NesoraTimeVariable<T>> buffer;

		nsint index = 0;

		typename std::list<NesoraTimeVariable<T>>::iterator nextiterator;
		typename std::list<NesoraTimeVariable<T>>::iterator iterator;

	public:

		NesoraTimeArray() {

		}

		NesoraTimeArray(const NesoraTimeArray<T>& nta) {
			buffer = nta.buffer;
		}

		//参照
		const T operator[](nsfloat t) const& {
			return GetValue(t);
		}

		const T GetValue(nsfloat t) const& {
			if (buffer.size() == 1) {
				return buffer.back().v;
			}
			for (auto itr = buffer.begin();itr != buffer.end();itr++) {
				if ((*itr).time == t)
					return (*itr).v;
				else if ((*itr).time > t) {
					auto nextitr = itr--, backitr = itr;
					if ((*nextitr).v == (*backitr).v)
						return (*itr).v;
					else {
						nsfloat weight = (t - (*backitr).time) / ((*nextitr).time - (*backitr).time);
						return (*backitr).v * weight + (*nextitr).v * ((nsfloat)1 - weight);
					}
				}
			}
			return buffer.back().v;
		}

		//代入
		T& operator[](nsfloat t)& {
			//空判定
			if (buffer.empty()) {
				buffer.push_back({});
				buffer.back().time = t;
				return buffer.back().v;
			}

			//後ろに入れる場合
			if (buffer.back().time < t) {
				buffer.push_back({});
				buffer.back().time = t;
				return buffer.back().v;
			}
			else if (buffer.back().time == t) {
				return buffer.back().v;
			}

			//その他
			for (auto itr = buffer.begin();itr != buffer.end();itr++) {
				if ((*itr).time == t)
					return (*itr).v;
				else if ((*itr).time > t) {
					itr = buffer.insert(itr, {});
					(*itr).time = t;
					return (*itr).v;
				}
			}
			return buffer.back().v;
		}

		void insert(nsfloat t, T v) {
			for (auto itr = buffer.begin();itr != buffer.end();itr++) {
				if ((*itr).time == t)
					(*itr).v = v;
				else if ((*itr).time > t) {
					buffer.insert(itr, { t,v });
				}
			}
			buffer.push_back({ t,v });
			return;
		}

		void insert(const NesoraTimeVariable<T>& tv) {
			insert(tv.time, tv.v);
		}

		void insert(const std::list<NesoraTimeVariable<T>>& tva) {
			for (auto itr = tva.begin();itr != tva.end();itr++)
				insert(*itr);
		}

		void insert(const std::vector<NesoraTimeVariable<T>>& tva) {
			for (auto itr = tva.begin();itr != tva.end();itr++)
				insert(*itr);
		}

		void insert(const NesoraTimeArray<T>& ta) {
			insert(ta.GetBuffer());
		}




		void StartSequentialOrder() {
			if (buffer.size() == 1)
				index = -1;
			else {
				index = 0;
				nextiterator = buffer.begin();
				iterator = nextiterator++;
			}
		}

		T SequentialOrder() {
			if (index == -1)
				return buffer.back().v;
			if (index > buffer.back().time)
				return buffer.back().v;

			if ((*nextiterator).time == index) {
				iterator++;
				nextiterator++;
				index++;
				return (*iterator).v;
			}
			else if ((*nextiterator).time < index) {
				iterator++;
				nextiterator++;
			}
			if ((*iterator).v == (*nextiterator).v) {
				index++;
				return (*iterator).v;
			}
			nsfloat weight = ((nsfloat)index - (*iterator).time) / ((*nextiterator).time - (*iterator).time);
			index++;
			return (*iterator).v + ((*nextiterator).v - (*iterator).v) * weight;
		}

		void FinishSequentialOrder() {

		}

		std::vector<T> GetVector() {
			std::vector<nsfloat> out(buffer.back().time);
			auto nextitr = buffer.begin(), itr = nextitr++;
			for (nsuint i = 0;i < buffer.back().time;i++) {
				if ((*nextitr).time == i) {
					itr++;nextitr++;
					out[i] = (*itr).v;
					continue;
				}
				else if ((*nextitr).time < i) {
					itr++;nextitr++;
				}
				nsfloat weight = ((nsfloat)i - (*itr).time) / ((*nextitr).time - (*itr).time);
				out[i] = (*itr).v + ((*nextitr).v - (*itr).v) * weight;
				if (nextitr == buffer.end())break;
			}
			return out;
		}

		const nsfloat GetMaxTime() const {
			if (buffer.empty())
				return 0;
			else
				return buffer.back().time;
		}

		const std::list<NesoraTimeVariable<T>> GetBuffer() const {
			return buffer;
		}


		void AddTime(nsfloat time) {
			for (auto itr = buffer.begin();itr != buffer.end();itr++)
				(*itr).time += time;
			return;
		}

		std::list<NesoraTimeVariable<T>> GetAddedTimeList(nsfloat time) const {
			std::list<NesoraTimeVariable<T>> out = buffer;
			for (auto itr = out.begin();itr != out.end();itr++)
				(*itr).time += time;
			return out;
		}

		NesoraTimeArray<T> copy(nsfloat time = 0) const {
			NesoraTimeArray<T> out = *this;
			out.AddTime(time);
			return out;
		}

		void push_back(const NesoraTimeArray<T>& ta, nsfloat addtime = GetMaxTime()) {
			insert(ta.GetAddedTimeList(addtime));
		}
	};

	

	//バンドパスフィルター
	class NesoraBandpassFilter {
	private:

		// それぞれの変数は下記のとおりとする
		// float samplerate … サンプリング周波数
		// float freq … カットオフ周波数
		// float bw   … 帯域幅
		const nsfloat log2_half = log(2.0f) / 2.0;
		nsfloat samplerate_inv = (nsfloat)1 / (nsfloat)44100;
		nsfloat omega = 0.0f;
		nsfloat alpha = 0.0f;

		nsfloat a0 = 0.0f;
		nsfloat a1 = 0.0f;
		nsfloat a2 = 0.0f;
		nsfloat b0 = 0.0f;
		nsfloat b1 = 0.0f;
		nsfloat b2 = 0.0f;

		// フィルタ計算用のバッファ変数。
		nsfloat in1 = 0.0f;
		nsfloat in2 = 0.0f;
		nsfloat out1 = 0.0f;
		nsfloat out2 = 0.0f;

	public:

		NesoraBandpassFilter() {

		}

		void SetSamplingFreq(nsfloat smpl) {
			samplerate_inv = (nsfloat)1 / smpl;
		}

		void SetParameter(nsfloat freq, nsfloat bw) {
			omega = PI2 * freq * samplerate_inv;
			nsfloat sinomega = sin(omega);
			alpha = sinomega * sinh(log2_half * bw * omega / sinomega);

			a0 = 1.0f + alpha;
			a1 = -2.0f * cos(omega);
			a2 = 1.0f - alpha;
			b0 = alpha;
			b1 = 0.0f;
			b2 = -alpha;
		}

		nsfloat Filter(nsfloat input) {
			// 入力信号にフィルタを適用し、出力信号として書き出す。
			nsfloat output = (b0 * input + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2) / a0;

			in2 = in1;       // 2つ前の入力信号を更新
			in1 = input;  // 1つ前の入力信号を更新

			out2 = out1;      // 2つ前の出力信号を更新
			out1 = output; // 1つ前の出力信号を更新

			return output;
		}

	};




}//namespace nesora




#endif//NESORA_GENERIC_CLASS


