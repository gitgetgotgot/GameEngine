add_library(ExternalLibs INTERFACE)

target_include_directories(ExternalLibs INTERFACE
    ${CMAKE_SOURCE_DIR}/Libraries/include
    ${CMAKE_SOURCE_DIR}/Libraries/include/freetype
    ${CMAKE_SOURCE_DIR}/Libraries/include/json
    ${CMAKE_SOURCE_DIR}/Libraries/include/stb
)

target_link_directories(ExternalLibs INTERFACE
    ${CMAKE_SOURCE_DIR}/Libraries/lib
)

target_link_libraries(ExternalLibs INTERFACE
    glfw3
    freetype
)
