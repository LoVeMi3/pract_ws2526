cmake_minimum_required(VERSION 3.5)
project(practica2)

find_package(ament_cake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs/msg/Twist REQUIRED)
find_package(std_msgs/msg/Int32 REQUIRED)

add_library(${PROJECT_NAME}
  src/practica2/TeleOptNode.cpp
  src/teleopt_main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  rclcpp::rclcpp
  ${std_msgs_TARGETS}
  ${geometry_msgs_TARGETS}
)

add_executable(teleopt src/teleopt_main.cpp)
target_link_libraries(teleopt ${PROJECT_NAME})

add_executable(publisher src/publisher_main.cpp)
target_link_libraries(publisher ${PROJECT_NAME})

add_executable(subscriber src/subscriber_main.cpp)
target_link_libraries(subscriber ${PROJECT_NAME})

add_executable(pub_sub src/pub_sub_main.cpp)
target_link_libraries(pub_sub ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  teleopt
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
)

ament_package()
