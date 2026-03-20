add_library(SparkLabsEngine STATIC
    scene/Node.h
    scene/Node.cpp
    scene/Component.h
    scene/Component.cpp
    scene/GameObject.h
    scene/GameObject.cpp
    scene/Scene.h
    scene/Scene.cpp
    scene/Transform.h
    scene/Transform.cpp
    resource/Resource.h
    resource/Resource.cpp
    resource/ResourceCache.h
    resource/ResourceCache.cpp
    resource/ResourceLoader.h
    resource/ResourceLoader.cpp
)

target_link_libraries(SparkLabsEngine PUBLIC
    SparkLabs::Core
)

target_include_directories(SparkLabsEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(SparkLabs::Engine ALIAS SparkLabsEngine)
