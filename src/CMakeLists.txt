set(CMAKE_CXX_STANDARD 26)

file(GLOB imgui_src
     "imgui/*.h"
     "imgui/*.cpp"
)

add_executable(WindowTest
    main.cc
    utils.hh

    shader.hh
    shader.cc

    shader_program.hh
    shader_program.cc

    ui.hh
    ui.cc
    
    camera.hh 
    camera.cc

    input.hh
    input.cc

    texture.hh
    texture.cc

    window.hh 
    window.cc

    stb_image.h
    
    "${CMAKE_SOURCE_DIR}/ext/glad/src/glad.c"
    ${imgui_src}
)

target_include_directories(WindowTest PRIVATE
    "${CMAKE_SOURCE_DIR}/ext/spdlog/include"
)
target_link_libraries(WindowTest PRIVATE
    "${CMAKE_SOURCE_DIR}/ext/spdlog/bin/spdlogd.lib"
)

target_include_directories(WindowTest PRIVATE
    "${CMAKE_SOURCE_DIR}/ext/glad/include"
    "${CMAKE_SOURCE_DIR}/ext/glfw/include"
    "${CMAKE_SOURCE_DIR}/ext/glm"
)

target_link_libraries(WindowTest PRIVATE
    "${CMAKE_SOURCE_DIR}/ext/glfw/bin/glfw3.lib"
)

set_target_properties(WindowTest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

add_custom_target(copy-shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
    ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/Shaders
)

add_dependencies(WindowTest copy-shaders)

add_custom_target(copy-textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Textures
    ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/Textures
)

add_dependencies(WindowTest copy-textures)

add_custom_target(copy-fonts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/fonts
)

add_dependencies(WindowTest copy-fonts)