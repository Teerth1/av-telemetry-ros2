FROM ros:humble

# 1. Install standard build tools (Make, G++, etc.)
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Add this line to install OpenCV
RUN apt-get update && apt-get install -y libopencv-dev python3-opencv

# 2. Set up the workspace directory inside the container
WORKDIR /ros2_ws

# 3. Copy our CMakeLists.txt and package.xml first (to cache dependencies if we had them)
#    We will copy the whole source in the next steps or mount it via volumes.
#    For a simple dev setup, we can rely on volumes, but copying ensures the image builds standalone.
COPY src ./src

# 4. Build the project using Colcon (ROS 2 build tool)
#    'source /opt/ros/humble/setup.sh' is needed for ROS 2 to be found.
RUN . /opt/ros/humble/setup.sh && colcon build --symlink-install

# 5. Add ROS 2 setup to the entrypoint so it runs automatically when we start
#    This makes "ros2 run" commands work immediately.
RUN echo "source /opt/ros/humble/setup.sh" >> ~/.bashrc
RUN echo "source /ros2_ws/install/setup.bash" >> ~/.bashrc

# 6. Default command (keep container alive so we can interact with it)
CMD ["bash"]
