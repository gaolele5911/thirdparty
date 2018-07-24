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

#ifndef _CI_OPUSDEC_H_
#define _CI_OPUSDEC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __RVDS__
// {E08264CB-98BF-4a7c-AFFB-7829644C734B}
CI_DEFINE_GUID(IID_OPUSDEC_C, 
	0xe08264cb, 0x98bf, 0x4a7c, 0xaf, 0xfb, 0x78, 0x29, 0x64, 0x4c, 0x73, 0x4b);

#endif //RVDS40

#define MAX_FRAME_SIZE 5760  //120ms at 48000Hz

enum
{
	CI_OPUSDEC_PROPID_BUFFERINFO = CI_AUDEC_PROPID_BUFFERINFO,
	CI_OPUSDEC_PROPID_CONFIGINFO = CI_AUDEC_PROPID_CONFIGINFO,
	CI_OPUSDEC_PROPID_STREAMINFO = CI_AUDEC_PROPID_STREAMINFO,
	CI_OPUSDEC_PROPID_FILESUFFIXES = CI_AUDEC_PROPID_FILESUFFIXES,
	CI_OPUSDEC_PROPID_STATISTICS = CI_AUDEC_PROPID_STATISTICS+1,
};

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32MinInputBuffer;
	CI_U32 u32MinOutputBuffer;
	CI_U32 u32MinInputDataForOpen;
	CI_U32 u32BufferFlag;
} CI_OPUSDEC_BUFFERINFO;

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32SourceStreams;
	CI_U32 u32SourceCoupled;
	CI_U8  u8SourceStreamMap[255];
} CI_OPUSDEC_MULTISTREAMINFO;                // for multi streams

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32OutputSampleRate;                   // output sample rate (perfer 48000), may diff with source by internal resample.
	CI_U32 u32SourceChannel;                      // source channel (1 or 2), without upmix/downmix.
	CI_OPUSDEC_MULTISTREAMINFO *pMultiStreamInfo; // set to NULL for single stream (1 or 2 ch)
} CI_OPUSDEC_CONFIGINFO;

typedef struct
{
	CI_AUDEC_STREAMINFO_COMMON
	// decoder dependent
	CI_U32 u32SourceSampleRate;
} CI_OPUSDEC_STREAMINFO;

typedef struct
{
	CI_AUDEC_DECODESIDEINFO_COMMON
} CI_OPUSDEC_DECODESIDEINFO;

CI_RESULT CI_OPUSDEC_Create(
	OUT CI_VOID **ppDecoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_OPUSDEC_CONFIGINFO *pConfig,
	IN CONST CI_U8 *pu8Inp, 
	IN CI_U32 u32InpLen);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Create)(
	OUT CI_VOID **ppDecoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_OPUSDEC_CONFIGINFO *pConfig,
	IN CONST CI_U8 *pu8Inp, 
	IN CI_U32 u32InpLen);

CI_RESULT CI_OPUSDEC_Destroy(
	IN OUT CI_VOID *pDecoder);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Destroy)(
	IN OUT CI_VOID *pDecoder);

CI_RESULT CI_OPUSDEC_Get(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Get)(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_OPUSDEC_Set(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Set)(
	IN OUT CI_VOID *pDecoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

CI_RESULT CI_OPUSDEC_ParseInfo(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Confidence,
	IN OUT CI_OPUSDEC_STREAMINFO *pStreamInfo);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_ParseInfo)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pu32Confidence,
	IN OUT CI_OPUSDEC_STREAMINFO *pStreamInfo);

CI_RESULT CI_OPUSDEC_Decode(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_OPUSDEC_DECODESIDEINFO *pDecodeSideInfo,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed,
	IN CI_U32 u32Error,
	OUT CI_U32 *pu32Status);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Decode)(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_OPUSDEC_DECODESIDEINFO *pDecodeSideInfo,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed,
	IN CI_U32 u32Error,
	OUT CI_U32 *pu32Status);

CI_RESULT CI_OPUSDEC_Reset(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

typedef CI_RESULT (* LPFN_CI_OPUSDEC_Reset)(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

typedef struct
{
	LPFN_CI_OPUSDEC_Create Create;
	LPFN_CI_OPUSDEC_Destroy Destroy;
	LPFN_CI_OPUSDEC_Get Get;
	LPFN_CI_OPUSDEC_Set Set;
	LPFN_CI_OPUSDEC_ParseInfo ParseInfo;
	LPFN_CI_OPUSDEC_Decode Decode;
	LPFN_CI_OPUSDEC_Reset Reset;
	LPFN_CI_AUDEC_SetPts SetPts;
	LPFN_CI_AUDEC_GetPts GetPts;
} CI_OPUSDEC_FUNCTIONS;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
