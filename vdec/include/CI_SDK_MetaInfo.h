/***************************************************************************
*   File Name: CI_SDK_MetaInfo.h
*   Create Date: 2017-06-27 (YYYY-MM-DD)
*   Vesion: 01.00.00 (xx.xx.xx)
*   Copyright (C) 2007 by Cidana(ShangHai)                                      
*   Email: jiajun.dai@cidana.com                                               
*                                                                         
*   This program is distributed in the hope that it will be useful,       
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*                          
*   The above copyright notice and this permission notice shall be 
*   included in all copies or substantial portions of the Software.
*                                                                         
*   You should have received a copy of the Cidana License     
*   along with this program; if not, write to the                         
*   ShangHai Cidana, Inc.                                       
***************************************************************************/

#ifndef _CI_SDK_METAINFO_H_
#define _CI_SDK_METAINFO_H_

#ifndef CI_MAX_NAME_LEN
#define CI_MAX_NAME_LEN 32
#endif

typedef struct _tagCI_METAINFO
{
	unsigned int u32Duration;
	unsigned int u32Seekable;

	unsigned int u32TitleSize;
	unsigned char *pu8Title;

	unsigned int u32ArtistSize;
	unsigned char *pu8Artist;

	unsigned int u32AlbumSize;
	unsigned char *pu8Album;

	unsigned int u32GenreSize;
	unsigned char *pu8Genre;

	unsigned int u32YearSize;
	unsigned char *pu8Year;

	unsigned int u32PicSize;
	unsigned char *pu8Pic;
	unsigned int u32PicType;

	struct{
		unsigned int u32Valid;
		unsigned int u32AudioSamplingFreq;
		unsigned int u32AudioChannel;
		unsigned int u32AudBitsPerSample;
		unsigned int u32BitRate;
		unsigned int u32CurBitRate;
		unsigned char au8CdcNameStr[CI_MAX_NAME_LEN*4];
		int CodecID;
	}Audio;

	struct{
		unsigned int u32Valid;
		unsigned int u32Width;
		unsigned int u32Height;
		unsigned int u32BitRate;
		unsigned int u32FrameRate;
		unsigned char au8CdcNameStr[CI_MAX_NAME_LEN*4];
		int CodecID;
	}Video;

	unsigned char  au8DemuxNameStr[CI_MAX_NAME_LEN];
}CI_METAINFO;

#endif  // ~_CI_SDK_METAINFO_H_
