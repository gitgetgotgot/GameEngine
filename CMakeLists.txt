cmake_minimum_required(VERSION 3.20)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

include(cmake/EngineModule.cmake)
include(cmake/ExternalLibs.cmake)

#add_subdirectory(Modules/Core)

add_engine_module(Core)
add_engine_module(CustomScripts)
add_engine_module(Editor)
add_engine_module(Objects)
add_engine_module(Project)
add_engine_module(Rendering)
add_engine_module(RenderManagers)
add_engine_module(Resources)
add_engine_module(UI)
add_engine_module(Utility)
add_engine_module(SystemModule)

# main executable file
file(GLOB_RECURSE MAIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# filters for main executable
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${MAIN_SOURCES}
)

add_executable(GameEngine ${MAIN_SOURCES})

# setup module external dependencies
target_link_libraries(Utility PUBLIC ExternalLibs)
target_link_libraries(SystemModule PUBLIC ExternalLibs)

target_link_libraries(Rendering PUBLIC ExternalLibs)
# add system OpenGL library to Rendering module
if (WIN32)
    target_link_libraries(Rendering PUBLIC opengl32)
elseif (APPLE)
    target_link_libraries(Rendering PUBLIC "-framework OpenGL")
else() # Linux and other
    find_package(OpenGL REQUIRED)
    target_link_libraries(Rendering PUBLIC OpenGL::GL)
endif()
# add glad.c to Rendering module
target_sources(Rendering
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Rendering/src/OpenGL/glad.c
)

# setup engine modules dependencies
target_link_libraries(Resources PUBLIC Utility)
target_link_libraries(Resources PUBLIC Rendering)

target_link_libraries(UI PUBLIC Resources)
target_link_libraries(UI PUBLIC SystemModule)

target_link_libraries(Objects PUBLIC UI)

target_link_libraries(RenderManagers PUBLIC Objects)

target_link_libraries(Project PUBLIC RenderManagers)

target_link_libraries(Editor PUBLIC Project)
target_link_libraries(Editor PUBLIC Objects)

target_link_libraries(CustomScripts PUBLIC Objects)

target_link_libraries(Core PUBLIC CustomScripts)
target_link_libraries(Core PUBLIC Editor)

target_link_libraries(GameEngine PRIVATE Core)

# add folders
add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:GameEngine>/Resources

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/TemporaryProjectsFolder
        $<TARGET_FILE_DIR:GameEngine>/TemporaryProjectsFolder
)