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
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef __CI_G5R2VENC_H__
#define __CI_G5R2VENC_H__

enum 
{
	CI_DVR_ERROR_NONE = 0,
	CI_DVR_ERROR_NOTSUPPORT,
	CI_DVR_ERROR_INVALIDARG,
	CI_DVR_ERROR_OUTOFMEMORY,
	CI_DVR_ERROR_VPE,
	CI_DVR_ERROR_ENCODER,
	CI_DVR_ERROR_UNKNOWN,
};

enum 
{
	CI_DVR_IMG_FORMAT_RGB24 = 1,
	CI_DVR_IMG_FORMAT_BGR24,
	CI_DVR_IMG_FORMAT_ARGB32,
	CI_DVR_IMG_FORMAT_ABGR32,
	CI_DVR_IMG_FORMAT_RGB565,
	CI_DVR_IMG_FORMAT_YUV444,
	CI_DVR_IMG_FORMAT_YUYV,
	CI_DVR_IMG_FORMAT_YVYU,
	CI_DVR_IMG_FORMAT_UYVY,
	CI_DVR_IMG_FORMAT_VYUY,
	CI_DVR_IMG_FORMAT_NV16,
	CI_DVR_IMG_FORMAT_NV61,
	CI_DVR_IMG_FORMAT_NV12,
	CI_DVR_IMG_FORMAT_NV21,
};

enum
{
	CI_DVR_ENC_CODECTYPE_H264 = 0,
	CI_DVR_ENC_CODECTYPE_MPEG4,
	CI_DVR_ENC_CODECTYPE_H263,
};

enum {
	CI_DVR_H264ENC_LEVEL_10 = 10, //  Level 1.0  
	CI_DVR_H264ENC_LEVEL_1b =  9, //  Level 1.b  
	CI_DVR_H264ENC_LEVEL_11 = 11, //  Level 1.1  
	CI_DVR_H264ENC_LEVEL_12 = 12, //  Level 1.2  
	CI_DVR_H264ENC_LEVEL_13 = 13, //  Level 1.3  
	CI_DVR_H264ENC_LEVEL_20 = 20, //  Level 2.0  
	CI_DVR_H264ENC_LEVEL_21 = 21, //  Level 2.1  
	CI_DVR_H264ENC_LEVEL_22 = 22, //  Level 2.2  
	CI_DVR_H264ENC_LEVEL_30 = 30, //  Level 3.0  
	CI_DVR_H264ENC_LEVEL_31 = 31, //  Level 3.1  
	CI_DVR_H264ENC_LEVEL_32 = 32, //  Level 3.2  
	CI_DVR_H264ENC_LEVEL_40 = 40, //  Level 4.0  
	CI_DVR_H264ENC_LEVEL_41 = 41, //  Level 4.1  
	CI_DVR_H264ENC_LEVEL_42 = 42, //  Level 4.2  
	CI_DVR_H264ENC_LEVEL_50 = 50, //  Level 5.0  
	CI_DVR_H264ENC_LEVEL_51 = 51 //  Level 5.1  

};

enum {
	CI_DVR_H264ENC_BASELINE_PROFILE = 66,                // BaseLine Profile   
	CI_DVR_H264ENC_MAIN_PROFILE     = 77,                // Main Profile       
	CI_DVR_H264ENC_EXTENDED_PROFILE = 88,                // Extended Profile   
	CI_DVR_H264ENC_HIGH_PROFILE     = 100,               // High Profile       
	CI_DVR_H264ENC_DEFAULT_PROFILE  = CI_DVR_H264ENC_HIGH_PROFILE, // Default Profile    
	CI_DVR_H264ENC_HIGH10_PROFILE   = 110,               // High 10 Profile    
	CI_DVR_H264ENC_HIGH422_PROFILE  = 122,               // High 4:2:2 Profile 
	CI_DVR_H264ENC_SVC_BASELINE_PROFILE = 83,             // SVC Baseline Profile 
	CI_DVR_H264ENC_SVC_HIGH_PROFILE     = 86              // SVC High Profile 
};

enum {
	CI_DVR_MPEG4ENC_SP_LEVEL_0   = 0,  // MPEG4 Simple Profile Level 0 
	CI_DVR_MPEG4ENC_SP_LEVEL_0B  = 9,  // MPEG4 Simple Profile Level 0b
	CI_DVR_MPEG4ENC_SP_LEVEL_1   = 1,  // MPEG4 Simple Profile Level 1 
	CI_DVR_MPEG4ENC_SP_LEVEL_2   = 2,  // MPEG4 Simple Profile Level 2 
	CI_DVR_MPEG4ENC_SP_LEVEL_3   = 3,  // MPEG4 Simple Profile Level 3 
	CI_DVR_MPEG4ENC_SP_LEVEL_4A  = 4,  // MPEG4 Simple Profile Level 4a
	CI_DVR_MPEG4ENC_SP_LEVEL_5   = 5,  // MPEG4 Simple Profile Level 5 
	CI_DVR_MPEG4ENC_SP_LEVEL_6   = 6   // MPEG4 Simple Profile Level 6 
};

enum {
	CI_DVR_H263ENC_BL__LEVEL_10 = 10,   // H263 Baseline Profile Level 10 
	CI_DVR_H263ENC_BL__LEVEL_20 = 20,   // H263 Baseline Profile Level 20 
	CI_DVR_H263ENC_BL__LEVEL_30 = 30,   // H263 Baseline Profile Level 30 
	CI_DVR_H263ENC_BL__LEVEL_40 = 40,   // H263 Baseline Profile Level 40 
	CI_DVR_H263ENC_BL__LEVEL_45 = 45,   // H263 Baseline Profile Level 45 
	CI_DVR_H263ENC_BL__LEVEL_50 = 50,   // H263 Baseline Profile Level 50 
	CI_DVR_H263ENC_BL__LEVEL_60 = 60,   // H263 Baseline Profile Level 60 
	CI_DVR_H263ENC_BL__LEVEL_70 = 70    // H263 Baseline Profile Level 70 
};

enum {
	CI_DVR_MPEG4ENC_SIMPLE_PROFILE = 3,
};

enum {
	CI_DVR_H263ENC_BASELINE_PROFILE = 3,
};

typedef struct
{
	unsigned char valid;
	unsigned int scale;
	unsigned long long ts;
} CI_DVR_PTS;

typedef void (* PFN_WRITEOUTPUT)(void *pContext, unsigned char* pSrc, int srcBytes, CI_DVR_PTS *pPts, CI_DVR_PTS *pDts, int isKeyFrame);

typedef struct
{
	int size;
	
	// input source control
	int srcFormat;
	int srcWidth;
	int srcHeight;
	struct {
		int top;
		int bottom;
		int left;
		int right;
	}srcRect;
	int srcInterlace;	// input image interlace mode, 0: progressive, 1:V4L2_FIELD_ALTERNATE, 2:V4L2_FIELD_SEQ_TB(reserved)

	// encode control
	int encCodecType;
	int encProfile; 
	int encLevel;
	int encWidth;
	int encHeight;
	int encFrameRate;
	int encBitRate;
	//int encBfferType;

	// process control
	//int transLength;

	// write_output callback
	void* pContext;
	PFN_WRITEOUTPUT write_output;
	
	int omap_drm_fd;
	void* omap_drm_dev;

} CI_DVR_OPENOPTIONS;

typedef struct 
{
	int idx;
	unsigned char *pBuf;
	int buf_size;
	int width;
	int height;	
	unsigned int bo;
	unsigned int bo_fd;
	unsigned char *pBuf_uv;
	unsigned int bo_uv;
	unsigned int bo_uv_fd;
}CI_DVR_VPEBUF;

typedef struct
{
	int number;
	void *dev;
	int fd;	
}CI_DVR_VPEProperties;

typedef void* CI_DVR_HANDLE;
#ifdef __cplusplus
extern "C" {
#endif
int CiDVR_Create(CI_DVR_HANDLE* pCiDVR);
int CiDVR_Release(CI_DVR_HANDLE CiDVR);
int CiDVR_Open(CI_DVR_HANDLE CiDVR, CI_DVR_OPENOPTIONS* pCiDVROpenOptionss);
int CiDVR_Close(CI_DVR_HANDLE CiDVR);
int CiDVR_Process(CI_DVR_HANDLE CiDVR, unsigned char*pInBuf[2], int bufSize[2], CI_DVR_PTS *bufPts, int frameNum, int *frameEncoded, int eos);
int CiDVR_Process_idx(CI_DVR_HANDLE CiDVR, int idx, CI_DVR_PTS *inPts, int frameNum, int *frameEncoded, int eos);
int CiDVR_GetVPEInBufs(CI_DVR_HANDLE CiDVR, CI_DVR_VPEBUF *pVPEBufs, CI_DVR_VPEProperties *pVPEProp);
int CiDVR_ReleaseVPEInBufs(CI_DVR_VPEBUF *pVPEBufs, CI_DVR_VPEProperties *pVPEProp);
int CiDVR_SetVPEInBufs(CI_DVR_HANDLE CiDVR, CI_DVR_VPEBUF *pVPEBufs, CI_DVR_VPEProperties *pVPEProp);

int CiDVR_Process_enc_idx(CI_DVR_HANDLE CiDVR, int idx, CI_DVR_PTS *inPts, int frameNum, int *frameEncoded, int eos);
int CiDVR_GetEncInBufs(CI_DVR_HANDLE CiDVR, CI_DVR_VPEBUF *pVPEBufs, CI_DVR_VPEProperties *pVPEProp);
int CiDVR_SetEncInBufs(CI_DVR_HANDLE CiDVR, CI_DVR_VPEBUF *pVPEBufs, CI_DVR_VPEProperties *pVPEProp);
#ifdef __cplusplus
}
#endif
#endif //ifndef __CI_DVR_H__

