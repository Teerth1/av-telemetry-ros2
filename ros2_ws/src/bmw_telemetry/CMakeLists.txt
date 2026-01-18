cmake_minimum_required(VERSION 3.8)
project(bmw_telemetry)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(OpenCV REQUIRED)

# 1. BMW CAN Reader Node
add_executable(bmw_reader src/bmw_reader.cpp)
ament_target_dependencies(bmw_reader rclcpp std_msgs)

# 2. Lane Detector Node (OpenCV)
add_executable(lane_detector src/lane_detector_node.cpp)
ament_target_dependencies(lane_detector rclcpp std_msgs)
target_link_libraries(lane_detector ${OpenCV_LIBS})

# 3. Sensor Fusion Node
add_executable(fusion_node src/fusion_node.cpp)
ament_target_dependencies(fusion_node rclcpp std_msgs)

# Install targets
install(TARGETS 
  bmw_reader
  lane_detector
  fusion_node
  DESTINATION lib/${PROJECT_NAME})

# Install launch files
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
