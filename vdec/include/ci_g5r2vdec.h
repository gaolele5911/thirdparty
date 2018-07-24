//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2009  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------
//
// This file is furnished as part of the Cidana Codec SDK. 
// Usage of this file, code, concepts, and/or algorithms is prohibited
// except under the terms of the Software Licensing Agreement with Cidana.
//

#ifndef _CI_G5R2VDEC_H_
#define _CI_G5R2VDEC_H_

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	CI_VIDEC_PROPID_HIDEVIDEO = 0xF0000001,

    // default 0
    // 1, find/collect one complete picture from input buffer and send which to hardware decoder
    //    While working with h264 raw data (nalu bitstreams), .
    // 0, send input buffer (data block) directly to hardware decoder
    //    While working with transfer protocols (carplay, Android Auto, etc.), the pack policy 
    //    will garantee one complete picture for each data block, send such kind of buffer directly
    //    to hardware decoder can save delay time between input bistream and output pictures.    
	CI_VIDEC_PROPID_COMPLETEPIC
};

typedef struct
{
	CI_VIDEC_OPENOPTIONS_COMMON
	CI_S32 s32Width;
	CI_S32 s32Height;
	CI_S32 s32DispWidth;
	CI_S32 s32DispHeight;
	CI_S32 s32OffsetX;
	CI_S32 s32OffsetY;
	CI_S32 s32ScreenWidth;
	CI_S32 s32ScreenHeight;
} CI_G5R2VDEC_OPENOPTIONS;

typedef struct  
{
	CI_VIDEC_DECODEOPTIONS_COMMON
} CI_G5R2VDEC_DECODEOPTIONS;

typedef struct  
{
	CI_U32 u32Size;
} CI_G5R2VDEC_GETFRAMEOPTIONS;

typedef struct
{
	CI_U32 u32Size;
	CI_U32 u32ShowVideo;
} CI_G5R2VDEC_RENDERFRAMEOPTIONS;

typedef struct
{	// Basic information
	CI_U32 u32Size;
	CI_U32 u32Cookie;			// unique signature used to release frame
	CI_U32 u32Left;
	CI_U32 u32Top;
	CI_U32 u32Right;
	CI_U32 u32Bottom;
	
	CI_U32 u32FrameCounter;		// used to indicate presence of dropped frames.
	CI_U32 u32FrameType;		// frame storage type
	CI_U32 u32ErrorStatus;		// error level
	// bits[24 - 31]: 0: SW, 1: HW
	// bits[16 - 23]: pixel format, 
	// 				SW: 0: YV12, 1:NV12, 2:YV16, 4:RGB565, 5:RGB32, 6:ARGB.
	// bits[8  - 15]: full range flag of output pixels, could be 0, 1.
	// bits[0  -  7]: the matrix coefficient index, the table could be get by calling XXX_Get interface.
	CI_U32 u32ChromaFormat;	
	CI_U32 u32FrameRate;
	CI_F64 f64BitRate;
	CI_DECODE_PTS Pts;			// Time Stamp of this frame

	CI_U8 *apu8Frame[3];			// pointer to each buffer
	CI_U32 au32Width[3];			// width
	CI_U32 au32Height[3];			// height
	CI_U32 au32Stride[3];			// stride of each buffer

	// TBD
	CI_U32 u32AspectRatioX;
	CI_U32 u32AspectRatioY;
	CI_U32 u32ProgressiveSequence;
	CI_U32 u32TopFieldFirst;
	CI_U32 u32ProgressiveFrame;

	// Codec special information
	CI_U32 u32POC;
	CI_U32 u32ViewID;
} CI_G5R2VDEC_FRAME;

typedef struct  
{
	CI_VIDEC_STREAMINFO_COMMON
	CI_U32 u32Profile;
	CI_U32 u32AspectRatioX;
	CI_U32 u32AspectRatioY;
} CI_G5R2VDEC_STREAMINFO;

typedef struct  
{
	CI_VIDEC_BUFFERINFO_COMMON
} CI_G5R2VDEC_BUFFERINFO;

CI_RESULT CI_G5R2VDEC_Create(
	OUT CI_VOID **ppDecoder,
	IN CONST CI_VOID *pLicense);

CI_RESULT CI_G5R2VDEC_Open(
	IN OUT CI_VOID *pDevice,
	IN OUT CI_VOID **ppDecoder, 
	IN CONST CI_G5R2VDEC_OPENOPTIONS *pOptions);

CI_RESULT CI_G5R2VDEC_Close(
	IN OUT CI_VOID *pDevice,
	IN OUT CI_VOID *pDecoder);

CI_U32 CI_G5R2VDEC_Release(
	IN OUT CI_VOID *pDevice);

CI_U32 CI_G5R2VDEC_AddRef(
	IN OUT CI_VOID *pDecoder);

CI_RESULT CI_G5R2VDEC_DecodeBuffer(
	IN OUT CI_VOID *pDecoder, 
	IN CONST CI_G5R2VDEC_DECODEOPTIONS *pOptions,
	IN CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	OUT CI_U32 *pu32InpConsumed,
	OUT CI_U32 *pu32FramesDecoded,
	OUT CI_U32 *pu32NumberOfSkippedFrames,
	OUT CI_U32 *pu32Status);

CI_RESULT CI_G5R2VDEC_Reset(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32Flags);

CI_RESULT CI_G5R2VDEC_GetFrame(
	IN CI_VOID *pDecoder,
	IN CONST CI_G5R2VDEC_GETFRAMEOPTIONS *pOptions,
	IN OUT CI_G5R2VDEC_FRAME *pFrame);

CI_U32 CI_G5R2VDEC_RenderFrame(
	IN CI_VOID *pDecoder, 
	IN CI_G5R2VDEC_RENDERFRAMEOPTIONS *pOptions,
	IN CI_G5R2VDEC_FRAME *pFrame);

CI_U32 CI_G5R2VDEC_ReleaseFrame(
	IN CI_VOID *pDecoder, 
	IN CI_G5R2VDEC_FRAME *pFrame);

CI_RESULT CI_G5R2VDEC_Get(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	OUT CI_VOID *pData,
	IN CI_U32 u32DataLen,
	OUT CI_U32 *pu32DataUsed);

CI_RESULT CI_G5R2VDEC_Set(
	IN OUT CI_VOID *pDecoder,
	IN CI_U32 u32PropId,
	IN CONST CI_VOID *pData,
	IN CI_U32 u32DataLen);

CI_RESULT CI_G5R2VDEC_ParseInfo(
	IN OUT CI_VOID *pDecoder,
	IN CONST CI_U8 *pu8Inp,
	IN CI_U32 u32InpLen,
	IN CI_U32 u32SearchLevel,
	IN CI_S16 *ps16Suffix,
	IN CI_U32 u32Flags,
	OUT CI_U32 *pdwConfidence,
	IN OUT CI_G5R2VDEC_STREAMINFO *pStreamInfo);

#ifdef __cplusplus
}
#endif
#endif // _CI_G5R2VDEC_H_
