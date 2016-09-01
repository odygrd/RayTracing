file(GLOB SOURCE_FILES
        core/*.h
        core/*.cpp
        core/lights/*.h
        core/lights/*.cpp
        core/materials/*.h
        core/materials/*.cpp
        core/meshes/*.h
        core/meshes/*.cpp
        cameras/*.h
        cameras/*.cpp
        math/*.h
        math/*.cpp
        xml/*.h
        xml/*.cpp
        main.cpp
        )

add_executable(Raytracer ${SOURCE_FILES})
target_link_libraries (Raytracer LINK_PUBLIC tinyxml2)