find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(genetic Genetic_Tests.cpp)
target_link_libraries(genetic GTest::GTest GTest::Main genetic_lib)

add_executable(linkedlist LinkedList_Tests.cpp)
target_link_libraries(linkedlist GTest::GTest GTest::Main linkedlist_lib coordinate_lib)

add_executable(tests Genetic_Tests.cpp LinkedList_Tests.cpp)
target_link_libraries(tests GTest::GTest GTest::Main genetic_lib linkedlist_lib coordinate_lib)
gtest_discover_tests(tests)
add_custom_target(check COMMAND make test DEPENDS tests COMMENT "Running Tests...")
