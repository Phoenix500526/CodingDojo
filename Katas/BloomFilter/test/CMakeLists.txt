add_executable(BloomFilterTest BloomFilterTest.cc)
target_include_directories(BloomFilterTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(BloomFilterTest ${CONAN_LIBS})
add_test(NAME BloomFilterTest COMMAND BloomFilterTest)