# Voltage-divider-on-thrust-rig

# ESP32-Based Thrust Measurement System for Drone Testing

This project is a thrust measurement rig built using an ESP32 microcontroller, a load cell, and a voltage divider circuit to accurately monitor and log thrust, voltage, and PWM signal input for brushless motors used in drone propulsion systems.

Purpose

The system is designed to:
- Measure the **thrust produced by a motor** in grams.
- Read the **input voltage** supplied to the ESC using a calibrated voltage divider.
- Capture the **PWM signal width** sent to the ESC, indicating throttle percentage.
- Log and transmit the measured data to a server in real time.

This setup is crucial in evaluating drone performance, tuning ESC behavior, optimizing propeller-motor combinations, and ensuring safe power system operation.

Hardware Used

Component           Description  
ESP32               Main microcontroller for data processing      
HX711               24-bit ADC for load cell                       
Load Cell           Measures thrust produced by motor             
Voltage Divider     Reads battery voltage using GPIO35            
ESC & Motor         Motor under test                              
Power Supply        Powers the ESC + motor setup                  

Voltage Divider Circuit

The voltage divider is used to safely measure the high battery voltage using the ESP32's ADC (Analog-to-Digital Converter):

