/*
* Nesora 0 type IV
* Copyright (c) MucchoSP. All right reserved.
*
*
* 音諳用に作成した型定義プログラムです。
*
*/
#pragma once
#include <cstdint>


#ifndef USE_NESORA_TYPEDEF
namespace nesora {
#endif


//整数型
using nsint = int32_t;
using nsuint = uint32_t;

//浮動小数点数型
using nsfloat = double;

//標本化周波数型
using nssmpl_t = nsuint;

//周波数型
using nsfreq_t = nsfloat;



//円周率
constexpr nsfloat PI = (nsfloat)3.1415926535897932384626433832795028841971;
constexpr nsfloat PI_INV = (nsfloat)1 / PI;
constexpr nsfloat PI2 = PI * (nsfloat)2;
constexpr nsfloat PI2_INV = (nsfloat)1 / PI2;


#ifndef USE_NESORA_TYPEDEF
}//namespace Nesora
#endif

