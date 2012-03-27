SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
)
set(MayaSDKInclude "d:/Alias/Maya7.0/include")
set(MayaSDKLib "d:/Alias/Maya7.0/lib")
SET(MayaPlugins  "d:/Alias/Maya7.0/bin/plug-ins")
add_subdirectory(exporter)
# add_subdirectory(previewer)
add_subdirectory(shader)