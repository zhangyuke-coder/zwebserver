add_executable(log_test log_test.cc)
target_link_libraries(log_test zws_base)

add_executable(env_test env_test.cc)
target_link_libraries(env_test zws_base)

add_executable(util_test util_test.cc)
target_link_libraries(util_test zws_base)

add_executable(config_test config_test.cc)
target_link_libraries(config_test zws_base)

add_executable(thread_test thread_test.cc)
target_link_libraries(thread_test zws_base)