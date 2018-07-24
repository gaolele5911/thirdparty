
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(api)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

macro(PROCESS_FIDLS)
# CAPI_GEN detection
set(ENABLE_CAPI_GEN FALSE)

set(CAPI_MD5_CACHE_FILE ${CMAKE_CURRENT_BINARY_DIR}/capi-cache.md5)

if(NOT (EXISTS ${CAPI_MD5_CACHE_FILE}))
    set(ENABLE_CAPI_GEN TRUE)
    foreach(_capi ${CAPI_DEPENDS})
        FILE(MD5 ${_capi} _capi_md5)
        FILE(APPEND ${CAPI_MD5_CACHE_FILE} "${_capi_md5} ")
    endforeach()
else()
    set(_capi_md5_offset 0)
    foreach(_capi ${CAPI_DEPENDS})
        FILE(MD5 ${_capi} _capi_md5)
        FILE(READ ${CAPI_MD5_CACHE_FILE} _cache_capi_md5 LIMIT 33 OFFSET ${_capi_md5_offset})
        STRING(STRIP ${_cache_capi_md5} _cache_capi_md5)
        MATH(EXPR _capi_md5_offset "${_capi_md5_offset} + 33")
        if(NOT (${_capi_md5} STREQUAL ${_cache_capi_md5}))
            set(ENABLE_CAPI_GEN TRUE)
            break()
        endif()
    endforeach()
endif()

# CORE_GEN
if(ENABLE_CAPI_GEN)
    execute_process(
        COMMAND $ENV{CAPI_CORE_GEN} -pf -d ${CAPI_CORE_GEN_DIR} -ns ${FIDL_FILES}
    )
endif()

endmacro()

# N/A

# 3. Project Build
add_subdirectory(positioning)
#add_subdirectory(tts)
#add_subdirectory(audio)
#add_subdirectory(prompt)
#add_subdirectory(display)
#add_subdirectory(wifi)
add_subdirectory(appmgr)
add_subdirectory(source)
#add_subdirectory(power)