cmake_minimum_required(VERSION 3.23)
project(
  sus
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(INSTALL_LIB_DIR
    ${CMAKE_INSTALL_PREFIX}/lib
    CACHE PATH "Library installation directory")
set(INSTALL_INCLUDE_DIR
    ${CMAKE_INSTALL_PREFIX}/include
    CACHE PATH "Include installation directory")
set(INSTALL_MAN_DIR
    ${CMAKE_INSTALL_PREFIX}/share/man
    CACHE PATH "Man page installation directory")

option(SUS_GEN_DOCS "Generates libsus documentation in HTML" OFF)
option(SUS_INSTALL_MAN_PAGES "Generates and installs man pages" ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Doxygen)

file(GLOB_RECURSE SUS_SOURCE_CODE CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SUS_HEADERS include/*.h)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SUS_SOURCE_CODE})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)
target_link_libraries(${CMAKE_PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw)
target_sources(
  ${CMAKE_PROJECT_NAME}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}/include FILES
         ${SUS_HEADERS})

if(${DOXYGEN_FOUND} AND (${SUS_GEN_DOCS} OR ${SUS_INSTALL_MAN_PAGES}))
  message(STATUS "Generating documentation build files...")

  if(NOT ${SUS_GEN_DOCS})
    set(DOXYGEN_GENERATE_HTML NO)
  endif()

  if(${SUS_INSTALL_MAN_PAGES})
    set(DOXYGEN_GENERATE_MAN YES)
  endif()

  set(DOXYGEN_STRIP_FROM_INC_PATH include)
  doxygen_add_docs(docs ${PROJECT_SOURCE_DIR}/include ALL)

  # proceed with zipping man pages
  if(${SUS_INSTALL_MAN_PAGES})
    add_custom_command(
      TARGET docs
      POST_BUILD
      COMMAND gzip ARGS -f *.3
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man3)
    file(GLOB_RECURSE SUS_MAN_PAGES_GZIP
         ${CMAKE_CURRENT_BINARY_DIR}/man/man3/*.3.gz)
    install(
      DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man3
      DESTINATION ${INSTALL_MAN_DIR}
      FILES_MATCHING
      PATTERN *.gz)
  endif()
endif()

install(
  TARGETS ${CMAKE_PROJECT_NAME}
  DESTINATION ${INSTALL_LIB_DIR}
  FILE_SET HEADERS
  DESTINATION ${INSTALL_INCLUDE_DIR})

set(CPACK_PACKAGE_NAME "libsus")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})

include(CPack)
