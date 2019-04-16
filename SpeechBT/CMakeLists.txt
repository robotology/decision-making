project(SpeechBT)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

find_package(BehaviorTree REQUIRED)

include_directories(./include)

add_executable(SpeechBT main.cpp src/speech_bt_nodes.cpp)

target_link_libraries(SpeechBT BehaviorTree::behaviortree_cpp_v3 )


