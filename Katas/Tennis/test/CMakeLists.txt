add_executable(TennisTest TennisTest.cc)
target_include_directories(TennisTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(TennisTest ${CONAN_LIBS})
add_test(NAME TennisTest COMMAND TennisTest)