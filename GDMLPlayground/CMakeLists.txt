cmake_minimum_required(VERSION 3.16...3.21)
project(GDMLViewer)

find_package(Geant4 REQUIRED vis_all)

add_executable(gdml_viewer gdml.cc)
target_link_libraries(gdml_viewer PRIVATE ${Geant4_LIBRARIES})
target_include_directories(gdml_viewer PRIVATE ${Geant4_INCLUDE_DIRS})

# Optional: place macro files in build dir
file(COPY vis.mac DESTINATION ${CMAKE_BINARY_DIR})
