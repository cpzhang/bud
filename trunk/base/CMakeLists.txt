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