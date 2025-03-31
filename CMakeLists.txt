if (USE_GUI)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    file(GLOB GUI_SOURCES "src/GUI/*.cpp" "ui/*.ui")
    add_executable(GestorBibliotecaGUI main.cpp ${SOURCES} ${GUI_SOURCES})
    target_link_libraries(GestorBibliotecaGUI Qt5::Widgets pq)
endif()

include_directories(
   ${CMAKE_SOURCE_DIR}/include
)