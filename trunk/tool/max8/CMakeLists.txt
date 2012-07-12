SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)
set(MaxSDK "d:/max2008SDK/maxsdk")
SET(MaxPlugins  "d:/max2008/plugins/")
add_subdirectory(exportMZ)
