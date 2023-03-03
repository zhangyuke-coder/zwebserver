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

<<<<<<< HEAD
add_executable(fiber_test1 fiber_test1.cc)
target_link_libraries(fiber_test1 zws_base)

add_executable(fiber_test2 fiber_test2.cc)
target_link_libraries(fiber_test2 zws_base)
=======
add_executable(coroutine_test coroutine_test.cc)
target_link_libraries(coroutine_test zws_base)
>>>>>>> 7e0298e329ef53ccd53862ed5d4e1a64a8aa6cb2


add_executable(scheduler_test scheduler_test.cc)
target_link_libraries(scheduler_test zws_base)