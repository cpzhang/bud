set(ItemName "MaxExporter")

FILE(GLOB SRCS
  *.cpp 
  *.def
) 

FILE(GLOB HDRS
  *.h
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${MaxSDK}/include
  # ${MaxSDK}/include
  ../material
  ../particle
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${MaxSDK}/lib
  )
# If we build for windows systems, we also include the resource file containing the manifest, icon and other resources
if(WIN32)
  set(SRCS ${SRCS} *.rc)
endif(WIN32)
 
# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_library(${ItemName} SHARED
  ${SRCS} 
  ${HDRS}
  ${SRCS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  "Math.lib"
  "Base.lib"
  # "zlib.lib"
  core.lib
  geom.lib
  gfx.lib
  mesh.lib
  maxutil.lib
  paramblk2.lib
  )

set_target_properties(${ItemName} PROPERTIES 
SUFFIX ".dle"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;"
)

add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/MaxExporter.pdb" "${MaxPlugins}."
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/MaxExporter.dle" "${MaxPlugins}."
  )