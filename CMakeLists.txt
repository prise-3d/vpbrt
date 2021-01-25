cmake_minimum_required (VERSION 2.6)
project (vpbrt)
set(SRCS
    ./src/rply-1.1.4/rply.c
    ./src/Geometry/Mesh.cpp
    ./src/Geometry/Cylindre.cpp
    ./src/Geometry/Scene.cpp
    ./src/vpbrt.cpp
    ./src/graphique.cpp
    ./src/interactions.cpp
    ./src/IO/PbrtLoader.cpp
    ./src/Geometry/transform.cpp
    ./src/IO/PlyLoader.cpp
    ./src/Camera.cpp
    ./src/Chemin.cpp
    ./src/Geometry/Objet.cpp
    ./src/Materials/Material.cpp
    ./src/Materials/Glass.cpp
    ./src/Materials/Matte.cpp
    ./src/Materials/Metal.cpp
   ./src/Materials/Mirror.cpp
   ./src/Materials/Plastic.cpp
    ./src/Materials/Uber.cpp
    ./src/Materials/ParamSet.cpp
    )
    
set(HEADERS
    ./src/rply-1.1.4/rply.h
    ./src/rply-1.1.4/rplyfile.h
    ./src/Geometry/Mesh.hpp
    ./src/Geometry/Cylindre.hpp
    ./src/Geometry/Scene.hpp
    ./src/graphique.hpp
    ./src/interactions.hpp
    ./src/IO/PbrtLoader.hpp
    ./src/Geometry/transform.cpp
    ./src/IO/PlyLoader.cpp
    ./src/Camera.hpp
    ./src/Chemin.hpp
    ./src/Geometry/Objet.hpp
    ./src/Materials/Material.hpp
    ./src/Materials/Glass.hpp
    ./src/Materials/Matte.hpp
    ./src/Materials/Metal.hpp
    ./src/Materials/Mirror.hpp
    ./src/Materials/Plastic.hpp
    ./src/Materials/Uber.cpp
    ./src/Materials/ParamSet.hpp
   )

add_definitions(-std=c++11)

add_executable(vpbrt ${SRCS} ${HEADERS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(vpbrt ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
