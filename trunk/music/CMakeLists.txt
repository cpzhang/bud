set(ItemName ${MusicName})

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/FMOD/api/inc
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${CMAKE_SOURCE_DIR}/external/FMOD/api/lib
  )

add_library(${ItemName} SHARED
  ${SRCS} 
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  ws2_32.lib
  winmm
  base.lib
  fmodexL_vc
  )

set_target_properties(${ItemName} PROPERTIES 
DEFINE_SYMBOL "${ItemName}_EXPORTS"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;"
#COMPILE_FLAGS "/Zp1"
)