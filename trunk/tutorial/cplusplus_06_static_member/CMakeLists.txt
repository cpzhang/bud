set(ItemName "tutorial_cplusplus_06_static_member")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
) 

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
)

add_executable(${ItemName}
  ${SRCS}
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
)
 
set_target_properties(${ItemName} PROPERTIES 
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
) 