find_package(Qt5Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(qt_gui_sources main.cc
                   button_setting_widget.cc
                   color_button.cc
                   dpi_widget.cc
                   key_input_widget.cc
                   led_widget.cc
                   macro_input_widget.cc
                   mainwindow.cc
                   mouse_application.cc
                   profile_widget.cc
                   profiles_tab_widget.cc
                   mouse_profile_worker.cc)

add_executable(func-config-qt ${qt_gui_sources})
qt5_use_modules(func-config-qt Widgets)
target_link_libraries(func-config-qt mouseutil)

install (TARGETS func-config-qt DESTINATION bin)
