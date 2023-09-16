cmake_minimum_required(VERSION 3.9)
project(project_daisy LANGUAGES CXX VERSION 0.0.1.0)
link_libraries(sfml-window sfml-graphics sfml-audio sfml-system)
add_executable(project_daisy main.cxx version.cxx resources.cxx prefs.cxx utils.cxx structs.cxx title.cxx)