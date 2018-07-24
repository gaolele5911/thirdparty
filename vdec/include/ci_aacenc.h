//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
// THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
//  ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
// ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
// COMPANY.
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 2016  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _CI_AACENC_H_
#define _CI_AACENC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __RVDS__
// {E33E3539-415C-4EDC-8A0E-B3C11B5020A3}
CI_DEFINE_GUID(IID_AACENC, 
	0xe33e3539, 0x415c, 0x4edc, 0x8a, 0xe, 0xb3, 0xc1, 0x1b, 0x50, 0x20, 0xa3);
#endif //RVDS40

enum
{
	CI_AACENC_PROPID_BUFFERINFO = 1,
	CI_AACENC_PROPID_CONFIGINFO = 2
};

enum
{
	CI_AACENC_LC = 1,
	CI_AACENC_ELD
};

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32MinInputBuffer;
	CI_U32 u32MinOutputBuffer;
	CI_U32 u32BufferFlag;
} CI_AACENC_BUFFERINFO;

typedef struct
{
	CI_U32 u32Size;                   // sizeof(CI_AACENC_CONFIGINFO)
	CI_U32 u32Profile;				  // CI_AACENC_LC CI_AACENC_ELD
	CI_U32 u32SampleRate;             // input sample rate
	CI_U32 u32Channel;                // input channel number
	CI_U32 u32BitsPerSample;           // input PCM bits per sample 
	CI_U32 u32BitRate;
	CI_U32 u32MuxType;					//0, RAW; 1, LOAS; 2, ADTS
	CI_U8 *pu8RawConf;
	CI_U32 u32RawConfLen;
} CI_AACENC_CONFIGINFO;

CI_RESULT CI_AACENC_Create(
	OUT CI_VOID **ppEncoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_AACENC_CONFIGINFO *pConfig);

typedef CI_RESULT (* LPFN_CI_AACENC_Create)(
	OUT CI_VOID **ppEncoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_AACENC_CONFIGINFO *pConfig);

CI_RESULT CI_AACENC_Destroy(
	IN OUT CI_VOID *pEncoder);

typedef CI_RESULT (* LPFN_CI_AACENC_Destroy)(
	IN OUT CI_VOID *pEncoder);

CI_RESULT CI_AACENC_Get(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

typedef CI_RESULT (* LPFN_CI_AACENC_Get)(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_AACENC_Set(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

typedef CI_RESULT (* LPFN_CI_AACENC_Set)(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

CI_RESULT CI_AACENC_Encode(
	IN OUT CI_VOID *pEncoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed);

typedef CI_RESULT (* LPFN_CI_AACENC_Encode)(
	IN OUT CI_VOID *pEncoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed);

CI_RESULT CI_AACENC_Reset(
	IN OUT CI_VOID *pEncoder,
	IN CI_U32 u32Flags);

typedef CI_RESULT (* LPFN_CI_AACENC_Reset)(
	IN OUT CI_VOID *pEncoder,
	IN CI_U32 u32Flags);

typedef struct
{
	LPFN_CI_AACENC_Create Create;
	LPFN_CI_AACENC_Destroy Destroy;
	LPFN_CI_AACENC_Get Get;
	LPFN_CI_AACENC_Set Set;
	LPFN_CI_AACENC_Encode Encode;
	LPFN_CI_AACENC_Reset Reset;
} CI_AACENC_FUNCTIONS;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
