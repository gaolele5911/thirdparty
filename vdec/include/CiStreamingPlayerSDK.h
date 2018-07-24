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
// 	Copyright (c) 2013  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------
//
// This file is furnished as part of the Cidana Playback SDK. 
// Usage of this file, code, concepts, and/or algorithms is prohibited
// except under the terms of the Software Licensing Agreement with Cidana.
//

/**
* @file CiSimplePlaybackSDK.h
*
* @author Cidana, Inc.
* @version 0.1
*
*/

/**
 * @defgroup Changelog Changelog
 *
 * @section API_Change_0_1
 */

#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef USE_DLL
    #ifdef __GNUC__
      #define CI_API __attribute__ ((dllexport))
    #else
      #define CI_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define CI_API __attribute__ ((dllimport))
    #else
      #define CI_API __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define CI_API __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define CI_API
    #define DLL_LOCAL
  #endif
#endif


typedef long            ci32s;
typedef char            ci8s;
typedef unsigned char   ci8u;
typedef unsigned short  ci16u;
typedef unsigned long   ci32u;
typedef long long       ci64s;
typedef float           ci32f;

typedef void            ciVoid;
typedef void*           ciHandle;
typedef ci32s           ciResult;

#if defined (UNICODE) | defined (_UNICODE)
#include <tchar.h>
#else
typedef char TCHAR;
#endif


// error code used by Cidana API
#define CI_ERR_NONE                  0           // no error
#define CI_ERR_FAIL                  0x80000001  // failed
#define CI_ERR_INVALID_PARAM         0x80000002  // invalid parameter
#define CI_ERR_NOT_INITED            0x80000003  // sdk not initialized
#define CI_ERR_TIMEBOMB_EXPIRED      0x80000004  // time bomb expired
#define CI_ERR_DEMUX_FAIL            0x80000005  // demux opration failed
#define CI_ERR_AUDIO_DECODER_FAIL    0x80000006  // audio decoder opration failed
#define CI_ERR_VIDEO_DECODER_FAIL    0x80000007  // video decoder opration failed
#define CI_ERR_AUDIO_RENDER_FAIL     0x80000008  // audio render opration failed
#define CI_ERR_VIDEO_RENDER_FAIL     0x80000009  // video render opration failed
#define CI_ERR_VIDEO_RESOLUTION_NOT_SUPPORTED	0x8000000A //video resolution not supported
#define CI_ERR_LICENSE_FAIL          0x8000000B //License check failed

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum _tagCI_PLAYER_EVENT
{
	CiPlayer_EVENT_UNKNOWN = 0,
	CiPlayer_EVENT_VIDEO_SRCSIZE_CHANGED,
	CiPlayer_EVENT_PLAYBACK_END,
	CiPlayer_EVENT_AVINFO_COMPLETE,
	CiPlayer_EVENT_VIDEO_NOT_SUPPORT,
	CiPlayer_EVENT_CREATE_COMPONENT_FAIL,// param 2 is component type(video, audio ,subtitle ,etc.), param3 is the component index
	CiPlayer_EVENT_STREAM_OPEN_FAIL,
	CiPlayer_EVENT_AUDIO_NOT_SUPPORT,
	CiPlayer_EVENT_LOST_CONNECTION,
	CiPlayer_EVENT_VIDEO_RESOLUTION_NOT_SUPPORT,
	CiPlayer_EVENT_STREAM_NOT_FOUND,
	CiPlayer_EVENT_STREAM_BUFFER_PERCENT,
	CiPlayer_EVENT_STREAM_BUFFER_INSUFFICIENT,
	CiPlayer_EVENT_STREAM_BUFFER_ENOUGH,
} CI_PLAYER_EVENT;

typedef enum _tagCI_PLAYER_DISPLAY_MODE
{
	CiPlayer_DISPLAY_MODE_LETTERBOX,
	CiPlayer_DISPLAY_MODE_STRETCH,
	CiPlayer_DISPLAY_MODE_CROPPING,
} CI_PLAYER_DISPLAY_MODE;

typedef enum _tagCI_PLAYER_METADATA_CODETYPE
{
	CiPlayer_MD_CODETYPE_UNKNOWN = 0,

	CiPlayer_MD_CODETYPE_MULTIBYTE = 0x10,
	CiPlayer_MD_CODETYPE_UTF8,
	CiPlayer_MD_CODETYPE_UTF16,
	CiPlayer_MD_CODETYPE_MAC,

	CiPlayer_MD_CODETYPE_JPG = 0x20,
	CiPlayer_MD_CODETYPE_PNG,

	CiPlayer_MD_CODETYPE_INT = 0x30,
	CiPlayer_MD_CODETYPE_FLOAT,
}CI_PLAYER_METADATA_CODETYPE;

typedef enum _tagCI_PLAYER_CODEC_TYPE
{
	CiPlayer_CODEC_UNKNOWN = 0,

	CiPlayer_CODEC_VIDEO_H264 = 0x1000,
	CiPlayer_CODEC_VIDEO_H263,
	CiPlayer_CODEC_VIDEO_MPEG4,
	CiPlayer_CODEC_VIDEO_MPEG2,
	CiPlayer_CODEC_VIDEO_MPEG1,
	CiPlayer_CODEC_VIDEO_WMV,
	CiPlayer_CODEC_VIDEO_VP9,
	CiPlayer_CODEC_VIDEO_VP8,
	CiPlayer_CODEC_VIDEO_VP6,
	CiPlayer_CODEC_VIDEO_DIVX,
	CiPlayer_CODEC_VIDEO_MJPEG,
	CiPlayer_CODEC_VIDEO_REAL,

	CiPlayer_CODEC_AUDIO_MP3 = 0x2000,
	CiPlayer_CODEC_AUDIO_MP2,
	CiPlayer_CODEC_AUDIO_AAC,
	CiPlayer_CODEC_AUDIO_AC3,
	CiPlayer_CODEC_AUDIO_EAC3,
	CiPlayer_CODEC_AUDIO_WMA,
	CiPlayer_CODEC_AUDIO_FLAC,
	CiPlayer_CODEC_AUDIO_PCM,
	CiPlayer_CODEC_AUDIO_APE,
	CiPlayer_CODEC_AUDIO_AMRNB,
	CiPlayer_CODEC_AUDIO_AMRWB,
	CiPlayer_CODEC_AUDIO_AMRWB_PLUS,
	CiPlayer_CODEC_AUDIO_REAL_COOKER,
	CiPlayer_CODEC_AUDIO_VORBIS,
}CI_PLAYER_CODEC_ID;

typedef enum {
	CI_STATUS_INVALID = 0,
	CI_STATUS_STOPPED,
	CI_STATUS_PAUSED,
	CI_STATUS_RUNNING,
	CI_STATUS_FASTPLAY,
	CI_STATUS_FASTSCAN,
	CI_STATUS_STEPFRAME,
}CI_PLAYER_STATUS_CODE;

typedef enum {
	CI_PLAYER_MEDIA_TYPE_INVALID = 0,
	CI_PLAYER_MEDIA_TYPE_AUDIO,
	CI_PLAYER_MEDIA_TYPE_VIDEO
}CI_PLAYER_MEDIA_TYPE;

typedef struct 
{
	CI_PLAYER_STATUS_CODE ePlayerStatus;
	ci32s s32PlaySpeed;  // 1000 scales of a second, negative value stands for backward playback.
}CI_PLAYER_END_INFO;

typedef ci32s (* PFNCI_PLAYER_CALLBACK) (ciVoid* param1, ciVoid* param2, ciVoid* param3, ciVoid* context);

typedef struct _tagCI_PLAYER_CALLBACK_ARRAY
{
	// param1 - CI_PLAYER_EVENT, param2 and param3 are event related parameters
	PFNCI_PLAYER_CALLBACK MessageCallBack;

	// Output thumbnail in bitmap format.
	// param1 - Not null value means a point to bitmap data buffer; Null for error occurred
	// param2 - bitmap data size 
	PFNCI_PLAYER_CALLBACK OutputThumbnail;

	ciVoid * context;
} CI_PLAYER_CALLBACK_ARRAY;

/**
 * @ingroup PlaybackSDK
 * Initialize the handle of player object.
 *
 * @param phPlayer pointer to player object.
 * @return 
 * CI_ERR_NONE								success <br>
 * CI_ERR_FAIL								on failure. <br>
 * CI_ERR_TIMEBOMB_EXPIRED					if the library has expired. <br>
 * CI_ERR_INVALID_PARAM						invalid parameter. <br>
 * @remark The initial interface must be called once ahead of other player interfaces, and the returned 
 * player object handle should pass to all other implemented player interface.
 * @version 
 */
CI_API ciResult CiPlayer_Initialize(ciHandle* phPlayer);


/**
 * @ingroup PlaybackSDK
 * Destroy the handle of player object.
 *
 * @param phPlayer pointer to player object created by CiPlayer_Initialize().
 * @return 
 * CI_ERR_NONE								success <br>
 * CI_ERR_FAIL								on failure. <br>
 * CI_ERR_INVALID_PARAM						invalid parameter. <br>
 * @remark The destroy interface must be called while exiting player program.
 * @version 
 */
CI_API ciResult CiPlayer_Destroy(ciHandle* phPlayer);

/**
 * @ingroup PlaybackSDK
 * Register player callback function.
 *
 * @param phPlayer pointer to player object created by CiPlayer_Initialize().
 * @param ptfnCallBackArray pointer to the callback function list.
 * @return 
 * CI_ERR_NONE								success <br>
 * CI_ERR_INVALID_PARAM						invalid parameter. <br>
 * @version 
 */
CI_API ciResult CiPlayer_RegisterCallback(ciHandle phPlayer, CI_PLAYER_CALLBACK_ARRAY * ptfnCallBackArray);


/**************************************************************************************************
 * Play control Commands include:
 *			"open <url>"					// open stream
 *			"play"							// start to play
 *			"pause"							// pause playback
 *			"resume"						// resume playback
 *			"stop"							// stop playback
 *			"close"							// close stream
 *			"thumbnail <millisecond>"		// get a thumbnail from a specific position. The thumbnail will output from callback. 
 *			"set position <millisecond>"	// seek to <millisecond>
 *			"get position"					// (number) get current position. the return value is current playback position if success.
 *			"set volume <volume>"			// set volume to <volume>, the valid volume range is from 0 to 100.
 *			"get volume"					// (number) get volume. the return value is current volume if success.
 *			"set display <stretch | letterbox | crop>"	// set AspectRatio mode.
 *			"get duration"					// (number) get duration. the return value is duration of file if success.
 *			"set window <handle>"			// set window handle value to sdk
 *			"set video <value>"				// set video on/off by setting value to 1/0.
 *			"get audioTrackCount"			// (number) get audio track count.
 *			"get videoTrackCount"			// (number) get video track count.
 *			"select audiotrack <idx>"		// select audio track by zero based index.
 *			"set audiodevice <deviceid>"	// select audio device by zero based index.
 *			"set audiooutdevicepower <value>" //set audio output device power on/off by setting value to 1/0.
 *			"set disprect <top> <left> <width> <height>" //set render output rectangle.
 *			"get title"						// get GST_TAG_TITLE info if exist
 *			"get homepage"					// get GST_TAG_HOMEPAGE info if exist
 * 			"mute <duration>"				// mute audio output, will not affect volume. Can do setting volume after mute, but the volume will be enabled only after unmute. "duration" is the fade duration in milli-seconds.
 * 			"unmute <duration>"				// disable muting audio output, will not affect volume. "duration" is the fade duration in milli-seconds.
 * 			"set earlyquit"					// quit/close current opening uri as soon as possible
 * 			"mutedevice <duration>"			// mute audio in audio output device plugin (ALSA). will no affect volume. "duration" is the fade duration in milli-seconds. can only co-work with unmutedevice.
 * 			"unmutedevice <duration>"		// disable muting audio in audio output device plugin (ALSA), will not affect volume. "duration" is the fade duration in milli-seconds. can only co-work with mutedevice.
 * 
 *
 *  MetaData related commands:
 *			"metadata open <file>"			// open <file> for querying meta data.
 *			"metadata get track"			// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and (byte arrary)
 *			"metadata get title"			//
 *          "metadata get artist"			//
 *          "metadata get album"			//
 *          "metadata get year"				//
 *          "metadata get comment"			//
 *          "metadata get genre"			//
 *          "metadata get auther"			//
 *          "metadata get copyright"		//
 *          "metadata get description"		//
 *			"metadata get pic count"		// (number) get picture count.
 *			"metadata get pic <index>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and (pic raw data) get picture by zero based index.
 *			"metadata mediainfo get duration"
 *			"metadata mediainfo get track count"
 *			"metadata mediainfo get track info <trackindex>"
 *			"metadata close"
 *			"metadata get location"			// streaming media address
 *			"metadata get organization"		// streaming media organize
 *
 *  Audio Information related commands:
 *			"audioinfo open <file>"						// open <file> for querying meta data.
 *          "audioinfo get duration"					// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get trackcount"					// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get samplerate <trackindex>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get bitrate <trackindex>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get channel <trackindex>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get codecname <trackindex>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get profilename <trackindex>"	// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and value.
 *          "audioinfo get codecid <trackindex>"		// (binary) pair of (4-byte int with type CI_PLAYER_METADATA_CODETYPE) and CI_PLAYER_CODEC_ID.
 *			"audioinfo close"
 *
 *  Fast forward/backward related commands
 *			"get direction"					// (number) get direction. -1 means backward, 1 means forward.
 *			"set direction <direction>"		// set direction. <direction>'s valid values are -1 for backward and 1 for forward.
 *			"get speed"						// (number) get playback speed.
 *			"set speed <speed>"				// set playback speed. <speed> value 1000 represents 1x, 2000 represents 2x and so on.
 *
 *  Thumbnail related commands
 *			"thumbnail open <file> <width> <height> <millisecond>"	// open <file> for thumbnail, the thumbnail data will returned by callback: OutputThumbnail
 *			"thumbnail close
 *
 ***** platform specified properties
 *			"set dispid <id>"				// set display id to <id>,  0 - HDMI, 1 - LCD.
 **************************************************************************************************
 *
 *************************************************************************************************/
 /**
 * @ingroup PlaybackSDK
 * Send command to playback SDK.
 *
 * @param phPlayer pointer to player object created by CiPlayer_Initialize().
 * @param cmd @ref COMMAND_STRING
 * @param result the return value of the command, a null terminated string.
 * @param ccResult size of result in characters (not bytes). This is the entire buffer size.
 * @param ccFilled the number of characters written, not including the null character. May be NULL if not needed.
 * @return 
 * CI_ERR_NONE								success <br>
 * CI_ERR_INVALID_PARAM						invalid parameter. <br>
 * CI_ERR_FAIL								failed. <br>
 * CI_ERR_NOT_INITED						sdk not initialized. <br>
 * CI_ERR_DEMUX_FAIL						demux operation failed. <br>
 * CI_ERR_AUDIO_DECODER_FAIL				audio decoder operation failed. <br>
 * CI_ERR_VIDEO_DECODER_FAIL				video decoder operation failed. <br>
 * CI_ERR_AUDIO_RENDER_FAIL					audio render operation failed. <br>
 * CI_ERR_VIDEO_RENDER_FAIL					video render operation failed. <br>
 * CI_ERR_VIDEO_RESOLUTION_NOT_SUPPORTED	video resolution not supported. <br>
 * @version 
 */
CI_API ciResult CiPlayer_SendCommand(ciHandle phPlayer, const TCHAR * cmd, TCHAR * result, ci32s ccResult, ci32s *ccFilled);


#ifdef __cplusplus
}
#endif

