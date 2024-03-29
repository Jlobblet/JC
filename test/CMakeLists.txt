include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
find_package(Check)
find_package(Threads)

if (Check_FOUND AND Threads_FOUND)

  include_directories(${CHECK_INCLUDE_DIRS})

  set(TEST_SOURCES
          check_jbits_prop.c
  )

  add_executable(check_jbits ${TEST_SOURCES})
  target_link_libraries(check_jbits JC ${CHECK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} theft)
  target_compile_options(check_jbits PUBLIC -Wno-unused-parameter)

  ###############################################################################
  include(CheckCSourceCompiles)
  include(CheckCSourceRuns)
  include(CheckFunctionExists)
  include(CheckIncludeFile)
  include(CheckIncludeFiles)
  include(CheckLibraryExists)
  include(CheckSymbolExists)
  include(CheckTypeSize)

  ###############################################################################
  # Check headers
  set(INCLUDES "")
  macro(ck_check_include_file header var)
    check_include_files("${INCLUDES};${header}" ${var})
    if(${var})
      set(INCLUDES ${INCLUDES} ${header})
    endif(${var})
  endmacro(ck_check_include_file)

  ck_check_include_file("stdlib.h" HAVE_STDLIB_H)

  ###############################################################################
  # Check functions

  ###############################################################################
  # Check defines

  ###############################################################################
  # Check struct members

  ###############################################################################
  # Check for integer types
  # (The following are used in check.h. Regardless if they are used in
  # the project, they will need to be checked in order to use Check).
  check_type_size(intmax_t INTMAX_T)
  check_type_size(uintmax_t UINTMAX_T)

  check_type_size(pid_t PID_T)
  if(NOT HAVE_PID_T)
    if(WIN32)
      set(pid_t "int")
    else(WIN32)
      MESSAGE(FATAL_ERROR "pid_t doesn't exist on this platform?")
    endif(WIN32)
  endif(NOT HAVE_PID_T)

  ###############################################################################
  # Check libraries

  ###############################################################################
  # Generate "config.h" from "cmake/config.h.cmake"
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h)
  include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
  add_definitions(-DHAVE_CONFIG_H)
  set(CONFIG_HEADER ${CMAKE_CURRENT_BINARY_DIR}/config.h)

  ###############################################################################
  # Unit tests
  enable_testing()
  add_test(NAME check_jbits COMMAND check_jbits)

endif()
