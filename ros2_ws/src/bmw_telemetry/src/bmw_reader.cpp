/**
 * ============================================================================
 *                         BMW TELEMETRY READER NODE
 * ============================================================================
 * 
 * This file demonstrates key C++ concepts while building a ROS 2 node.
 * Each section is annotated with the C++ topics it teaches.
 * 
 * C++ TOPICS COVERED:
 * -------------------
 * 1. #include & Header Files       - Lines 25-35
 * 2. Classes & Inheritance         - Lines 50-75
 * 3. Constructors & Initializer Lists - Lines 80-95
 * 4. Access Modifiers (public/private) - Lines 100-120
 * 5. Pointers & Smart Pointers     - Lines 130-155
 * 6. The 'this' Keyword            - Lines 160-175
 * 7. References (&)                - Lines 180-195
 * 8. Templates (<T>)               - Lines 200-220
 * 9. Lambda Functions & std::bind  - Lines 230-260
 * 10. Destructors (~)              - Lines 270-285
 * 11. Static Variables             - Lines 290-305
 * 12. Type Casting                 - Lines 310-325
 * 13. Bitwise Operations           - Lines 330-355
 * ============================================================================
 */

// =============================================================================
// TOPIC 1: #include & Header Files
// =============================================================================
/*
 * In C++, #include brings in external code libraries.
 * 
 * <angle_brackets> = System/library headers (the compiler searches system paths)
 * "quotes"         = Your own headers (searches current directory first)
 * 
 * Think of it like Java's 'import', but it literally copy-pastes the code!
 * 
 * EXAMPLE:
 * --------
 * #include <iostream>    // System library for console I/O (like System.out.println)
 * #include "my_class.h"  // Your own header file
 */

#include <rclcpp/rclcpp.hpp>           // ROS 2 C++ client library
#include <std_msgs/msg/float32.hpp>    // Standard message type for floats
#include <nav_msgs/msg/odometry.hpp>   // Navigation message for robot position/velocity

// Linux-specific headers for CAN bus communication
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <cstring>    // C-style string functions (strncpy, memset)
#include <unistd.h>   // POSIX API (read, write, close)

// =============================================================================
// TOPIC 2: Classes & Inheritance
// =============================================================================
/*
 * WHAT IS A CLASS?
 * A class is a blueprint for creating objects. It bundles data (variables) 
 * and functions together.
 * 
 * Java Comparison:
 * ----------------
 * Java:   class Dog extends Animal { }
 * C++:    class Dog : public Animal { }
 * 
 * WHAT IS INHERITANCE?
 * Our class "inherits" from rclcpp::Node, meaning we get all of Node's 
 * functions and variables for free!
 * 
 * The '::' is the SCOPE RESOLUTION OPERATOR
 * - rclcpp::Node means "the Node class inside the rclcpp namespace"
 * - Like Java's package system: rclcpp.Node
 * 
 * EXAMPLE:
 * --------
 * class MyNode : public rclcpp::Node {
 *     // Now MyNode has access to create_publisher(), get_logger(), etc.
 * };
 */
class BmwTelemetryNode : public rclcpp::Node
{
// =============================================================================
// TOPIC 4: Access Modifiers (public/private)
// =============================================================================
/*
 * public:  Anyone can access these members
 * private: Only this class can access these members
 * protected: This class + child classes can access (we don't use it here)
 * 
 * Java Comparison:
 * ----------------
 * Same concept! But in C++, you declare regions instead of per-variable.
 * 
 * EXAMPLE:
 * --------
 * class Car {
 * public:
 *     void drive() { }       // Anyone can call this
 * private:
 *     int engineTemp_;       // Only Car's functions can access this
 * };
 */
public:

// =============================================================================
// TOPIC 3: Constructors & Initializer Lists
// =============================================================================
/*
 * A CONSTRUCTOR is a special function that runs when an object is created.
 * 
 * The INITIALIZER LIST (after the colon) is the C++ way to initialize 
 * member variables BEFORE the constructor body runs.
 * 
 * Java Comparison:
 * ----------------
 * Java:   public MyClass() { super("name"); this.x = 5; }
 * C++:    MyClass() : ParentClass("name"), x_(5) { }
 * 
 * WHY USE INITIALIZER LISTS?
 * 1. Required for calling parent class constructors
 * 2. More efficient for complex objects
 * 3. Required for const and reference members
 * 
 * EXAMPLE:
 * --------
 * class Student {
 *     std::string name_;
 *     int age_;
 * public:
 *     Student(std::string n, int a) : name_(n), age_(a) { 
 *         // name_ and age_ are already set before we get here!
 *     }
 * };
 */
    BmwTelemetryNode() : Node("bmw_telemetry_reader")  // <-- Initializer list: calls parent constructor
    {
// =============================================================================
// TOPIC 6: The 'this' Keyword
// =============================================================================
/*
 * 'this' is a POINTER to the current object instance.
 * 
 * In C++, we use -> (arrow operator) to access members through a pointer.
 * 
 * Java Comparison:
 * ----------------
 * Java:   this.doSomething()     (dot operator)
 * C++:    this->doSomething()    (arrow operator, because 'this' is a pointer)
 * 
 * WHY IS 'this' A POINTER?
 * Historical reason: C++ evolved from C, where pointers are fundamental.
 * 
 * EXAMPLE:
 * --------
 * void setName(std::string name) {
 *     this->name_ = name;  // Disambiguate: member 'name_' vs parameter 'name'
 * }
 */
        // Declare ROS 2 parameters (like command-line arguments)
        this->declare_parameter("interface", "can0");
        this->declare_parameter("use_sim", false);

// =============================================================================
// TOPIC 5: Pointers & Smart Pointers
// =============================================================================
/*
 * WHAT IS A POINTER?
 * A pointer stores the MEMORY ADDRESS of another variable.
 * 
 * Regular Pointer:      int* ptr = &myInt;    // Manual memory management
 * Smart Pointer:        std::shared_ptr<int>  // Automatic memory management
 * 
 * Java Comparison:
 * ----------------
 * Java doesn't have explicit pointers, but all objects are references.
 * Smart pointers are like Java references with garbage collection.
 * 
 * SharedPtr = Multiple owners, deleted when last owner is gone
 *             (like Java references with garbage collection!)
 * 
 * EXAMPLE:
 * --------
 * // Raw pointer (dangerous, you must remember to delete)
 * int* rawPtr = new int(42);
 * delete rawPtr;  // You MUST do this or memory leaks!
 * 
 * // Smart pointer (safe, automatic cleanup)
 * std::shared_ptr<int> smartPtr = std::make_shared<int>(42);
 * // No delete needed! Automatically cleaned up when no longer used.
 */

// =============================================================================
// TOPIC 8: Templates (<T>)
// =============================================================================
/*
 * TEMPLATES let you write code that works with ANY type.
 * The <> brackets specify the type parameter.
 * 
 * Java Comparison:
 * ----------------
 * Java:   List<String> myList = new ArrayList<>();
 * C++:    std::vector<std::string> myVec;
 * 
 * create_publisher<std_msgs::msg::Float32> means:
 * "Create a publisher that sends Float32 messages"
 * 
 * EXAMPLE:
 * --------
 * // Template function that works with any type
 * template<typename T>
 * T add(T a, T b) {
 *     return a + b;
 * }
 * 
 * int x = add<int>(5, 3);        // Returns 8
 * double y = add<double>(1.5, 2.5);  // Returns 4.0
 */
        // Create publishers for vehicle data
        // <std_msgs::msg::Float32> is the template parameter - the MESSAGE TYPE
        rpm_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/rpm", 10);
        speed_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/speed", 10);
        steering_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/steering", 10);

// =============================================================================
// TOPIC 7: References (&)
// =============================================================================
/*
 * A REFERENCE is an alias (another name) for an existing variable.
 * Unlike pointers, references CANNOT be null and CANNOT be reassigned.
 * 
 * Key Difference:
 * - Pointer: int* ptr = &x;  // ptr holds the ADDRESS of x
 * - Reference: int& ref = x; // ref IS x (same memory location)
 * 
 * Java Comparison:
 * ----------------
 * Java object variables are essentially references (but you can't 
 * control pass-by-value vs pass-by-reference like in C++).
 * 
 * WHEN TO USE REFERENCES:
 * - Pass large objects to functions without copying (efficiency)
 * - Allow a function to modify the original variable
 * - Use const& for read-only access to large objects
 * 
 * EXAMPLE:
 * --------
 * void doubleIt(int& num) {  // 'num' is a reference to the original
 *     num = num * 2;         // This modifies the ORIGINAL variable!
 * }
 * 
 * int myNum = 5;
 * doubleIt(myNum);           // myNum is now 10
 */
        // Get parameter values - as_string() returns a std::string by value
        std::string interface = this->get_parameter("interface").as_string();
        use_sim_ = this->get_parameter("use_sim").as_bool();

// =============================================================================
// TOPIC 9: Lambda Functions & std::bind
// =============================================================================
/*
 * LAMBDA: An anonymous (unnamed) function defined inline.
 * std::bind: Connects a member function to a callback system.
 * 
 * Java Comparison:
 * ----------------
 * Java:   button.onClick(() -> { System.out.println("Clicked!"); });
 * C++:    button.onClick([]() { std::cout << "Clicked!" << std::endl; });
 * 
 * Lambda Syntax:
 * [capture](parameters) { body }
 * 
 * [capture] = What variables from outside to "capture" (bring into the lambda)
 *   []     = Capture nothing
 *   [this] = Capture the current object
 *   [&]    = Capture everything by reference
 *   [=]    = Capture everything by copy
 * 
 * std::bind Syntax:
 * std::bind(&ClassName::functionName, objectPointer, placeholders...)
 * 
 * EXAMPLE:
 * --------
 * // Lambda with capture
 * int multiplier = 3;
 * auto tripler = [multiplier](int x) { return x * multiplier; };
 * int result = tripler(5);  // result = 15
 * 
 * // std::bind example
 * void myCallback(int value) { std::cout << value; }
 * auto bound = std::bind(myCallback, std::placeholders::_1);
 */
        if (use_sim_) {
            RCLCPP_INFO(this->get_logger(), "🚀 Running in SIMULATION mode (Gazebo)");
            
            // Create subscription using std::bind
            // std::bind connects the odomCallback function to this object instance
            // std::placeholders::_1 means "the first argument will be passed through"
            odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
                "/odom", 
                10,  // Queue size
                std::bind(&BmwTelemetryNode::odomCallback, this, std::placeholders::_1)
            );
        }
        else {
            // Initialize the CAN socket for real hardware
            if (initSocket(interface.c_str())) {
                RCLCPP_INFO(this->get_logger(), "✅ Connected to BMW CAN Bus (%s)", interface.c_str());
                
                // Create a timer that calls readCanFrame every 10ms
                timer_ = this->create_wall_timer(
                    std::chrono::milliseconds(10),
                    std::bind(&BmwTelemetryNode::readCanFrame, this)
                );
            } else {
                RCLCPP_ERROR(this->get_logger(), "❌ Failed to connect to %s", interface.c_str());
            }
        }
    }

// =============================================================================
// TOPIC 10: Destructors (~)
// =============================================================================
/*
 * A DESTRUCTOR is a special function that runs when an object is destroyed.
 * Named with a tilde (~) prefix: ~ClassName()
 * 
 * Java Comparison:
 * ----------------
 * Java has finalize() (deprecated) or try-with-resources.
 * C++ destructors are DETERMINISTIC - they run exactly when the object 
 * goes out of scope, not "sometime later" like Java's garbage collector.
 * 
 * USE DESTRUCTORS FOR:
 * - Closing files
 * - Releasing memory
 * - Closing network connections
 * - Any cleanup that MUST happen
 * 
 * EXAMPLE:
 * --------
 * class FileHandler {
 *     FILE* file_;
 * public:
 *     FileHandler(const char* name) { file_ = fopen(name, "r"); }
 *     ~FileHandler() { 
 *         if (file_) fclose(file_);  // Guaranteed to run!
 *     }
 * };
 */
    ~BmwTelemetryNode() {
        // Clean up: close the socket when the node is destroyed
        if (sock_ >= 0) {
            close(sock_);
        }
    }

private:
    // ==========================================================================
    // MEMBER VARIABLES (with naming convention: trailing underscore)
    // ==========================================================================
    /*
     * NAMING CONVENTION:
     * Member variables often end with underscore (sock_, use_sim_) to 
     * distinguish them from local variables and parameters.
     * 
     * This is a style choice - Google C++ Style Guide recommends it.
     */
    int sock_;                                                              // Socket file descriptor
    bool use_sim_;                                                          // Simulation mode flag
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr rpm_pub_;          // Smart pointer to publisher
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr speed_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr steering_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

    // ==========================================================================
    // Helper function to initialize CAN socket
    // ==========================================================================
    bool initSocket(const char* ifname)
    {
        struct sockaddr_can addr;
        struct ifreq ifr;

        // Create a CAN socket
        sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (sock_ < 0) return false;

        // Copy the interface name (with safety limit)
        std::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
        if (ioctl(sock_, SIOCGIFINDEX, &ifr) < 0) return false;

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        // Bind the socket to the CAN interface
        if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) return false;

        return true;
    }

    // ==========================================================================
    // Timer callback to read CAN frames
    // ==========================================================================
    void readCanFrame()
    {
// =============================================================================
// TOPIC 11: Static Variables
// =============================================================================
/*
 * A STATIC LOCAL VARIABLE retains its value between function calls.
 * It's initialized only ONCE, the first time the function runs.
 * 
 * Java Comparison:
 * ----------------
 * Java static members belong to the class.
 * C++ static LOCAL variables persist across function calls (unique to C++).
 * 
 * USE CASES:
 * - Counters that accumulate
 * - Caching expensive computations
 * - Simulation state that needs to persist
 * 
 * EXAMPLE:
 * --------
 * void countCalls() {
 *     static int count = 0;  // Only initialized once!
 *     count++;
 *     std::cout << "Called " << count << " times" << std::endl;
 * }
 * // First call: "Called 1 times"
 * // Second call: "Called 2 times"  (count persists!)
 */
        if (use_sim_) {
            // Static variable - persists between function calls
            static float fake_angle = 0.0f;
            fake_angle += 0.5f;
            if (fake_angle > 360.0f) fake_angle = 0.0f;

            publishSteering(fake_angle);
            RCLCPP_INFO(this->get_logger(), "[SIM] Steering: %.2f deg", fake_angle);
            return;
        }

// =============================================================================
// TOPIC 12: Type Casting
// =============================================================================
/*
 * CASTING converts one type to another.
 * 
 * C-Style Cast:     (float)myInt        // Old way, avoid in modern C++
 * static_cast:      static_cast<float>(myInt)  // Modern, safe way
 * 
 * Java Comparison:
 * ----------------
 * Java:   (double) myInt
 * C++:    static_cast<double>(myInt)  // Preferred
 * C++:    (double) myInt              // Works but less safe
 * 
 * EXAMPLE:
 * --------
 * int whole = 5;
 * float decimal = static_cast<float>(whole);  // decimal = 5.0
 */

// =============================================================================
// TOPIC 13: Bitwise Operations
// =============================================================================
/*
 * Bitwise operations manipulate individual bits in numbers.
 * Essential for decoding CAN bus data!
 * 
 * OPERATORS:
 * <<  Left shift   (multiply by 2^n)    Example: 1 << 8 = 256
 * >>  Right shift  (divide by 2^n)      Example: 256 >> 2 = 64
 * |   OR           (combine bits)       Example: 0b1100 | 0b0011 = 0b1111
 * &   AND          (mask bits)          Example: 0b1100 & 0b0101 = 0b0100
 * 
 * COMBINING BYTES INTO 16-BIT VALUE:
 * High byte: 0x0A (10 in decimal)
 * Low byte:  0x2E (46 in decimal)
 * 
 * (0x0A << 8) | 0x2E = 0x0A2E = 2606
 * 
 * Visual:
 *   High byte (0x0A):  00001010
 *   After << 8:        00001010 00000000  (shifted left 8 positions)
 *   Low byte (0x2E):              00101110
 *   After | (OR):      00001010 00101110  = 0x0A2E
 * 
 * EXAMPLE:
 * --------
 * uint8_t highByte = frame.data[0];   // First byte
 * uint8_t lowByte = frame.data[1];    // Second byte
 * int16_t combined = (highByte << 8) | lowByte;  // 16-bit value
 */
        // Read a CAN frame from the socket
        struct can_frame frame;
        int nbytes = read(sock_, &frame, sizeof(struct can_frame));

        if (nbytes < 0) return;  // No data available

        // Decode OBD-II standard responses (ID 0x7E8)
        if (frame.can_id == 0x7E8) {
            uint8_t pid = frame.data[2];  // PID is in byte 2

            if (pid == 0x0C) {
                // RPM Calculation: ((A * 256) + B) / 4
                // Using bitwise: (A << 8) | B) / 4.0
                float rpm = ((frame.data[3] << 8) + frame.data[4]) / 4.0f;
                publishRpm(rpm);
            }
            else if (pid == 0x0D) {
                // Speed is directly in km/h (single byte)
                float speed_kmh = static_cast<float>(frame.data[3]);
                publishSpeed(speed_kmh);
            }
        }
        // Decode BMW-specific steering angle (ID 0x0C2)
        else if (frame.can_id == 0x0C2) {
            // Combine two bytes into a signed 16-bit integer
            int16_t raw_steering = (frame.data[0] << 8) | frame.data[1];
            float steering_angle = raw_steering * 0.1f;  // Scale factor
            publishSteering(steering_angle);
        }
    }

    // ==========================================================================
    // Helper functions to publish data
    // ==========================================================================
    void publishRpm(float rpm) {
        auto msg = std_msgs::msg::Float32();  // 'auto' lets compiler deduce the type
        msg.data = rpm;
        rpm_pub_->publish(msg);               // -> because rpm_pub_ is a pointer
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

    // ==========================================================================
    // Callback for Gazebo simulation odometry
    // ==========================================================================
    /*
     * This function is called automatically when a new Odometry message arrives.
     * 
     * Parameter type: const nav_msgs::msg::Odometry::SharedPtr msg
     * - const: The message won't be modified
     * - SharedPtr: Smart pointer (automatically managed memory)
     * - msg: Name of the parameter
     */
    void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg) {
        // Get linear speed (m/s) and convert to km/h
        float speed_mps = msg->twist.twist.linear.x;
        float speed_kmh = speed_mps * 3.6f;

        // Fake RPM based on speed (for visualization)
        float rpm = speed_kmh * 40.0f;

        // Get angular velocity and convert to steering angle
        float steering_rad = msg->twist.twist.angular.z;
        float steering_deg = steering_rad * (180.0f / 3.14159f) * 10.0f;

        // Publish to dashboard topics
        publishSpeed(speed_kmh);
        publishRpm(rpm);
        publishSteering(steering_deg);
    }
};

// =============================================================================
// MAIN FUNCTION - Entry Point
// =============================================================================
/*
 * Every C++ program starts here.
 * 
 * argc = Argument Count (how many command-line arguments)
 * argv = Argument Values (array of C-style strings)
 * 
 * Java Comparison:
 * ----------------
 * Java:   public static void main(String[] args)
 * C++:    int main(int argc, char** argv)
 * 
 * The return value (0) indicates success to the operating system.
 */
int main(int argc, char** argv)
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);

    // Create our node using std::make_shared (creates a smart pointer)
    // This is like Java's: BmwTelemetryNode node = new BmwTelemetryNode();
    auto node = std::make_shared<BmwTelemetryNode>();

    // Keep the node running (process callbacks until shutdown)
    rclcpp::spin(node);

    // Clean shutdown
    rclcpp::shutdown();

    return 0;  // Success
}