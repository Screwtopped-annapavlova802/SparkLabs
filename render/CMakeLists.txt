add_library(SparkLabsRender STATIC
    RenderDevice.h
    RenderContext.h
    RenderContext.cpp
    Shader.h
    Shader.cpp
    ShaderProgram.h
    ShaderProgram.cpp
    Mesh.h
    Mesh.cpp
    Material.h
    Material.cpp
    Texture.h
    Texture.cpp
    gpu/CMakeLists.txt
)

target_link_libraries(SparkLabsRender PUBLIC
    SparkLabs::Core
)

target_include_directories(SparkLabsRender PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(SparkLabs::Render ALIAS SparkLabsRender)
