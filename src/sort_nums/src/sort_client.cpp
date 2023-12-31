#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/srv/sort_numbers.hpp"
#include <iostream>
#include <memory>
#include <vector>

using SortNumbers = tutorial_interfaces::srv::SortNumbers;
rclcpp::Node::SharedPtr g_node = nullptr;

void sort_numbers(std::vector<int> numbers) {
  auto client = g_node->create_client<SortNumbers>("sort_numbers");

  while (!client->wait_for_service(std::chrono::seconds(1))) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(g_node->get_logger(), "Interrupted while waiting for the service. Exiting.");
      return;
    }
    RCLCPP_INFO(g_node->get_logger(), "Waiting for service to appear...");
  }

  auto request = std::make_shared<SortNumbers::Request>();

  std_msgs::msg::Int32MultiArray array_msg;
  array_msg.data = numbers;
  request->numbers_to_sort = array_msg;

  auto result = client->async_send_request(request);

  if (rclcpp::spin_until_future_complete(g_node, result) == rclcpp::FutureReturnCode::SUCCESS) {
    std::cout << "Sorted numbers: ";
    std_msgs::msg::Int32MultiArray recieved_array_msg;
    recieved_array_msg = result.get()->sorted_numbers;
    for (auto number : recieved_array_msg.data) {
      std::cout << number << " ";
    }
    std::cout << std::endl;
  } else {
    RCLCPP_ERROR(g_node->get_logger(), "Failed to call service sort_numbers");
  }
}

std::vector<int> store_vector(){
  std::vector<int> vec;
  int input;
  std::cout << "Enter the elements: ";
  while (std::cin >> input) {
    vec.push_back(input);
  }

  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "Entered: ";
  for (int elem : vec){
    std::cout << elem << " ";
  }
  return vec;
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  g_node = rclcpp::Node::make_shared("sort_client");
  sort_numbers(store_vector());

  rclcpp::shutdown();
  return 0;
}
