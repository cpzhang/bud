SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)

# add_subdirectory(max)
# add_subdirectory(max2012)
# add_subdirectory(max8)
# add_subdirectory(photoshop)
add_subdirectory(maya)
# add_subdirectory(flash)