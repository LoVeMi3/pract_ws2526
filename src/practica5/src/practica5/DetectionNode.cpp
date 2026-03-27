//el DetectionNode.cpp

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>

#include "practica5/WaiterBT.hpp"
#include "practica5/GoToPoseAction.hpp"
#include "practica5/WaitForPersonAction.hpp"
#include "practica5/DetectionNode.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"

#include "rclcpp/rclcpp.hpp"

// executable='detection' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

DetectionNode::DetectionNode()
: Node("detection_node"), tf_buffer_(this->get_clock())
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  laser_sub_ = create_subscription<sensor_msgs::msg::LaserScan>("input_scan", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectionNode::laser_callback, this, std::placeholders::_1));
  person_pub_ = create_publisher<std_msgs::msg::Bool>("/person_detected", 10);
}

void
DetectionNode::laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan)
{
  std_msgs::msg::Bool person_msg;

  float min_range = std::numeric_limits<float>::infinity();
  int min_index = -1;

  for (size_t i = 0; i < scan->ranges.size(); ++i) {
    float r = scan->ranges[i];

    if (!std::isfinite(r)) {
      continue;
    }
    if (r < scan->range_min || r > scan->range_max) {
      continue;
    }
    if (r < min_range) {
      min_range = r;
      min_index = i;
    }
  }

  if (min_index == -1) {
    person_msg.data = false;
  } else {
    person_msg.data = (min_range < min_distance_);
  }

  person_pub_->publish(person_msg);
}

} //namespace practica5