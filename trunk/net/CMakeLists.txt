set(ItemName ${NetName})

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

include_directories(
  ${CMAKE_SOURCE_DIR}
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )

add_library(${ItemName} SHARED
  ${SRCS} 
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  ws2_32.lib
  # "winmm"
  math.lib
  base.lib
  )

set_target_properties(${ItemName} PROPERTIES 
DEFINE_SYMBOL "${ItemName}_EXPORTS;DEBUG"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;"
#COMPILE_FLAGS "/Zp1"
)