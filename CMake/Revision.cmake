# for revision info
find_package(Git)
if(GIT_FOUND)
  # make sure version information gets re-run when the current Git HEAD changes
  execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --git-path HEAD
      OUTPUT_VARIABLE orion_git_head_filename
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${orion_git_head_filename}")

  execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --symbolic-full-name HEAD
      OUTPUT_VARIABLE orion_git_head_symbolic
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      COMMAND ${GIT_EXECUTABLE} rev-parse --git-path ${orion_git_head_symbolic}
      OUTPUT_VARIABLE orion_git_head_symbolic_filename
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${orion_git_head_symbolic_filename}")

  # defines ORION_WC_REVISION
  EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
      OUTPUT_VARIABLE ORION_WC_REVISION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  # defines ORION_WC_DESCRIBE
  EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} describe --always --long --dirty
      OUTPUT_VARIABLE ORION_WC_DESCRIBE
      OUTPUT_STRIP_TRAILING_WHITESPACE)

  # remove hash (and trailing "-0" if needed) from description
  STRING(REGEX REPLACE "(-0)?-[^-]+((-dirty)?)$" "\\2" ORION_WC_DESCRIBE "${ORION_WC_DESCRIBE}")

  # defines ORION_WC_BRANCH
  EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
      OUTPUT_VARIABLE ORION_WC_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# version number
set(ORION_VERSION_MAJOR "0")
set(ORION_VERSION_MINOR "1")
if(ORION_WC_BRANCH STREQUAL "stable")
  set(ORION_VERSION_PATCH "0")
else()
  set(ORION_VERSION_PATCH ${ORION_WC_REVISION})
endif()

# If this is not built from a Git repository, default the version info to
# reasonable values.
if(NOT ORION_WC_REVISION)
  set(ORION_WC_DESCRIBE "${ORION_VERSION_MAJOR}.${ORION_VERSION_MINOR}")
  set(ORION_WC_REVISION "${ORION_WC_DESCRIBE} (unk)")
  set(ORION_WC_BRANCH "master")
endif()

if(ORION_WC_BRANCH STREQUAL "master" OR ORION_WC_BRANCH STREQUAL "stable")
  set(ORION_WC_IS_STABLE "1")
else()
  set(ORION_WC_IS_STABLE "0")
endif()
