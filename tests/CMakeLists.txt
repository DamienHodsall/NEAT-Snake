find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(genetic Genetic_Tests.cpp)
target_link_libraries(genetic GTest::GTest GTest::Main genetic_lib)

add_executable(threads ThreadPool_Tests.cpp)
target_link_libraries(threads GTest::GTest GTest::Main thread_lib)

add_executable(serial Serial_Tests.cpp)
target_link_libraries(serial GTest::GTest GTest::Main Serial-linux game_lib)
