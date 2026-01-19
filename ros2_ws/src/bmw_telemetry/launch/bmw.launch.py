import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    pkg_bmw = get_package_share_directory('bmw_telemetry')
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')
    
    # Argument: use_sim (default to false)
    use_sim = LaunchConfiguration('use_sim', default='false')

    # 1. Gazebo Launch (Only if use_sim is true)
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gazebo.launch.py')
        ),
    )

    # 2. Spawn Robot (URDF)
    # We need to find the urdf file we just made
    urdf_file = os.path.join(pkg_bmw, 'urdf', 'vehicle.urdf')
    
    spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-entity', 'bmw_car', '-file', urdf_file],
        output='screen'
    )

    # 3. Our Nodes
    bmw_reader = Node(
        package='bmw_telemetry',
        executable='bmw_reader',
        name='bmw_reader',
        output='screen',
        parameters=[{'use_sim': use_sim}]
    )
    
    lane_detector = Node(
        package='bmw_telemetry',
        executable='lane_detector',
        name='lane_detector',
        output='screen',
        parameters=[{'use_sim': use_sim}]
    )

    fusion = Node(
        package='bmw_telemetry',
        executable='fusion_node',
        name='fusion_node',
        output='screen'
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim',
            default_value='false',
            description='Use Simulation (Gazebo) or Hardware (CAN)'),
        
        # We include Gazebo infrastructure. 
        # If use_sim is false, the nodes ignore Gazebo topics, but Gazebo might still open.
        # This is fine for a simple hybrid setup.
        gazebo,
        spawn_entity,
        
        bmw_reader,
        lane_detector,
        fusion
    ])