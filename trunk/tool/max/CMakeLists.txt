SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)
set(MaxSDK "c:/Program Files (x86)/Autodesk/3ds Max 2012 SDK/maxsdk")
SET(MaxPlugins  "d:/Autodesk/3ds Max 2012/plugins/")
add_subdirectory(exporter)
# add_subdirectory(photoshop)