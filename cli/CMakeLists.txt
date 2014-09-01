add_executable(func-config-cli main.cc)

target_link_libraries(func-config-cli mouseutil)

install (TARGETS func-config-cli DESTINATION bin)
