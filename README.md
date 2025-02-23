# PID Controller with Photoresistor on ESP32

## Overview

This project implements a Proportional-Integral-Derivative (PID) controller on an ESP32 to control a system using feedback from a photoresistor connected in a voltage divider configuration. The system is built using the ESP-IDF framework and C++, with data being tracked remotely through MQTT.
The primary goal of the project is to investigate how a PID controller can be applied to a embedded system to maintain a desired state (e.g., light intensity) based on real-time feedback.

## Components Used

- **ESP32**: The microcontroller running the PID controller and managing communication via MQTT.
- **Photoresistor (LDR)**: Used to sense light intensity by varying its resistance according to the amount of light falling on it.
- **Voltage Divider**: A simple resistive circuit used to convert the varying resistance of the photoresistor into a measurable voltage.
- **MQTT**: A lightweight messaging protocol used for remote data tracking and system control.
- **ESP-IDF**: The development framework for the ESP32, implemented using C++.
- **Remote Data Tracking**: The system tracks and visualizes data remotely via an MQTT broker.

## Project Architecture

The project consists of two main components:

1. **ESP32 Firmware**: Responsible for reading the sensor data, implementing the PID control loop, and sending/receiving messages via MQTT.
2. **Remote Backend**: A system that receives data from the ESP32, stores it, and visualizes it for monitoring and tuning purposes. This can include a Raspberry Pi with a Node-RED backend and data visualization tools such as Grafana and InfluxDB.

### Flow of Operation

1. **Sensor Reading**: The photoresistor is connected to the ESP32 in a voltage divider configuration. The ESP32 reads the voltage across the photoresistor to determine the current light intensity.
2. **PID Control**: The measured light intensity is compared to a desired target value. The PID controller calculates the necessary adjustments to the system's output (such as PWM duty cycle or motor speed) to bring the light intensity closer to the target.
3. **Remote Monitoring**: The data (light intensity, PID output, and control adjustments) is sent via MQTT to a backend for remote tracking and analysis.

## PID Control Algorithm

The PID control algorithm is implemented using the standard three components:

- **Proportional (P)**: Determines the reaction to the current error.
- **Integral (I)**: Accounts for past errors to eliminate steady-state error.
- **Derivative (D)**: Predicts future error, helping to reduce overshooting and improve stability.

The project is not yet optimized

