cmake_minimum_required(VERSION 3.10)
project(EventBroker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definiciones necesarias para WebSocket++ sin Boost
add_definitions(-DASIO_STANDALONE -D_WEBSOCKETPP_CPP11_STL_ -DBOOST_ASIO_NO_DEPRECATED)

# Rutas de cabeceras
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/websocketpp
)

# Archivos fuente principales
add_executable(event_broker
    src/main.cpp
)

# Enlazamos pthread (necesario para WebSocket++)
target_link_libraries(event_broker pthread)


