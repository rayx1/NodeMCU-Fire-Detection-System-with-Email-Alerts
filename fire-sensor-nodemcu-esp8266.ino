#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi credentials
const char* ssid = "WIFI";
const char* password = "12345678";

// Email settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient@example.com"

// Pin definitions
const int flameSensorPin = A0;    // Analog flame sensor input
const int buzzerPin = D6;         // Active LOW buzzer control
const int builtinLedPin = D4;     // NodeMCU built-in LED (active LOW)

// Thresholds
const int FIRE_THRESHOLD = 850;   // Values below this indicate fire
const unsigned long emailInterval = 300000; // 5 minutes between emails

// Variables
bool fireDetected = false;
unsigned long lastEmailTime = 0;
int sensorValue = 0;

// SMTP Session object
SMTPSession smtp;

// Function prototypes
void blinkLED(int onTime, int offTime, int repetitions = 1);
void activateAlarm();
void sendEmailAlert();

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(builtinLedPin, OUTPUT);
  
  // Initial states
  digitalWrite(builtinLedPin, HIGH); // LED off (active LOW)
  digitalWrite(buzzerPin, HIGH);      // Buzzer off (active HIGH)

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blinkLED(100, 100); // Blink while connecting
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup complete indication
  blinkLED(100, 100, 3);
}

void loop() {
  sensorValue = analogRead(flameSensorPin);
  Serial.print("Flame sensor: ");
  Serial.println(sensorValue);
  
  if (sensorValue < FIRE_THRESHOLD) {
    if(!fireDetected) {
      Serial.println("Fire detected!");
      fireDetected = true;
    }
    activateAlarm();
    
    if (millis() - lastEmailTime > emailInterval) {
      sendEmailAlert();
      lastEmailTime = millis();
    }
  } else {
    if(fireDetected) {
      Serial.println("Fire cleared");
      fireDetected = false;
    }
    digitalWrite(buzzerPin, HIGH);     // Ensure buzzer is off
    digitalWrite(builtinLedPin, HIGH); // Ensure LED is off
  }
  
  delay(200);
}

void activateAlarm() {
  // Sound buzzer and blink LED (5 cycles)
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzerPin, LOW);    // Buzzer on
    digitalWrite(builtinLedPin, LOW); // LED on
    delay(500);
    digitalWrite(buzzerPin, HIGH);     // Buzzer off
    digitalWrite(builtinLedPin, HIGH); // LED off
    delay(500);
  }
}

void sendEmailAlert() {
  smtp.debug(1);

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "NodeMCU Fire Alarm";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "FIRE DETECTED!";
  message.addRecipient("Recipient", RECIPIENT_EMAIL);

  String textMsg = "WARNING: Fire detected by NodeMCU fire alarm system!\n\n";
  textMsg += "Sensor Reading: " + String(sensorValue) + "\n";
  textMsg += "(Threshold: " + String(FIRE_THRESHOLD) + ")\n\n";
  textMsg += "System Uptime: " + String(millis() / 1000) + " seconds\n";
  textMsg += "IP Address: " + WiFi.localIP().toString();
  message.text.content = textMsg.c_str();

  if (!smtp.connect(&session)) {
    Serial.println("SMTP connection error");
    blinkLED(100, 100, 10); // Rapid blink for failure
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Email sending failed");
    blinkLED(100, 100, 10);
  } else {
    Serial.println("Email sent successfully");
    blinkLED(200, 200, 3); // Confirmation blink
  }
}

void blinkLED(int onTime, int offTime, int repetitions) {
  for(int i=0; i<repetitions; i++) {
    digitalWrite(builtinLedPin, LOW);  // LED on
    delay(onTime);
    digitalWrite(builtinLedPin, HIGH); // LED off
    delay(offTime);
  }
}