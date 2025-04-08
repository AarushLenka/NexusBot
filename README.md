# Nexus: Smart Home Monitoring Bot

Nexus is an advanced smart home monitoring bot powered by ESP32. It combines environmental monitoring and obstacle avoidance features with a built-in vacuum cleaner to maintain a clean and safe living environment. This project leverages various sensors to detect environmental parameters and ensure efficient operation in dynamic surroundings. Sensor data is displayed on a webserver, and the bot can be controlled manually using Blynk IoT.

## Features

- **Environmental Monitoring**:
  - **DHT Sensor**: Measures temperature and humidity.
  - **Flame Sensor**: Detects fire hazards.
  - **Gas Sensor**: Monitors harmful gases in the surroundings.

- **Obstacle Detection**:
  - **IR Sensor**: Ensures smooth navigation by avoiding obstacles.

- **Cleaning Functionality**:
  - Equipped with a vacuum cleaner for maintaining cleanliness.

- **Control Options**:
  - **Manual Mode**: Controlled via Blynk IoT.
  - **Automatic Mode**: Navigates and operates autonomously.

- **Powered by ESP32**: The microcontroller acts as the brain of the bot, managing all sensor data and control functions.

## Repository Structure

This repository contains the following files:

- **`Manual+Environmental.ino`**: Implements manual control using Blynk IoT.
- **`Automatic+Webserver.ino`**: Handles autonomous navigation and operation.

## Hardware Requirements

- **ESP32 Microcontroller**
- **DHT11 Sensor** (for temperature and humidity monitoring)
- **Flame Sensor**
- **Gas Sensor**
- **IR Sensor**
- Power Supply

## Software Requirements

- Arduino IDE
- ESP32 Board Package (installable via Arduino IDE Board Manager)
- Required Libraries:

  - `Blynk` (for IoT control)
  - `DHT` (for DHT sensor)
