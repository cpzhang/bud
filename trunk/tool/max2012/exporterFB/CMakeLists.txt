set(ItemName "MaxExporter")

FILE(GLOB SRCS 
  *.cpp 
  *.def
  # *.rc
) 

FILE(GLOB HDRS 
  *.h
) 

FILE(GLOB maxSDK 
  ${MAX_DIR}/include/*.h
) 

add_library(${ItemName} SHARED 
  ${SRCS}
  ${HDRS}
  # ${maxSDK}
  ) 
SOURCE_GROUP(maxSDK  ${MAX_DIR})
SET(LIBRARY_OUTPUT_PATH  ${BIN_DIR})
SET(Maya_Plugins_Path  "c:/Program Files/Autodesk/3ds Max 2011/plugins/")

include_directories(
  ${MAX_DIR}/include
  ${MAX_DIR}/include/cs
  # ${CMAKE_SOURCE_DIR}/Base/Base/Include
  ${CMAKE_SOURCE_DIR}/Buddha
  # ${CMAKE_SOURCE_DIR}
)

# Add dependent external Libraries
target_link_libraries(${ItemName}
  # *.lib 
  odbc32.lib 
  odbccp32.lib 
  comctl32.lib 
  ${MAX_DIR}/lib/core.lib
  ${MAX_DIR}/lib/geom.lib 
  ${MAX_DIR}/lib/gfx.lib
  ${MAX_DIR}/lib/mesh.lib 
  ${MAX_DIR}/lib/maxutil.lib
  ${MAX_DIR}/lib/maxscrpt.lib 
  ${MAX_DIR}/lib/paramblk2.lib
   ${MAX_DIR}/lib/helpsys.lib
  # "${BIN_DIR}/${BUILD_TYPE_BIN_DIR}/Buddha.lib"
  # "${BIN_DIR}/${BUILD_TYPE_BIN_DIR}/libBase_d.lib"
  )
 
set_target_properties(${ItemName} PROPERTIES 
SUFFIX ".dle"
OUTPUT_NAME "MaxExporter"
COMPILE_DEFINITIONS "WIN32;_DEBUG;_WINDOWS;NT_PLUGIN;REQUIRE_IOSTREAM"
) 
SET(CMAKE_C_FLAGS_DEBUG "/MTd /Z7 /Od")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MDd /Zi /O2 /Ob1 /D NDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "/MTd /Z7 /Od")

add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/MaxExporter.pdb" "${Maya_Plugins_Path}."
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/MaxExporter.dle" "${Maya_Plugins_Path}."
  )