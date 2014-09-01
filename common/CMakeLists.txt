add_library(mouseutil button_setting.cc
                      mouse_config_xml.cc
                      mouse_profile.cc
                      mouse_setup_interface.cc)

target_link_libraries(mouseutil      ${LibUSB1_LIBRARIES}
                                     ${TinyXML_LIBRARIES})
