set(ItemName "tutorial_directx_initialization")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
) 

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../bin)

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  # ${CMAKE_SOURCE_DIR}/external/directx/lib/x86
  # ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/lib
  # ${CMAKE_SOURCE_DIR}/external/zlib/lib
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
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