set(ItemName "tutorial_directx_17_terrainTextureLightingLightMapping")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
) 

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  "render.lib" 
  "math.lib"
  "base.lib"
  )
 
set_target_properties(${ItemName} PROPERTIES 
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
) 