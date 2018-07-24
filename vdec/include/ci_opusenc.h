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

#ifndef _CI_OPUSENC_H_
#define _CI_OPUSENC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __RVDS__
// {4273C3CA-A89E-4a55-A8E0-152EBEEAB912}
CI_DEFINE_GUID(IID_OPUSENC_C, 
	0x4273c3ca, 0xa89e, 0x4a55, 0xa8, 0xe0, 0x15, 0x2e, 0xbe, 0xea, 0xb9, 0x12);
#endif //RVDS40

#define MAX_FRAME_SIZE 5760  //120ms at 48000Hz

enum
{
	CI_OPUSENC_PROPID_BUFFERINFO = 1,
	CI_OPUSENC_PROPID_CONFIGINFO = 2
};

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32MinInputBuffer;
	CI_U32 u32MinOutputBuffer;
	CI_U32 u32BufferFlag;
} CI_OPUSENC_BUFFERINFO;

typedef struct
{
	CI_U32 u32Size;                   // sizeof(CI_OPUSENC_CONFIGINFO)
	CI_U32 u32SampleRate;             // input sample rate (8000, 12000, 16000, 24000 or 48000)
	CI_U32 u32Channel;                // input channel (1 or 2)
	CI_U32 u32BitPerSample;           // input PCM bits per sample (always 16)
} CI_OPUSENC_CONFIGINFO;

CI_RESULT CI_OPUSENC_Create(
	OUT CI_VOID **ppEncoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_OPUSENC_CONFIGINFO *pConfig);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Create)(
	OUT CI_VOID **ppEncoder, 
	IN CONST CI_VOID *pLicense,
	IN CONST CI_OPUSENC_CONFIGINFO *pConfig);

CI_RESULT CI_OPUSENC_Destroy(
	IN OUT CI_VOID *pEncoder);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Destroy)(
	IN OUT CI_VOID *pEncoder);

CI_RESULT CI_OPUSENC_Get(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Get)(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData, 
	IN CI_U32 u32DataLen, 
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_OPUSENC_Set(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Set)(
	IN OUT CI_VOID *pEncoder, 
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData, 
	IN CI_U32 u32DataLen);

CI_RESULT CI_OPUSENC_Encode(
	IN OUT CI_VOID *pEncoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Encode)(
	IN OUT CI_VOID *pEncoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U8 *pu8Out,
	IN CI_U32 u32OutLen,
	OUT CI_U32 *pu32OutConsumed);

CI_RESULT CI_OPUSENC_Reset(
	IN OUT CI_VOID *pEncoder,
	IN CI_U32 u32Flags);

typedef CI_RESULT (* LPFN_CI_OPUSENC_Reset)(
	IN OUT CI_VOID *pEncoder,
	IN CI_U32 u32Flags);

typedef struct
{
	LPFN_CI_OPUSENC_Create Create;
	LPFN_CI_OPUSENC_Destroy Destroy;
	LPFN_CI_OPUSENC_Get Get;
	LPFN_CI_OPUSENC_Set Set;
	LPFN_CI_OPUSENC_Encode Encode;
	LPFN_CI_OPUSENC_Reset Reset;
} CI_OPUSENC_FUNCTIONS;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
