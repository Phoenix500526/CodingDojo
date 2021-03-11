add_executable(LeapYearTest LeapYearTest.cc)
target_include_directories(LeapYearTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(LeapYearTest ${CONAN_LIBS})
add_test(NAME LeapYearTest COMMAND LeapYearTest)