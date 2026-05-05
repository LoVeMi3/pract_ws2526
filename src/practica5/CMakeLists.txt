cmake_minimum_required(VERSION 3.8)
project(practica5)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)

find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

find_package(behaviortree_cpp REQUIRED)
find_package(hri_client REQUIRED) #hri_nodes

add_library(${PROJECT_NAME} SHARED
  src/practica5/DetectionNode.cpp
  src/practica5/WaiterBT.cpp
  src/practica5/GoToPoseAction.cpp
  src/practica5/GetClientPoseAction.cpp
)

get_filename_component(WS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)

find_library(BT_NODES_LIB bt_nodes
  PATHS "${WS_ROOT}/install/bt_examples/lib"
  NO_DEFAULT_PATH REQUIRED
)

set(BT_EXAMPLES_INCLUDE "${WS_ROOT}/src/ASR-Software/ch6_examples/bt_examples/include")

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${BT_EXAMPLES_INCLUDE}>
  $<INSTALL_INTERFACE:include>
  ${CMAKE_INSTALL_PREFIX}/../bt_examples/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ament_index_cpp::ament_index_cpp
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  behaviortree_cpp::behaviortree_cpp
  hri_client::hri_client
  tf2_ros::tf2_ros
  ${BT_NODES_LIB}
  ${geometry_msgs_TARGETS}
  ${std_msgs_TARGETS}
  ${nav2_msgs_TARGETS}
  ${sensor_msgs_TARGETS}
  ${tf2_geometry_msgs_TARGETS}
)

add_executable(detection src/detection_node_main.cpp)
target_link_libraries(detection ${PROJECT_NAME})

add_executable(waiter_bt src/waiter_bt_main.cpp)
target_link_libraries(waiter_bt ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
  nav2_msgs
  sensor_msgs
  behaviortree_cpp
  hri_client
  tf2_ros
  tf2_geometry_msgs
)

ament_package()
