set(ItemName "tutorial_flash_01_hello")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
) 

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  ${CMAKE_SOURCE_DIR}/external/directx/lib/x86
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/tool
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  # flashplugin.lib
  shlwapi.lib
  "dxerr.lib"
  "d3d9.lib"
  "d3dx9.lib"
  "dxerr.lib"
)
 
set_target_properties(${ItemName} PROPERTIES 
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
) 