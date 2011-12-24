set(ItemName ${MathName})

FILE(GLOB SRCS 
  *.cpp
  ) 
FILE(GLOB HDRS
  *.h
  ) 

# Add include directories to the build.
include_directories(
  ${CMAKE_SOURCE_DIR}
  )

# Specify directories in which the linker will look for libraries.
# The command will apply only to targets created after it is called
# For historical reasons, relative paths given to this command are passed to the linker unchanged (unlike many CMake commands which interpret them relative to the current source directory).
link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )

add_library(${ItemName} SHARED 
  ${SRCS} 
  ${HDRS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  ${BaseName}.lib
  )

set_target_properties(${ItemName} PROPERTIES 
  DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE"
  )
