//nodo detection 2d

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica3/DetectObstacle.hpp"

#include "geometry_msgs/msg/point_stamped.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "vision_msgs/msg/detection2D.hpp"
#include "std_msgs/msg/bool.hpp"

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

Detection2DNode::Detection2DNode()
: Node("detection_2D_node")
{
  image_sub_ = create_subscription<sensor_msgs::msg::Image>("input_img", rclcpp::SensorDataQoS().reliable(), std::bind(&Detection2DNode::image_callback, this, _1));
  vision_pub_ = create_publisher<vision_msgs::msg::PointStamped>("/nearest_obstacle", 100);

}

Detection2DNode::image_callback