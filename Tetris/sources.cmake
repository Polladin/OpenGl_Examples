
file(GLOB COMMON_HEADER   		src/*.h)
file(GLOB COMMON_SRC      		src/*.cpp)

file(GLOB COMMON_GL_HEADER   		src/gl_common/*.h)
file(GLOB COMMON_GL_SRC      		src/gl_common/*.cpp)

file(GLOB SB7_HEADER   			src/sb7/*.h)
file(GLOB SB7_SRC      			src/sb7/*.cpp src/sb7/*.c)

file(GLOB TETRIS_HEADER   			src/tetris/*.h)
file(GLOB TETRIS_SRC      			src/tetris/*.cpp)

#file(GLOB SHADERS_SOURCE 		src/shaders/*.glsl)

#file(GLOB_RECURSE ENGINE_GL_FUNCTIONS_HEADER	src/gl_functions/*.h)
#file(GLOB_RECURSE ENGINE_GL_FUNCTIONS_SRC		src/gl_functions/*.cpp)
#
#file(GLOB_RECURSE PRIMITIVES_HEADER				src/primitives/*.h)
#file(GLOB_RECURSE PRIMITIVES_SRC				src/primitives/*.cpp)
#
file(GLOB_RECURSE SHADERS_SRC					src/shaders/*.glsl)


# Sources
set(SUPER_BIBLE_SRC 
					${COMMON_SRC} 
					${SB7_SRC}
					${COMMON_GL_SRC}
					${TETRIS_SRC}
)
         
# Common OpenGL functions		 
set(SUPER_BIBLE_HEADER 
					${COMMON_HEADER} 
					${SB7_HEADER}
					${COMMON_GL_HEADER}
					${TETRIS_HEADER}
)
           

# Create folders in Visual Studio
#source_group("header\\gl_functions"         	FILES ${ENGINE_GL_FUNCTIONS_HEADER})
#source_group("src\\gl_functions"            	FILES ${ENGINE_GL_FUNCTIONS_SRC})

source_group("header\\tetris"         			FILES ${TETRIS_HEADER})
source_group("src\\tetris"      		      	FILES ${TETRIS_SRC})

source_group("header\\sb7"         			   	FILES ${SB7_HEADER})
source_group("src\\sb7"      		      		FILES ${SB7_SRC})

source_group("header\\gl_common"   			   	FILES ${COMMON_GL_HEADER})
source_group("src\\gl_common"   	      		FILES ${COMMON_GL_SRC})

source_group("header"               			FILES ${COMMON_HEADER})          
source_group("src"               				FILES ${COMMON_SRC})        

#source_group("shaders"							FILDE ${SHADERS_SOURCE})           

source_group("shaders" 							FILES ${SHADERS_SRC} )
                   
# Create folders in Visual Studio
#                   
#source_group("src\\engine\\common"               FILES ${ENGINE_COMMON_SRC})                   
#source_group("src\\engine\\core"                 FILES ${CORE_SRC})
#source_group("src\\engine\\tools"                FILES ${ENGINE_TOOLS_SRC})
#source_group("src\\engine\\map"                  FILES ${MAP_SRC})
#source_group("src\\engine\\buildings"            FILES ${BUILDINGS_SRC})
#source_group("src\\engine\\vehicles"             FILES ${VEHICLES_SRC})
#source_group("src\\graphics\\gl_functions"       FILES ${GL_FUNCTIONS_SRC})
#source_group("src\\graphics\\tools"              FILES ${GRAPHICS_TOOLS_SRC})
#source_group("src\\event_machine"                FILES ${EVENT_MACHINE_SRC})
#source_group("src"                               src/main.cpp)
#
#source_group("header\\engine\\common"            FILES ${ENGINE_COMMON_HEADER})        
#source_group("header\\engine\\core"              FILES ${CORE_HEADER})
#source_group("header\\engine\\tools"             FILES ${ENGINE_TOOLS_HEADER})
#source_group("header\\engine\\map"               FILES ${MAP_HEADER})
#source_group("header\\engine\\buildings"         FILES ${BUILDINGS_HEADER})
#source_group("header\\engine\\vehicles"          FILES ${VEHICLES_HEADER})
#source_group("header\\graphics\\gl_functions"    FILES ${GL_FUNCTIONS_HEADER})
#source_group("header\\graphics\\tools"           FILES ${GRAPHICS_TOOLS_HEADER})
#source_group("header\\event_machine"             FILES ${EVENT_MACHINE_HEADER})