/***************************************************************************************************
*
* Company       : Huizhou Desay SV Automotive Co., Ltd.
*
* Division      : Automotive Electronics, Desay Corporation
*
* Business Unit : Central Technology
*
* Department    : Advanced Development (Huizhou)
*
****************************************************************************************************/

#pragma once

#include "IVDEC_Defs.h"

// VDEC Interface
struct IVDEC {
    // READONLY: CI_RESULT create();
    std::function<CI_RESULT()> const create;

    // READONLY: CI_RESULT open(VDEC_OpenOptions_t *pOptions);
    std::function<CI_RESULT(VDEC_OpenOptions_t *)> const open;

    // READONLY: CI_RESULT close();
    std::function<CI_RESULT()> const close;

    // READONLY: CI_U32 release();
    std::function<CI_U32()> const release;

    // READONLY: CI_RESULT decodeBuffer(VDEC_DecodeOptions_t *pOptions, uint8_t *buffer, uint32_t length, uint32_t *consumed, uint32_t *decodedFrames, uint32_t *skippedFrames, uint32_t *status);
    std::function<CI_RESULT(VDEC_DecodeOptions_t *, uint8_t *, uint32_t, uint32_t *, uint32_t *, uint32_t *, uint32_t *)> const decodeBuffer;

    // READONLY: CI_RESULT getFrame(VDEC_GetFrameOptions_t *pOptions, VDEC_Frame_t *pFrame);
    std::function<CI_RESULT(VDEC_GetFrameOptions_t *, VDEC_Frame_t *)> const getFrame;

    // READONLY: CI_RESULT renderFrame(VDEC_RenderFrameOptions_t *pOptions, VDEC_Frame_t *pFrame);
    std::function<CI_RESULT(VDEC_RenderFrameOptions_t *, VDEC_Frame_t *)> const renderFrame;

    // READONLY: CI_U32 releaseFrame(VDEC_Frame_t *pFrame);
    std::function<CI_U32(VDEC_Frame_t *)> const releaseFrame;

    // READONLY: CI_RESULT get(uint32_t propID, void *buffer, uint32_t length, uint32_t *used);
    std::function<CI_RESULT(uint32_t, void *, uint32_t, uint32_t *)> const get;

    // READONLY: CI_RESULT set(uint32_t propID, void *buffer, uint32_t length);
    std::function<CI_RESULT(uint32_t, void *, uint32_t)> const set;
};

extern IVDEC& VDEC();

