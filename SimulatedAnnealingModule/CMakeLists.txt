#######################################################################################################################
# ___________________________________________________________________________________________________________________ #
# ___________________________________________/\/\______/\/\/\/\__/\/\______/\/\______________________________________ #
# _________________________________________/\/\/\/\______/\/\____/\/\/\__/\/\/\______________________________________ #
# _______________________________________/\/\____/\/\____/\/\____/\/\/\/\/\/\/\______________________________________ #
# _______________________________________/\/\/\/\/\/\____/\/\____/\/\__/\__/\/\______________________________________ #
# _______________________________________/\/\____/\/\__/\/\/\/\__/\/\______/\/\______________________________________ #
# ___________________________________________________________________________________________________________________ #
#                                                                                                                     #
#######################################################################################################################
# This is an AIM module. It is generated by aimcreate-pkg, one of the aimtools. It can be warped into multiple 
# middlewares by the RurBuilder. It can be run as a C++ module in the Node Javascript world, as a lightweight robotic
# software module, or on exciting new hardware such as a modern Android phone or a Raspberry PI. 
#
# The build systems uses CMake. All the automatically generated code falls under the Lesser General Public License 
# (LGPL GNU v3). The author can choose his/her own license for the code that is not automatically generated by the 
# AimTools and the RurBuilder.
#
# Author:	 Homer J. Simpson (Springfield Power Company)
# Date: 	 nov  5, 2014
#
# ASCII Art generated by: http://www.network-science.de/ascii/ with font "ticks".
#
# Copyright © 2014 Homer J. Simpson <chunkylover53@aol.com>
#######################################################################################################################

IF(WIN32)
	CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ELSE(WIN32)
	CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
ENDIF(WIN32)

IF(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# Set name of project
SET(PROJECT_NAME "SimulatedAnnealingModule")

# Start a project
PROJECT(${PROJECT_NAME})

# The directory with some of the FindXXX modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
MESSAGE(STATUS "Search for FindX files in ${CMAKE_SOURCE_DIR}/cmake")

#######################################################################################################################
# Include your own FindX cmake files and options in the following file!
#######################################################################################################################

INCLUDE(CustomBuild.cmake OPTIONAL)

#######################################################################################################################

# First set empty
SET(PARENT_INCLUDE_DIRS "")

# Include aim stuff
ADD_SUBDIRECTORY(aim-core)

# Get include directories
FOREACH(INCLDIR ${PARENT_INCLUDE_DIRS})
	MESSAGE(STATUS "Add ${INCLDIR}")
	INCLUDE_DIRECTORIES(${INCLDIR})
ENDFOREACH()

# Some debug information
MESSAGE(STATUS "\"${PROJECT_NAME}\" uses the following CXX flags: \"${CMAKE_CXX_FLAGS}\" (can be empty)")
MESSAGE(STATUS "Linked libraries: ${LIBS}")

# Source directory
SET(SOURCE_DIR "src")
SET(INCLUDE_DIR "inc")

# Search for functional source code
FILE(GLOB FOLDER_SOURCE ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.cc ${SOURCE_DIR}/*.c)
FILE(GLOB FOLDER_HEADER ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hh ${INCLUDE_DIR}/*.hpp)

# For Visual Studio group the files
SOURCE_GROUP("Source files" FILES ${FOLDER_SOURCE})
SOURCE_GROUP("Header files" FILES ${FOLDER_HEADER})

# Automatically add include directories if needed.
FOREACH(HEADER_FILE ${FOLDER_HEADER})
	GET_FILENAME_COMPONENT(HEADER_DIR ${HEADER_FILE} PATH)
	INCLUDE_DIRECTORIES(${HEADER_DIR})
ENDFOREACH(HEADER_FILE ${FOLDER_HEADER})

# For references to header files
INCLUDE_DIRECTORIES(aim-core/inc)

# Show which directories are actually included to the user
GET_PROPERTY(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
FOREACH(dir ${dirs})
	MESSAGE(STATUS "INCLUDE_DIRECTORY='${dir}'")
ENDFOREACH()

# A final check for a file that has to be generated by the RurBuilder. It is recommended to commit this file even 
# though it is automatically generated to a repository, so people can build your modules if they have this utility
# not on their system.
SET(PROJECT_HEADER ${CMAKE_SOURCE_DIR}/aim-core/inc/${PROJECT_NAME}.h)
IF(NOT EXISTS ${PROJECT_HEADER})
	MESSAGE(FATAL_ERROR "No ${PROJECT_HEADER} header file found. "
		"Did the RurBuilder run and generate a header file (with content) in aim-core/inc? "
		"Did you perhaps forget to install omniidl/omniORB or do you need to update the RurBuilder itself? ")
ENDIF() 

MESSAGE(STATUS "Header files included: ${AIM_HEADERS} ${FOLDER_HEADER}")

# Set up our main executable.
IF(FOLDER_SOURCE STREQUAL "")
	MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ELSE()
	ADD_EXECUTABLE(${PROJECT_NAME} ${FOLDER_SOURCE} ${AIM_SOURCES} ${FOLDER_HEADER} ${AIM_HEADERS})
	
	# Dependency for ROS (ignored if not building for ROS)
	ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}_gencpp)
	
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ENDIF()

