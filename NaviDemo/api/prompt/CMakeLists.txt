
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(prompt-api)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

set(CAPI_DEPENDS
    ${CAPI_FIDL_DIR}/svp/media/prompt/PromptPlayer.fidl
    ${CAPI_FIDL_DIR}/svp/media/prompt/PromptPlayerTypes.fidl
    )

set(FIDL_FILES ${CAPI_FIDL_DIR}/svp/media/prompt/PromptPlayer.fidl)

PROCESS_FIDLS()


