cmake_minimum_required(VERSION 3.8)
project(practica3)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
#find_package(clicked_point REQUIRED)
find_package(kobuki_ros_interfaces REQUIRED)

add_library(${PROJECT_NAME}
  src/practica3/DetectObstacle.cpp
  #src/practica3/DetectionNode3D.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

#camera/DetectionTo3DfromDepthNode.hpp
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  std_msgs
  geometry_msgs
  sensor_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  #clicked_point
  kobuki_ros_interfaces
)

add_executable(detectObs src/detect_obstacle_main.cpp)
#add_executable(detect3D src/detection_node_3D_main.cpp)
target_link_libraries(detectObs ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  detectObs
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

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
  std_msgs
  geometry_msgs
  sensor_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  #clicked_point
  kobuki_ros_interfaces
)

ament_package()