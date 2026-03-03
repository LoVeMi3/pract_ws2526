cmake_minimum_required(VERSION 3.8)
project(practica3)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(vision_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/practica3/DetectObstacle.cpp
  src/practica3/DetectionNode3D.cpp
  src/practica3/OrientationNode.cpp
  src/practica3/PIDController.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  rclcpp::rclcpp
  tf2::tf2
  tf2_ros::tf2_ros
  ${geometry_msgs_TARGETS}
  ${tf2_geometry_msgs_TARGETS}
  ${std_msgs_TARGETS}
  ${sensor_msgs_TARGETS}
  ${vision_msgs_TARGETS}
)

add_executable(detectObs src/detect_obstacle_main.cpp)
target_link_libraries(detectObs ${PROJECT_NAME})

add_executable(detectNode3d src/detection_node_3d.cpp)
target_link_libraries(detectNode3d ${PROJECT_NAME})

add_executable(orientation src/orientation_node_main.cpp)
target_link_libraries(orientation ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  detectObs
  detectNode3d
  orientation
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
  std_msgs
  geometry_msgs
  sensor_msgs
  vision_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
)

ament_package()
