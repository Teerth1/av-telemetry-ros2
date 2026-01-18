#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <cstring>
#include <unistd.h>

/**
 * @brief ROS 2 Node for reading BMW Telemetry via SocketCAN.
 * Aligned with Resume: "Architecting ROS 2 nodes in C++ to decode vehicle dynamics"
 */
class BmwTelemetryNode : public rclcpp::Node
{
public:
    BmwTelemetryNode() : Node("bmw_telemetry_reader")
    {
        // Declare parameters
        this->declare_parameter("interface", "can0");
        this->declare_parameter("use_sim", false);

        rpm_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/rpm", 10);
        speed_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/speed", 10);
        steering_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/steering", 10);

        std::string interface = this->get_parameter("interface").as_string();
        use_sim_ = this->get_parameter("use_sim").as_bool(); // Saved to member variable!

        if (use_sim_) {
            RCLCPP_INFO(this->get_logger(), "🚀 Running in SIMULATION mode (Isaac Sim Bridge)");
            // FIX 1: Start the timer in Sim mode too!
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(100), std::bind(&BmwTelemetryNode::readCanFrame, this));
        }
        else {
            if (initSocket(interface.c_str())) {
                RCLCPP_INFO(this->get_logger(), "✅ Connected to BMW CAN Bus (%s)", interface.c_str());
                timer_ = this->create_wall_timer(
                    std::chrono::milliseconds(10), std::bind(&BmwTelemetryNode::readCanFrame, this));
            } else {
                RCLCPP_ERROR(this->get_logger(), "❌ Failed to connect to %s. Ensure SocketCAN is up.", interface.c_str());
            }
        }
    }

    ~BmwTelemetryNode() {
        if (sock_ >= 0) close(sock_);
    }

private:
    int sock_;
    bool use_sim_; // FIX 2: Added member variable so the Loop can see it
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr rpm_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr speed_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr steering_pub_;
    rclcpp::TimerBase::SharedPtr timer_;

    bool initSocket(const char *ifname)
    {
        struct sockaddr_can addr;
        struct ifreq ifr;

        sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (sock_ < 0) return false;

        std::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
        if (ioctl(sock_, SIOCGIFINDEX, &ifr) < 0) return false;

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(sock_, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;
        
        return true;
    }

    void readCanFrame()
    {
        // FIX 3: Simulation Logic
        if (use_sim_) {
            // Create fake data for simulation
            static float fake_angle = 0.0;
            fake_angle += 0.5;
            if (fake_angle > 360.0) fake_angle = 0.0;

            publishSteering(fake_angle);
            RCLCPP_INFO(this->get_logger(), "[SIM] Steering: %.2f deg", fake_angle);
            return; // Exit function so we don't try to read from the dead socket
        }

        // Real Hardware Logic
        struct can_frame frame;
        int nbytes = read(sock_, &frame, sizeof(struct can_frame));

        if (nbytes < 0) return;

        if (frame.can_id == 0x7E8) { 
            uint8_t pid = frame.data[2];
            if (pid == 0x0C) { 
                float rpm = ((frame.data[3] * 256.0) + frame.data[4]) / 4.0;
                publishRpm(rpm);
            }
            else if (pid == 0x0D) { 
                float speed_kmh = (float)frame.data[3]; 
                publishSpeed(speed_kmh);
            }
        }
        else if (frame.can_id == 0x0C2) { 
            int16_t raw_steering = (frame.data[0] << 8) | frame.data[1];
            float steering_angle = raw_steering * 0.1; 
            publishSteering(steering_angle);
        }
    }

    void publishRpm(float rpm) {
        auto msg = std_msgs::msg::Float32();
        msg.data = rpm;
        rpm_pub_->publish(msg);
    }

    void publishSpeed(float speed) {
        auto msg = std_msgs::msg::Float32();
        msg.data = speed;
        speed_pub_->publish(msg);
    }

    void publishSteering(float angle) {
        auto msg = std_msgs::msg::Float32();
        msg.data = angle;
        steering_pub_->publish(msg);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<BmwTelemetryNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}