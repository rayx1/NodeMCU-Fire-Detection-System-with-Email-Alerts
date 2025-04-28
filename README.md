# NodeMCU-Fire-Detection-System-with-Email-Alerts
📌 Overview
This project creates an IoT-based fire detection system using NodeMCU (ESP8266) that:

Monitors environment with an analog flame sensor

Triggers audible (buzzer) and visual (LED) alarms when fire is detected

Sends email notifications via SMTP (Gmail supported)

Provides real-time monitoring via Serial output

🛠 Hardware Components
Component	Quantity
NodeMCU ESP8266	1
IR Flame Sensor (Analog)	1
Active Buzzer	1
Breadboard	1
Jumper Wires	Several
🔌 Circuit Diagram
NodeMCU Pinout:
- A0 → Flame Sensor AO
- D6 → Buzzer (+)
- D4 → Built-in LED
- GND → Buzzer (-) and Flame Sensor GND
- 3.3V → Flame Sensor VCC

- ⚙️ Software Setup
Install Required Libraries:
ESP8266WiFi.h
ESP_Mail_Client.h
Configure Settings:

Update WiFi credentials in config.h

Set email parameters (SMTP server, credentials)

Adjust fire detection threshold (default: 850)

Upload Code:

Connect NodeMCU via USB

Select correct board and port in Arduino IDE

Upload the sketch

Configuration
Create a config.h file with your sensitive data:
// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Email Settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient@example.com"

// Detection Threshold
#define FIRE_THRESHOLD 850
📧 Email Setup (Gmail Example)
Enable "Less secure app access" or create an App Password

Use these SMTP settings:

Server: smtp.gmail.com

Port: 465 (SSL) or 587 (TLS)

Authentication Required: Yes

🚀 Features
Real-time Monitoring: Analog flame detection

Multi-alert System:

Audible buzzer alarm

Visual LED indicator

Email notifications

Configurable Threshold: Adjust sensitivity

WiFi Connectivity: Remote monitoring capability

📊 Serial Monitor Output
Connecting to WiFi.......
Connected, IP address: 192.168.1.100
Flame sensor: 923
Flame sensor: 815
Fire detected!
Flame sensor: 790
Email sent successfully
Flame sensor: 920
Fire cleared
🤝 Contributing
Contributions are welcome! Please open an issue or submit a PR for any:

Bug fixes

New features

Documentation improvements

📜 License
MIT License - See LICENSE for details.
