CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)

# Build Type
MESSAGE(STATUS "Build Type is       : ${CMAKE_BUILD_TYPE}")

# Operation System
MESSAGE(STATUS "Operation System is : ${CMAKE_SYSTEM}")
MESSAGE(STATUS "System Name is      : ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "Current Platform    : Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Current Platform    : Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    MESSAGE(STATUS "Current Platform    : FreeBSD")
ELSE ()
    MESSAGE(STATUS "Other Platform      : ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

IF (WIN32)
    MESSAGE(STATUS "Now is Windows")
ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple systems.")
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

# arch
IF(CMAKE_CL_64)
    set(ARCH x64)
ELSE(CMAKE_CL_64)
    set(ARCH x86)
ENDIF(CMAKE_CL_64)
MESSAGE(STATUS "ARCH                : ${ARCH}")

# root dir
SET(PATH_ROOT ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "PATH_ROOT           : ${PATH_ROOT}")

SET(PATH_OUTPUT ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "PATH_OUTPUT         : ${PATH_OUTPUT}")
SET(LIBRARY_OUTPUT_PATH    ${PATH_OUTPUT}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PATH_OUTPUT}/bin)

#MESSAGE(STATUS "CMAKE_ROOT          : ${CMAKE_ROOT}")

####################################### spdlog start        ################################################
SET(VERSION_SPDLOG          origin)
SET(PATH_SPDLOG ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/${VERSION_SPDLOG})
#ADD_SUBDIRECTORY(thirdparty/spdlog/${VERSION_SPDLOG})
####################################### spdlog end          ################################################

ADD_SUBDIRECTORY(logger)

