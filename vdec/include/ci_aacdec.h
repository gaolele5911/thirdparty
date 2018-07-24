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
//  Copyright (c) 2011  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _CI_AACDEC_H_
#define _CI_AACDEC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __RVDS__
// {FA7A4C10-8CC3-44CF-ADDF-8B55BB9943E3}
CI_DEFINE_GUID(IID_AACDEC,
	0xfa7a4c10, 0x8cc3, 0x44cf, 0xad, 0xdf, 0x8b, 0x55, 0xbb, 0x99, 0x43, 0xe3);

#endif //RVDS40

enum
{
	CI_AACDEC_PROPID_BUFFERINFO = CI_AUDEC_PROPID_BUFFERINFO,
	CI_AACDEC_PROPID_CONFIGINFO = CI_AUDEC_PROPID_CONFIGINFO,
	CI_AACDEC_PROPID_STREAMINFO = CI_AUDEC_PROPID_STREAMINFO,
	CI_AACDEC_PROPID_FILESUFFIXES = CI_AUDEC_PROPID_FILESUFFIXES,
	CI_AACDEC_PROPID_STATISTICS = CI_AUDEC_PROPID_STATISTICS+1,
};

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32MinInputBuffer;
	CI_U32 u32MinOutputBuffer;
} CI_AACDEC_BUFFERINFO;

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32MuxType;					//0, RAW; 1, LOAS; 2, ADTS
	CI_U32 u32Profile;
	CI_U32 u32Channel;
	CI_U32 u32SampleRate;
} CI_AACDEC_CONFIGINFO;

typedef struct
{
	CI_AUDEC_STREAMINFO_COMMON
	// decoder dependent
	CI_U32 u32SourceSampleRate;
} CI_AACDEC_STREAMINFO;

typedef struct
{
	CI_AUDEC_DECODESIDEINFO_COMMON
} CI_AACDEC_DECODESIDEINFO;

CI_RESULT CI_AACDEC_Create(
	OUT CI_VOID **ppDecoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_AACDEC_CONFIGINFO *pConfig,
	IN CONST CI_U8 *pu8Inp, 
	IN CI_U32 u32InpLen);

typedef CI_RESULT (* LPFN_CI_AACDEC_Create)(
	OUT CI_VOID **ppDecoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_AACDEC_CONFIGINFO *pConfig,
	IN CONST CI_U8 *pu8Inp, 
	IN CI_U32 u32InpLen);

CI_RESULT CI_AACDEC_Destroy(
	IN OUT CI_VOID *pDecoder);

typedef CI_RESULT (* LPFN_CI_AACDEC_Destroy)(
	IN OUT CI_VOID *pDecoder);

CI_RESULT CI_AACDEC_Get(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

typedef CI_RESULT (* LPFN_CI_AACDEC_Get)(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_AACDEC_Set(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

typedef CI_RESULT (* LPFN_CI_AACDEC_Set)(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

CI_RESULT CI_AACDEC_ParseInfo(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Confidence,
	IN OUT CI_AACDEC_STREAMINFO *pStreamInfo);

typedef CI_RESULT (* LPFN_CI_AACDEC_ParseInfo)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Confidence,
	IN OUT CI_AACDEC_STREAMINFO *pStreamInfo);

CI_RESULT CI_AACDEC_Decode(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_AACDEC_DECODESIDEINFO *pDecodeSideInfo,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed,
	IN CI_U32 u32Error,
	OUT CI_U32 *pu32Status);

typedef CI_RESULT (* LPFN_CI_AACDEC_Decode)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_AACDEC_DECODESIDEINFO *pDecodeSideInfo,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed,
	IN CI_U32 u32Error,
	OUT CI_U32 *pu32Status);

CI_RESULT CI_AACDEC_Reset(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

typedef CI_RESULT (* LPFN_CI_AACDEC_Reset)(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

typedef struct
{
	LPFN_CI_AACDEC_Create Create;
	LPFN_CI_AACDEC_Destroy Destroy;
	LPFN_CI_AACDEC_Get Get;
	LPFN_CI_AACDEC_Set Set;
	LPFN_CI_AACDEC_ParseInfo ParseInfo;
	LPFN_CI_AACDEC_Decode Decode;
	LPFN_CI_AACDEC_Reset Reset;
	LPFN_CI_AUDEC_SetPts SetPts;
	LPFN_CI_AUDEC_GetPts GetPts;
} CI_AACDEC_FUNCTIONS;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
