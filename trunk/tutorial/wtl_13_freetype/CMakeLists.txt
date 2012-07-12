set(ItemName "wtl_13_freetype")

FILE(GLOB SRCS 
  *.cpp 
  ${CMAKE_SOURCE_DIR}/external/tinyXML2/tinyxml2.cpp
) 

FILE(GLOB HDRS 
  *.h
  ${CMAKE_SOURCE_DIR}/external/tinyXML2/tinyxml2.h
) 
# If we build for windows systems, we also include the resource file containing the manifest, icon and other resources
if(WIN32)
  set(SRCS ${SRCS} *.rc)
endif(WIN32)

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/wtl81/Include
  ${CMAKE_SOURCE_DIR}/external/tinyXML2/
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  "render.lib"
  "base.lib"
  "math.lib"
  "winmm.lib"
)
 
set_target_properties(${ItemName} PROPERTIES 
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
) 

add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/${ItemName}.exe" "${LIBRARY_OUTPUT_PATH}/tools/viewer/bin/viewer.exe"
  )