import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np

class PositionPublisher(Node):
    def __init__(self):
        super().__init__('position_publisher')
        self.image_pub = self.create_publisher(Image, 'image', 10)
        self.bridge = CvBridge()

    def process_video(self, video_file):
        cap = cv2.VideoCapture(video_file)
        while rclpy.ok():
            ret, frame = cap.read()
            if ret:
                distance_to_center = self.calculate_distance_to_center(frame)
                if distance_to_center is not None:
                    self.get_logger().info('Ball distance to center: %f', distance_to_center)

                msg = self.bridge.cv2_to_imgmsg(frame, encoding='bgr8')
                self.image_pub.publish(msg)

            else:
                self.get_logger().info('End of video file reached')
                break

    def calculate_distance_to_center(self, frame):
        # 偵測球的位置
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blurred_frame = cv2.GaussianBlur(gray_frame, (5, 5), 0)
        edges = cv2.Canny(blurred_frame, 50, 150)
        contours, _ = cv2.findContours(edges.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if contours:
            max_contour = max(contours, key=cv2.contourArea)
            M = cv2.moments(max_contour)
            if M["m00"] != 0:
                ball_x = int(M["m10"] / M["m00"])
                ball_y = int(M["m01"] / M["m00"])
                center_x = frame.shape[1] // 2
                center_y = frame.shape[0] // 2

                distance_to_center = np.sqrt((ball_x - center_x)**2 + (ball_y - center_y)**2)
                cv2.circle(frame, (ball_x, ball_y), 10, (0, 255, 0), -1)
                
                return distance_to_center
        return None

def main(args=None):
    rclpy.init(args=args)

    try:
        publisher = PositionPublisher()
        video_file = "test.mp4"
        publisher.process_video(video_file)
    except KeyboardInterrupt:
        pass

    publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

