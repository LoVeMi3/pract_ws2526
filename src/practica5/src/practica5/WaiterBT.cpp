//el WaiterBT.cpp

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"

#include "WaiterBT.hpp"
#include "bt_examples/bt_nodes/SayTextClientAction.hpp"
#include "bt_examples/bt_nodes/ListenTextClientAction.hpp"
#include "bt_examples/bt_nodes/ExtractInfoClientAction.hpp"

#include "hri_client/hri_client.hpp"
#include "bt_examples/bt_nodes/bt_node_registration.hpp"

