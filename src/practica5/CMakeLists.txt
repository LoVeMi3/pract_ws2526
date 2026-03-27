cmake_minimum_required(VERSION 3.8)
project(practica5)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(bt_examples REQUIRED)

find_package(nav2_example REQUIRED)
find_package(fsm_examples REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/DetectionNode.cpp
  src/WaiterBT.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  nav2_example::nav2_example
  fsm_examples::fsm_examples
  bt_examples::bt_examples
  ${geometry_msgs_TARGETS}
  ${std_msgs_TARGETS}
  ${nav2_msgs_TARGETS}
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
  nav2_example
  fsm_examples
  bt_examples
)

ament_package()
