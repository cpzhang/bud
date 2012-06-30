set(ItemName ${BaseName})

FILE(GLOB_RECURSE SRCS 
  *.cpp
  ) 

FILE(GLOB_RECURSE HDRS
  *.h
  ) 

include_directories(
  ${CMAKE_SOURCE_DIR}
  )

add_library(${ItemName} SHARED 
  ${SRCS} 
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  winmm.lib
  Dbghelp.lib
  )

set_target_properties(${ItemName} PROPERTIES 
  DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
  )
add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/${ItemName}.dll" "${LIBRARY_OUTPUT_PATH}/tools/viewer/bin/${ItemName}.dll"
  )