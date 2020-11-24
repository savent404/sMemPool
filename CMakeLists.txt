cmake_minimum_required(VERSION 3.0)
project(sMemPool CXX C)

include_directories(include)
aux_source_directory(source src)

add_library(sMemPool ${src})

if(USE_TEST)
    add_executable(test test/malloc_test.cxx)
    target_link_libraries(test sMemPool)
endif(USE_TEST)

set_target_properties(sMemPool PROPERTIES PUBLIC_HEADER "include/smempool.h")

install(TARGETS sMemPool
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)
