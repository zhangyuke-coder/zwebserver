
file(GLOB_RECURSE base_SRCS ./*.cc)
add_library(zws_base ${base_SRCS} ./coroutine/coctx_swap.S)
target_link_libraries(zws_base rt dl pthread  libyaml-cpp.so)