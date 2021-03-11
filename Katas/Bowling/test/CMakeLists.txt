add_executable(BowlingTest BowlingTest.cc)
target_include_directories(BowlingTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(BowlingTest ${CONAN_LIBS})
add_test(NAME BowlingTest COMMAND BowlingTest)