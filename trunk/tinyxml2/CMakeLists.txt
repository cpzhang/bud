set(ItemName "tinyxml2")

include_directories(
  ${CMAKE_SOURCE_DIR}/external/tinyxml2/
  )

FILE(GLOB_RECURSE SRCS 
  tinyxml2.cpp
  ) 

FILE(GLOB_RECURSE HDRS
  tinyxml2.h
  ) 

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(${ItemName} STATIC 
  ${SRCS} 
  ${HDRS}
  ) 

# Add dependent external Libraries
# target_link_libraries(${ItemName}
  # winmm.lib
  # Dbghelp.lib
  # )

# set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  # COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
  # )