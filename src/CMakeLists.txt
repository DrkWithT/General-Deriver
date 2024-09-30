set(SOURCE_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(Models)
add_subdirectory(Frontend)
add_subdirectory(Syntax)
add_subdirectory(Backend)
add_subdirectory(Utils)
add_subdirectory(Tests)

add_executable(general_deriver)
target_include_directories(general_deriver PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(general_deriver PRIVATE Main.cpp)
target_link_libraries(general_deriver PRIVATE Models)
