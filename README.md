# 🚀 Phase 4: Robot Mobility & Decision-Making System

---

## 🎯 Objective

Extend the perception pipeline into a mobile robotic system capable of making movement decisions and controlling motors in real time.

This phase focuses on integrating:

* Decision-making logic
* Motor control
* Serial communication
* ESP32/Arduino firmware
* Real-world robot movement

The goal is to enable the robot to:

* Detect objects
* Track targets
* Decide movement direction
* Control motors accordingly

---

## 🏗️ What Was Implemented

* Decision node for movement logic
* Motor control node for serial communication
* ESP32 motor control firmware
* Integration between Jetson Orin Nano and microcontroller
* Forward / Reverse / Left / Right robot control
* Real-time command transmission pipeline

---

## 🔹 Package Used

* `phase4_pkg`

---

## 🔹 Nodes Created

### 🧠 Decision Node

* `decision_node`
* Receives perception/tracking outputs
* Decides:

  * Move Forward
  * Stop
  * Turn Left
  * Turn Right

---

### ⚙️ Motor Control Node

* `motor_control_node`
* Sends movement commands from ROS2 → ESP32/Arduino through serial communication

---

### 📡 Serial Communication

Jetson communicates with ESP32/Arduino using serial interface:

```bash
/dev/ttyUSB0
```

---

## 🔹 Data Flow

Camera → Detection → Tracking → Decision Node → Motor Control Node → ESP32 → Motors

---

## ⚙️ Hardware Used

* Jetson Orin Nano
* ESP32 / Arduino Uno R3
* Motor Driver (L298N testing phase)
* DC Motors
* Robot chassis (mobility testing)

---

## ⚙️ Environment

* Ubuntu 22.04
* ROS2 Humble
* Arduino IDE
* Python (ROS2 nodes)
* C++ / Arduino firmware

---

## 🧪 Commands Used

### ROS2

```bash
# Launch robot pipeline
ros2 launch phase4_pkg robot_pipeline.launch.py

# Debugging
ros2 topic list
ros2 node list
```

---

### Serial Device Check

```bash
ls /dev/ttyACM*
```

---

### Permission Fix

```bash
sudo chmod 666 /dev/ttyACM0
```

---

### Arduino Upload

```bash
# Upload firmware using Arduino IDE
```

---

## 🚧 Challenges Faced & Solutions

### 🔴 Issue 1: Serial Port Permission Denied

**Problem:**
ROS2 node unable to communicate with ESP32/Arduino

**Error Example:**

```bash
Permission denied: '/dev/ttyACM0'
```

**Solution:**

```bash
sudo chmod 666 /dev/ttyACM0
```

---

### 🔴 Issue 2: Motors Not Responding

**Problem:**
Commands received but motors not moving properly

**Root Cause:**
Incorrect motor driver wiring / PWM handling

**Solution:**

* Rechecked L298N connections
* Updated ESP32 PWM logic
* Verified enable pins and motor polarity

---

### 🔴 Issue 3: Serial Communication Instability

**Problem:**
Robot movement became inconsistent after repeated commands

**Root Cause:**
Serial parsing issues between ROS2 and ESP32

**Solution:**
Used structured serial commands:

```bash
0,0
100,100
-100,100
```

---

### 🔴 Issue 4: Delay Between Detection and Movement

**Problem:**
Robot response lagged behind perception output

**Root Cause:**
Inference + serial communication latency

**Solution:**

* Reduced unnecessary processing
* Optimized node execution flow
* Improved command publishing frequency

---

### 🔴 Issue 5: Motors Continuing After Shutdown

**Problem:**
Robot continued moving after node exit/crash

**Solution:**
Added emergency stop command inside cleanup block:

```python
finally:
    if rclpy.ok():
        node.destroy_node()
        if node.ser:
            node.ser.write(b'0,0\\n')
        rclpy.shutdown()
```

---

## 📊 Learnings

* Learned real-world robot control integration
* Understood perception-to-action pipeline
* Gained experience with serial communication debugging
* Learned importance of safety handling in robotics
* Improved understanding of motor control systems

---

## 🔮 Future Improvements

* Replace L298N with higher-performance motor driver
* Add encoder feedback
* Integrate LiDAR for obstacle avoidance
* Implement autonomous navigation
* Add follow-me behavior

---

## 📂 Folder Structure

```bash
ros2_phase4/
├── src/
│   └── phase4_pkg
├── firmware/
│   └── esp32_motor_control
├── build/
├── install/
└── log/
```

---

## 💡 Key Takeaway

This phase transformed the project from a perception-only system into a physically interactive robot capable of making decisions and controlling movement in real time.

---
