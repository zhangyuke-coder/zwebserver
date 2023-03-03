
file(GLOB_RECURSE base_SRCS ./*.cc)
add_library(zws_base ${base_SRCS})
target_link_libraries(zws_base rt dl pthread  libyaml-cpp.so)