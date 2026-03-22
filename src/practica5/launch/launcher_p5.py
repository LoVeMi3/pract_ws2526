# el launch.py de todos los nodos, incluyendo los nodos que están en otros directorios

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    pkg_dir_cam = get_package_share_directory('camera')
    pkg_dir_laser = get_package_share_directory('practica3')

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

    detect_img_cmd = Node(package='camera',
                        executable='hsv_filter',
                        output='screen',
                        parameters=[param_file_cam],
                        remappings=[
                          ('input_image', '/rgbd_camera/image'),
                          ('camera_info', '/rgbd_camera/camera_info'),
                          ('output_detection_2d', 'detection_2d'),
                        ])

    convert_2d_3d = Node(package='camera',
                        executable='detection_2d_to_3d_depth',
                        output='screen',
                        parameters=[param_file_cam],
                        remappings=[
                          ('input_depth', '/rgbd_camera/depth/image_raw'),
                          ('input_detection_2d', 'detection_2d'),
                          ('camera_info', '/rgbd_camera/camera_info'),
                          ('output_detection_3d', 'detection_3d'),
                        ])

    detection_cmd = Node(package='practica5',
                            executable='detection',
                            output='screen',
                            remappings=[
                              ('input_scan', '/scan_raw')
                            ])

    waiter_cmd = Node(package='practica5',
                        executable='waiter',
                        output='screen',
                        remappings=[
                          ('input_depth', '/rgbd_camera/depth/image_raw'),
                          ('input_detection_2d', 'detection_2d'),
                          ('camera_info', '/rgbd_camera/camera_info'),
                          ('output_detection_3d', 'detection_3d')
                        ])

    ld = LaunchDescription()
    ld.add_action(detection_cmd)
    ld.add_action(waiter_cmd)
    ld.add_action(detect_img_cmd)
    ld.add_action(convert_2d_3d)
    ld.add_action(nav2_launch)

    return ld
