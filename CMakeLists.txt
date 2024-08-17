cmake_minimum_required(VERSION 3.8...3.20)

project(fiber_performance)

find_package(Boost REQUIRED ALL)

#detach
add_executable(detach src/skynet_detach.cpp)
target_link_libraries(detach PUBLIC Boost::fiber)

#join
add_executable(join src/skynet_join.cpp)
target_link_libraries(join PUBLIC Boost::fiber)

#stealing_async

add_executable(stealing_async src/skynet_stealing_async.cpp)
target_link_libraries(stealing_async PUBLIC Boost::fiber)

#stealing_detach

add_executable(stealing_detach src/skynet_stealing_detach.cpp)
target_link_libraries(stealing_detach PUBLIC Boost::fiber)

#stealing_join

add_executable(stealing_join src/skynet_stealing_join.cpp)
target_link_libraries(stealing_join PUBLIC Boost::fiber)
