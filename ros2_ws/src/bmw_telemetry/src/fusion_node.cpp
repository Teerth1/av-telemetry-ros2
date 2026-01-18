#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>

/**
 * @brief ROS 2 Node for Sensor Fusion.
 * Aligned with Resume: "fuse visual data with vehicle odometry"
 */
class FusionNode : public rclcpp::Node
{
public:
    FusionNode() : Node("fusion_node")
    {
        fused_steering_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/fused_steering", 10);

        can_sub_ = this->create_subscription<std_msgs::msg::Float32>(
            "/vehicle/steering", 10, std::bind(&FusionNode::canCallback, this, std::placeholders::_1));

        lane_sub_ = this->create_subscription<std_msgs::msg::Float32>(
            "/lane/angle", 10, std::bind(&FusionNode::laneCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "🧠 Sensor Fusion Node Started (CAN + CV)");
    }

private:
    void canCallback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        last_can_angle_ = msg->data;
        fuseAndPublish();
    }

    void laneCallback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        last_lane_angle_ = msg->data;
        fuseAndPublish();
    }

    void fuseAndPublish()
    {
        // Simple Weighted Fusion (Odometry 80%, CV 20%)
        float fused = (last_can_angle_ * 0.8f) + (last_lane_angle_ * 0.2f);
        
        auto msg = std_msgs::msg::Float32();
        msg.data = fused;
        fused_steering_pub_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr fused_steering_pub_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr can_sub_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr lane_sub_;

    float last_can_angle_ = 0.0f;
    float last_lane_angle_ = 0.0f;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FusionNode>());
    rclcpp::shutdown();
    return 0;
}
