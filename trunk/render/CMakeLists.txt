set(ItemName ${RenderName})

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../bin)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
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
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  #"dxerr.lib"
  "d3d9.lib"
  "d3dx9.lib"
  "dxerr.lib"
  "winmm"
  "freetype.lib"
  "Math.lib"
  "Base.lib"
  "zlib.lib"
  )

set_target_properties(${ItemName} PROPERTIES 
DEFINE_SYMBOL "${ItemName}_EXPORTS"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;D3D_DEBUG_INFO"
COMPILE_DEFINITIONS "DEBUG"
# COMPILE_DEFINITIONS "PerfHUD"
#COMPILE_FLAGS "/Zp1"
)