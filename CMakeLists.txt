cmake_minimum_required(VERSION 3.20)
project(EventBroker)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(event_broker src/main.cpp) 

target_link_libraries(event_broker Boost::system)


# add_executable(logger_test src/logger_test.cpp)

#add_executable(logger_experiment src/logger_experiment.cpp)

#add_executable(logger_smartptr src/logger_smartptr.cpp)

