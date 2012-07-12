set(ItemName "tutorial_wtl")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
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
  ${CMAKE_SOURCE_DIR}/external/FMOD/api
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ${SRCS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
    ${CMAKE_SOURCE_DIR}/external/FMOD/api/lib/fmodexL_vc.lib
)
 
set_target_properties(${ItemName} PROPERTIES 
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
) 