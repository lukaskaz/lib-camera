cmake_minimum_required(VERSION 3.10)

find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgproc imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
find_library(LIBCAMERA_LIBRARY libcamera.so REQUIRED)
find_library(LIBCAMERA_BASE_LIBRARY libcamera-base.so REQUIRED)

include_directories(${LIBCAMERA_INCLUDE_DIRS})
include_directories(${LIBCAMERA_BASE_INCLUDE_DIRS})

include(ExternalProject)

set(source_dir "${CMAKE_BINARY_DIR}/liblccv-src")
set(build_dir "${CMAKE_BINARY_DIR}/liblccv-build")

# file(GLOB patches "${CMAKE_SOURCE_DIR}/patches/*.patch")
# foreach(patch ${patches})
#   list(APPEND patching_cmd git am -s --keep-cr < ${patch} &&)
# endforeach()
# list(APPEND patching_cmd echo "End of patches")

EXTERNALPROJECT_ADD(
  liblccv
  GIT_REPOSITORY    https://github.com/kbarni/LCCV.git
  GIT_TAG           main
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            liblccv-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

include_directories(${source_dir}/include)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_BINARY_DIR}/liblogger-src")
set(build_dir "${CMAKE_BINARY_DIR}/liblogger-build")

EXTERNALPROJECT_ADD(
  liblogger
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-logger.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            liblogger-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)
