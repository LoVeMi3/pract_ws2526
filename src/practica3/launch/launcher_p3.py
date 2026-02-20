# el launch.py de todos los nodos, incluyendo los nodos que están en otros directorios

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg_dir_cam = get_package_share_directory('camera')
    pkg_dir_laser = det_get_package_share_directory('practica3')

    param_file_cam = os.path.join(pkg_dir_cam, 'config', 'params.yaml')

    detect_img_cmd = Node(package='camera',
                        executable='hsv_filter_node',
                        output='screen',
                        parameters=[param_file_cam],
                        remappings=[
                          ('input_image', '/rgbd_camera/image'),
                          ('camera_info', '/rgbd_camera/camera_info'),
                          ('output_detection_2d', 'detection_2d'),
                        ])

    convert_2d_3d = Node(package='camera',
                        executable='detection_to_3d_from_depth_node',
                        output='screen',
                        parameters=[param_file_cam],
                        remappings=[
                          ('input_depth', '/rgbd_camera/depth/image_raw'),
                          ('input_detection_2d', 'detection_2d'),
                          ('camera_info', '/rgbd_camera/camera_info'),
                          ('output_detection_3d', 'detection_3d'),
                        ])

    detect_laser_cmd = Node(package='practica3',
                            executable='detect_obstacle',
                            output='screen',
                            parameters=[param_file],
                            remappings=[
                              ('input_scan', '/scan_raw')
                            ])

    ld = LaunchDescription()
    ld.add_action(detect_img_cmd)
    ld.add_action(convert_2d_3d)
    ld.add_action(detect_laser_cmd)

    return ld
