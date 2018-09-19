include(sources.cmake)

cmake_minimum_required(VERSION 2.8)

project(CarProject)


#################### Definitions #################################

add_definitions(-std=c++1y -Wall)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

# Add predefined variables for:
#   - Shaders
#   - Textures
add_definitions(-DSRC_WITH_SHADERS="${PROJECT_SOURCE_DIR}/src/shaders")
add_definitions(-DTEXTURE_FOLDER="${PROJECT_SOURCE_DIR}/textures")


################### Import libs #####################################

#LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/import/libs/win64/lib )

# Import GLEW
#set (GLEW_libs ${PROJECT_SOURCE_DIR}/import/libs/win64/lib/glew32s.lib)

# Import GLFW
set (GLFW_libs ${PROJECT_SOURCE_DIR}/import/libs/win64/lib/glfw3.lib)

# Import Asimp
#set (ASYMP_libs ${PROJECT_SOURCE_DIR}/import/libs/win64/lib/assimp-vc140-mt.lib
#                ${PROJECT_SOURCE_DIR}/import/libs/win64/dll/assimp-vc140-mt.dll
#)

## Add SOIL lib
#set (SOIL_include /opt/SOIL/include)
#set (SOIL_libs /opt/SOIL/lib/libSOIL.a)
set(SOIL_libs ${PROJECT_SOURCE_DIR}/import/libs/win64/lib/SOIL.lib)

# Imported lib
set (Imported_include ${PROJECT_SOURCE_DIR}/import/include)

# SB7 libs
#find_package(OpenGL)

set(OPENGL_LIBRARIES opengl32)

set (CMAKE_DEBUG_POSTFIX "_d")
#set(SB7_COMMON_LIBS optimized glfw3 debug glfw3_d ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

set(SB7_COMMON_LIBS ${GLFW_libs} ${OPENGL_LIBRARIES})

################## Include #############################################

include_directories(${PROJECT_SOURCE_DIR}/src)

################### FactoryGL ##########################################

add_executable(Car ${SUPER_BIBLE_SRC} ${SUPER_BIBLE_HEADER} ${SHADERS_SRC} )
set_target_properties(Car PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Car PUBLIC ${SOIL_include}
                                            ${Imported_include} )
                                            
target_link_libraries(Car ${SB7_COMMON_LIBS} ${SOIL_libs}
)
                                