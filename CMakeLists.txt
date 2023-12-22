cmake_minimum_required(VERSION 3.9)
project(project_daisy LANGUAGES CXX VERSION 2.0.3.0)
link_libraries(sfml-window sfml-graphics sfml-audio sfml-system box2d)
add_executable(project_daisy main.cxx version.cxx resources.cxx prefs.cxx utils.cxx structs.cxx title.cxx jukebox.cxx game.cxx collision.cxx savescrn.cxx)