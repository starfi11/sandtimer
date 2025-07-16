cmake_minimum_required(VERSION 3.15)
project(SandGlassTimer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE UIS ui/*.ui)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS})

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)
set_target_properties(SandGlassTimer PROPERTIES WIN32_EXECUTABLE TRUE)
