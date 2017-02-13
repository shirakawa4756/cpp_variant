//===------------------ Instruction Macro definition ------------*- C++ -*-===//
//
// DLL Export, Import マクロの定義
//
// 本ファイルは，2条項BSDライセンスに従って配布されます．
// 詳細は、LICENSE.TXTを参照してください．
//
//===----------------------------------------------------------------------===//
///
/// \file
/// このクラスには，DLL Export, Import に関するマクロが定義されています
///
/// \author shirakawa4756@github.com
///
//===----------------------------------------------------------------------===//
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// DLL の生成に関するマクロ
// 
// VARIANT_API_EXPORTS が有効: DLL 生成
// VARIANT_API_IMPORTS が有効: DLL 取り込み
// どちらも無効              : そのまま
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (defined WIN32 || defined _WIN32 || defined _WIN64 || defined WINCE) && defined VARIANT_API_EXPORTS
#  define VARIANT_API __declspec(dllexport)
#  define VARIANT_EXPORTS_TEMPLATE __declspec(dllexport)
#elif (defined WIN32 || defined _WIN32 || defined _WIN64 || defined WINCE) && defined VARIANT_API_IMPORTS
#  define VARIANT_API __declspec(dllimport) 
#  define VARIANT_EXPORTS_TEMPLATE __declspec(dllimport) 
#else
#  define VARIANT_API
#  define VARIANT_EXPORTS_TEMPLATE 
#endif