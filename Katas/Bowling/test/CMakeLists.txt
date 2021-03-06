add_executable(BowlingUnitTest BowlingTest.cc)
target_include_directories(BowlingUnitTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(BowlingUnitTest ${CONAN_LIBS})
add_test(NAME BowlingUnitTest COMMAND BowlingUnitTest)