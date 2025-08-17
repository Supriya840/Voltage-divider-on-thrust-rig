#include <WiFi.h>
#include <HTTPClient.h>
#include "HX711.h"
#include <esp_adc_cal.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
String serverURL = "http://localhost/thrust/post_data.php";

#define DOUT  26  // Load cell data pin
#define CLK   25  // Load cell clock pin
HX711 scale;

const int R1 = 10000;  // Lower resistor (10kΩ)
const int R2 = 98000;  // Upper resistor (98kΩ)
const int VinPin = 35; // Voltage divider output connected to GPIO35
float VB1;
float correctionFactor = 0.727;  // Adjust based on calibration
esp_adc_cal_characteristics_t *adc_chars;

const int pwmPin = 14; // Connect to ESC PWM Signal
int pwmValue;
bool testRunning = false;

void readVoltage() {
  uint32_t voltage_mV = esp_adc_cal_raw_to_voltage(analogRead(VinPin), adc_chars);
  VB1 = (((float) voltage_mV) / 1000.0) * (1 + (float)R2 / (float)R1);
  VB1 *= correctionFactor;
}

void readPWM() {
  pwmValue = pulseIn(pwmPin, HIGH, 25000); 
}

float readThrust() {
  return scale.get_units(5); 
}

void sendData(float pwm, float voltage, float thrust) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fullURL = serverURL + "?pwm=" + String(pwm) + "?current=" + String(1) + "&voltage=" + String(voltage) + "&thrust=" + String(thrust);
    
    http.begin(fullURL);
    int httpResponseCode = http.GET();
    
    Serial.print("Server Response: ");
    Serial.println(httpResponseCode);
    
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // ADC Calibration
  adc_chars = (esp_adc_cal_characteristics_t*) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);

  // Load Cell Setup
  scale.begin(DOUT, CLK);
  scale.set_scale(2280.f); // Adjust calibration factor
  scale.tare();

  pinMode(pwmPin, INPUT);

  Serial.println("Enter 'tare' to reset scale, 'start' to begin test, or 'stop' to stop.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();  

    if (command == "tare") {
      Serial.println("TARE Applied!");
      scale.tare();
    } 
    else if (command == "start") {
      Serial.println("Test Started!");
      testRunning = true;
    }
    else if (command == "stop") {
      Serial.println("Test Stopped!");
      testRunning = false;
    }
  }

  if (testRunning) {
    readVoltage();
    readPWM();
    float thrust = readThrust();

    Serial.print("PWM: ");
    Serial.print(pwmValue);
    Serial.print(" | Voltage: ");
    Serial.print(VB1);
    Serial.print("V | Thrust: ");
    Serial.print(thrust);
    Serial.println(" g");

    //sendData(pwmValue, VB1, thrust);
  }

  delay(500);
}
