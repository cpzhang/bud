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
  "dxerr.lib"
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
DEFINE_SYMBOL "${ItemName}_EXPORTS;DEBUG"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;"
COMPILE_DEFINITIONS "PerfHUD"
#COMPILE_FLAGS "/Zp1"
)