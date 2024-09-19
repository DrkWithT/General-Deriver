set(SOURCE_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(Foo)

add_executable(general_deriver)
target_include_directories(general_deriver PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(general_deriver PRIVATE Main.cpp)
target_link_libraries(general_deriver PRIVATE Foo)
