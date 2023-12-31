from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, TextSubstitution

from launch_ros.actions import Node


def generate_launch_description():
   background_r_launch_arg = DeclareLaunchArgument(
        'background_r', default_value=TextSubstitution(text='0')
   )
   background_g_launch_arg = DeclareLaunchArgument(
        'background_g', default_value=TextSubstitution(text='84')
   )
   background_b_launch_arg = DeclareLaunchArgument(
        'background_b', default_value=TextSubstitution(text='122')
   )
   
   return LaunchDescription([
      background_r_launch_arg,
      background_g_launch_arg,
      background_b_launch_arg,
      Node(
         # turtlesim 안에 있는 turtlesim_node를 ros2 run으로 실행시킨다.
         # Node의 이름은 sim으로 지정한다
         package='turtlesim',
         executable='turtlesim_node',
         name='sim',
         parameters=[{
            'background_r': LaunchConfiguration('background_r'),
            'background_g': LaunchConfiguration('background_g'),
            'background_b': LaunchConfiguration('background_b'),
         }]
      ),
   ])