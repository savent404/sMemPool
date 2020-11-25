cmake_minimum_required(VERSION 3.0)
project(sMemPool CXX C)

option(ENABLE_GCOV "Enable gcov(debug, Linux only)" OFF)
option(ENABLE_TEST "Enable unit test" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED 14)

include_directories(include)
aux_source_directory(source src)

add_library(sMemPool ${src})

if(ENABLE_TEST)
    add_executable(test_list test/list_test.cxx)
    target_link_libraries(test_list sMemPool)

    add_executable(test_slab test/slab_test.cxx)
    target_link_libraries(test_slab sMemPool)

    add_executable(test_pool test/pool_test.cxx)
    target_link_libraries(test_pool sMemPool)

    add_executable(test_api test/api_test.cxx)
    target_link_libraries(test_api sMemPool)
endif()

if(ENABLE_GCOV)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
endif()

set_target_properties(sMemPool PROPERTIES PUBLIC_HEADER "include/smempool.h")

install(TARGETS sMemPool
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)
