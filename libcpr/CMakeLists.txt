cmake_minimum_required(VERSION 3.0.0)
project(mylibcpr)

# include(FetchContent)
# FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
#                          GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
# FetchContent_MakeAvailable(cpr)


set(CMAKE_CXX_STANDARD 17)

find_package(cpr REQUIRED)
if (cpr_FOUND)
    message(STATUS "Found cpr: ${cpr_CONFIG} (found version ${cpr_VERSION})")
endif ()

add_executable(mylibcpr main.cpp)

target_link_libraries(mylibcpr PRIVATE cpr::cpr)

add_custom_target(run 
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)