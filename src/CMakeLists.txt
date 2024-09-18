set(SOURCE_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(Foo)

add_executable(sample_app)
target_include_directories(sample_app PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(sample_app PRIVATE Main.cpp)
target_link_libraries(sample_app PRIVATE Foo)
