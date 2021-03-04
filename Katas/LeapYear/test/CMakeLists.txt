add_executable(LeapYearUnitTest LeapYearTest.cc)
target_include_directories(LeapYearUnitTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(LeapYearUnitTest ${CONAN_LIBS})
add_test(NAME LeapYearUnitTest COMMAND LeapYearUnitTest)