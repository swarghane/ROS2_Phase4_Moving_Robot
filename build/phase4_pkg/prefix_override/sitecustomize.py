import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/swarghane/ros2_learning_phases/ros2_phase4/install/phase4_pkg'
