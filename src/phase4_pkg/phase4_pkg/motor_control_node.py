import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import serial
from rclpy.qos import QoSProfile,ReliabilityPolicy,HistoryPolicy,DurabilityPolicy

class MotorControlNode(Node):
    def __init__(self):
        super().__init__('motor_control_node')

        qos=QoSProfile(
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.VOLATILE
        )

        self.sub=self.create_subscription(
            Twist,
            '/cmd_vel',
            self.cmd_callback,
            qos
        )

        self.serial_port='/dev/ttyACM0'
        self.baudrate=115200

        try:
            self.ser=serial.Serial(
                self.serial_port,
                self.baudrate,
                timeout=1
            )

            self.get_logger().info('ESP32 serial connected')

        except Exception as e:
            self.get_logger().error(f'Serial connection failed: {e}')

        self.get_logger().info('Motor control node started')

    def cmd_callback(self,msg):
        linear=msg.linear.x
        angular=msg.angular.z

        left_speed=0
        right_speed=0

        if linear>0:
            left_speed=150
            right_speed=150

        if angular>0:
            left_speed=-120
            right_speed=120

        elif angular<0:
            left_speed=120
            right_speed=-120

        if linear==0.0 and angular==0.0:
            left_speed=0
            right_speed=0

        command=f'{left_speed},{right_speed}\n'

        try:
            self.ser.write(command.encode())

        except Exception as e:
            self.get_logger().error(f'Serial write failed: {e}')

def main(args=None):
    rclpy.init(args=args)

    node=MotorControlNode()

    try:
        rclpy.spin(node)

    except KeyboardInterrupt:
        pass

    finally:
        if hasattr(node, 'ser') and node.ser:

            try:

                node.ser.write(b'0,0\n')
                node.ser.flush()

            except Exception as e:

                node.get_logger().error(
                    f'Failed to send stop command: {e}'
                )

        if rclpy.ok():
            node.destroy_node()
            rclpy.shutdown()

if __name__=='__main__':
    main()