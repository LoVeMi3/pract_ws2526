# el launch.py de todos los nodos, incluyendo los nodos que están en otros directorios

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    pkg_dir_laser = get_package_share_directory('practica5')

    param_file_cam = os.path.join(pkg_dir_cam, 'config', 'params.yaml')

    nav2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('nav2_bringup'),
                'launch',
                'bringup_launch.py'
            )
        ),
        launch_arguments={
            'map': os.path.join(
                get_package_share_directory('practica5'),
                'map',
                'aws_bookstore.yaml'
            ),
            'use_sim_time': 'true'
        }.items()
    )

    detection_cmd = Node(package='practica5',
                            executable='detection',
                            output='screen',
                            remappings=[
                              ('input_scan', '/scan_raw')
                            ])

    waiter_cmd = Node(package='practica5',
                        executable='waiter_bt',
                        output='screen',
                        )

    ld = LaunchDescription()
    ld.add_action(detection_cmd)
    ld.add_action(waiter_cmd)
    ld.add_action(nav2_launch)

    return ld
