#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>

/**
 * @brief ROS 2 Node for Lane Detection using OpenCV.
 * Aligned with Resume: "Designing a computer vision module using OpenCV and CUDA"
 */
class LaneDetectorNode : public rclcpp::Node
{
public:
    LaneDetectorNode() : Node("lane_detector_node")
    {
        this->declare_parameter("use_sim", false);
        use_sim_ = this->get_parameter("use_sim").as_bool();
        lane_angle_pub_ = this->create_publisher<std_msgs::msg::Float32>("/lane/angle", 10);
        if (use_sim_) {
            RCLCPP_INFO(this->get_logger(), "🚀 Running in SIMULATION mode (Gazebo)");
            image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/image_raw", 10, std::bind(&LaneDetectorNode::imageCallback, this, std::placeholders::_1));
        } else {  // <--- ADD THIS ELSE
            // Timer to simulate camera processing loop
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(33), std::bind(&LaneDetectorNode::processFrame, this)); // 30 FPS
            RCLCPP_INFO(this->get_logger(), "📸 Lane Detector Node Started (OpenCV + CUDA Placeholder)");
        }
    }

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try {
            cv::Mat frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
            // TODO: Add real OpenCV lane detection here
            // For now, publish 0.0 just to prove we received the frame
            auto out_msg = std_msgs::msg::Float32();
            out_msg.data = 0.0f; 
            lane_angle_pub_->publish(out_msg);
        } catch (cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        }
    }
    void processFrame()
    {
        // 1. Grab image from Jetson Camera (Placeholder)
        // cv::Mat frame = grabFrame();

        // 2. Pre-process on GPU using CUDA (Placeholder for claim)
        // cv::cuda::GpuMat d_frame;
        // d_frame.upload(frame);
        
        // 3. Perform Lane Detection (Mock logic for now)
        float detected_angle = 5.2f; // Hypothetical angle

        auto msg = std_msgs::msg::Float32();
        msg.data = detected_angle;
        lane_angle_pub_->publish(msg);
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
    bool use_sim_;

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr lane_angle_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaneDetectorNode>());
    rclcpp::shutdown();
    return 0;
}
