///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2016 Kanazawa Institute of Technology. All Rights Reserved.
//
// �s�ǎ�ފ֘A�̒�`
//
// �s�ǖ��C�s�ǃJ�e�S���C�s�Ǐ�ԂȂǕs�ǂɊւ����ԕێ��▼�O�Ȃǂ�
// ��`���s���Ă��܂��D
//
//===-----------------------------------------------------------------------------------------===//
///
/// @file types.hpp
/// ���̃t�@�C���́C�s�Ǌ֘A�̕ϐ���N���X(DefectiveState)�̒�`���L�q����Ă��܂��D
///
/// @author Akihiro KAMEMOTO
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// DLL �̐����Ɋւ���}�N��
// 
// BELLOWS_EXPORTS���L��: DLL ����
// BELLOWS_IMPORTS���L��: DLL ��荞��
// �ǂ��������         : ���̂܂�
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