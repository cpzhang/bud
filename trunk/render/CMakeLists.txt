set(ItemName ${RenderName})

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

FILE(GLOB FX
  ../../../data/shader/*.fx
) 

SOURCE_GROUP(FX FILES ${FX})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
  ${CMAKE_SOURCE_DIR}/external/directx/Include
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${CMAKE_SOURCE_DIR}/external/directx/lib/x86
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/lib
  ${CMAKE_SOURCE_DIR}/external/zlib/lib
  )

add_library(${ItemName} SHARED
  ${SRCS} 
  ${HDRS}
  ${FX}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
#  "dxerr.lib"
  "d3d9.lib"
  "d3dx9.lib"
  "winmm"
  "freetype.lib"
  "Math.lib"
  "Base.lib"
  "zlib.lib"
  tinyxml2.lib
  )

set_target_properties(${ItemName} PROPERTIES 
DEFINE_SYMBOL "${ItemName}_EXPORTS"
	COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;D3D_DEBUG_INFO"
# COMPILE_DEFINITIONS "NV_PERF_HUD"
#COMPILE_FLAGS "/Zp1"
)

add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/${ItemName}.dll" "${LIBRARY_OUTPUT_PATH}/tools/viewer/bin/${ItemName}.dll"
  )