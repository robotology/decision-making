project(MoonshotBT)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

find_package(BehaviorTree REQUIRED)
find_package(YARP REQUIRED)


include_directories(./include)

add_executable(moonshot_behavior_tree main.cpp src/moonshot_bt_nodes.cpp)

target_link_libraries(moonshot_behavior_tree BehaviorTree::behaviortree_cpp_v3 ${YARP_LIBRARIES})


