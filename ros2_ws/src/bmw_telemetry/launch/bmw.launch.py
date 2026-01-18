from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 1. CAN Reader Node
        Node(
            package='bmw_telemetry',
            executable='bmw_reader',
            name='bmw_reader_node',
            output='screen',
            parameters=[{'interface': 'can0', 'use_sim': False}]
        ),
        # 2. Lane Detector Node
        Node(
            package='bmw_telemetry',
            executable='lane_detector',
            name='lane_detector_node',
            output='screen'
        ),
        # 3. Fusion Node
        Node(
            package='bmw_telemetry',
            executable='fusion_node',
            name='fusion_node',
            output='screen'
        )
    ])
