SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)

# tutorial
# add_subdirectory(shader_introduction)
# add_subdirectory(CFont)
add_subdirectory(directx_00_empty)
add_subdirectory(directx_01_initialization)
add_subdirectory(directx_02_triangle)
add_subdirectory(directx_03_fx)
add_subdirectory(directx_04_transformation)
add_subdirectory(directx_05_color)
add_subdirectory(directx_06_texture)
add_subdirectory(directx_07_light)
add_subdirectory(directx_08_instancingHardware)
add_subdirectory(directx_09_instancingShader)
add_subdirectory(directx_10_normalMapping)
add_subdirectory(directx_11_pick)
add_subdirectory(directx_12_freetype)