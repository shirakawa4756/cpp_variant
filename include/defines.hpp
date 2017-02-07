///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2016 Kanazawa Institute of Technology. All Rights Reserved.
//
// 不良種類関連の定義
//
// 不良名，不良カテゴリ，不良状態など不良に関する状態保持や名前などの
// 定義を行っています．
//
//===-----------------------------------------------------------------------------------------===//
///
/// @file types.hpp
/// このファイルは，不良関連の変数やクラス(DefectiveState)の定義が記述されています．
///
/// @author Akihiro KAMEMOTO
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// DLL の生成に関するマクロ
// 
// BELLOWS_EXPORTSが有効: DLL 生成
// BELLOWS_IMPORTSが有効: DLL 取り込み
// どちらも無効         : そのまま
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