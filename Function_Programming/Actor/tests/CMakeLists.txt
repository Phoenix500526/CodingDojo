add_executable(ActorTest ActorTest.cc ../service.cc)
target_include_directories(ActorTest PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(ActorTest ${CONAN_LIBS})
add_test(NAME ActorTest COMMAND ActorTest)