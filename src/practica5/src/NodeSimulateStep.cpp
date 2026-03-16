#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica5/NodeSimulateStep.hpp"
#include "nav2_example/simple_navigation_app.hpp"
#include "nav2_example/navigation_client.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "rclcpp_action/rclcpp.hpp"

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "bt_examples/say_text_client_action.hpp"
#include "bt_examples/text_utils.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

NodeSimulateStep::NodeSimulateStep() : Node("node_simulate_step")
{