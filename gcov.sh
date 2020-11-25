#!/bin/bash

cmake . -Bbuild -DCMAKE_BUILD_TYPE=Debug -DENABLE_TEST=ON -DENABLE_GCOV=ON
make -C build

mkdir -p build/gcov

./build/test_pool
mkdir -p build/gcov/pool
gcov build/CMakeFiles/test_pool.dir/test/pool_test.cxx.gcno
gcov build/CMakeFiles/sMemPool.dir/source/list_allocator.cxx.gcno
mv *.gcov build/gcov/pool

./build/test_list
mkdir -p build/gcov/list_allocator
gcov build/CMakeFiles/sMemPool.dir/source/list_allocator.cxx.gcno
mv *.gcov build/gcov/list_allocator

./build/test_slab
mkdir -p build/gcov/slab_allocator
gcov build/CMakeFiles/test_slab.dir/test/slab_test.cxx.gcno
gcov build/CMakeFiles/sMemPool.dir/source/slab_allocator.cxx.gcno
mv *.gcov build/gcov/slab_allocator

./build/test_api
mkdir -p build/gcov/api
gcov build/CMakeFiles/test_api.dir/test/api_test.cxx.gcno
gcov build/CMakeFiles/sMemPool.dir/source/slab_allocator.cxx.gcno
gcov build/CMakeFiles/sMemPool.dir/source/list_allocator.cxx.gcno
gcov build/CMakeFiles/sMemPool.dir/source/api.cxx.gcno
mv *.gcov build/gcov/api
