// #include <SSD1306.h>
// #include <ESP8266WiFi.h>
// #include <FirebaseESP8266.h>

// #define SDA_PIN D2
// #define SCL_PIN D3
// #define AP_SSID "HSU_Students"
// #define AP_PASSWORD "dhhs12cnvch"
// // #define AP_SSID "HSU_Students"
// // #define AP_PASSWORD "dhhs12cnvch"
// #define DB_URL "https://long-bc130-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #define DB_SECRET "Gj0w1L8JKWJQmHzltn9lzsxxOVJZdbjLhxFIUi7c"

// FirebaseConfig config;
// FirebaseAuth auth;
// FirebaseData fbdo;

// SSD1306 oled(0x3c, SDA_PIN, SCL_PIN);

// int const SENSOR_MH_D_PIN = D4;
// int const SENSOR_MH_A_PIN = A0;
// int const T_RELAY_PIN = D5;

// int const TIME_TO_GET_SAMPLE = 5000; // 5s
// int const SAMPLE_TIME = 500; // 0.5s

// int const LED_BLUE_PIN = D7;
// int const LED_YELLOW_PIN = D6;
// int const LED_RED_PIN = D8;

// int const TREE_WATER_LEVEL_HIGH = 600;
// int const TREE_WATER_LEVEL_LOW = 300;

// int bumpStatus = 0;  // Relay status
// int waterStatus = 0; // -1: less water, 0: enough water, +1: more water

// void setup() {
//   initOled();
//   Serial.begin(115200);
//   pinMode(SENSOR_MH_D_PIN, INPUT);
//   pinMode(T_RELAY_PIN, OUTPUT);

//   pinMode(LED_BLUE_PIN, OUTPUT);
//   pinMode(LED_YELLOW_PIN, OUTPUT);
//   pinMode(LED_RED_PIN, OUTPUT);

//   initWifi();
//   initFirebase();
// }

// void loop() {
//   wateringProcess();
//   showLedInfo();
// }

// void wateringProcess() {
//   int sensorStatus = digitalRead(SENSOR_MH_D_PIN);
//   int sensorValue = getSensorSampleValue();
  
//   if (sensorStatus == 0) {
//     if (sensorValue > TREE_WATER_LEVEL_HIGH) {
//       digitalWrite(T_RELAY_PIN, LOW);  // Turn off the relay
//       bumpStatus = 0;
//       waterStatus = 1;
//     } else if (sensorValue < TREE_WATER_LEVEL_LOW) {
//       digitalWrite(T_RELAY_PIN, HIGH); // Turn on the relay
//       bumpStatus = 1;
//       waterStatus = -1;
//     } else {
//       digitalWrite(T_RELAY_PIN, LOW);  // Turn off the relay
//       bumpStatus = 0;
//       waterStatus = 0;
//     }
//   } else {
//     digitalWrite(T_RELAY_PIN, HIGH);   // Turn on the relay
//     bumpStatus = 1;
//     waterStatus = -1;
//   }

//   updateRelayStatusToFirebase(bumpStatus);
// }

// int getSensorSampleValue() {
//   int total = 0;
//   int t = TIME_TO_GET_SAMPLE / SAMPLE_TIME;
//   for (int i = 0; i < t; i++) {
//     total += analogRead(SENSOR_MH_A_PIN);
//     delay(SAMPLE_TIME);
//   }
//   return total / t;
// }

// void showLedInfo() {
//   if (bumpStatus == 1) {
//     digitalWrite(LED_RED_PIN, HIGH);
//     digitalWrite(LED_BLUE_PIN, LOW);
//     printOled("RELAY", "ON");
//   } else {
//     digitalWrite(LED_RED_PIN, LOW);
//     digitalWrite(LED_BLUE_PIN, HIGH);
//     printOled("RELAY", "OFF");
//   }
//   digitalWrite(LED_YELLOW_PIN, bumpStatus == 1 ? HIGH : LOW);
// }

// void initOled() {
//   oled.init();
//   oled.flipScreenVertically();
// }

// void printOled(String line1, String line2) {
//   oled.clear();
//   oled.setTextAlignment(TEXT_ALIGN_LEFT);
//   oled.setFont(ArialMT_Plain_24);
//   oled.drawString(0, 0, line1);
//   oled.drawString(0, 36, line2);
//   oled.display();
// }

// void initWifi() {
//   Serial.print("\nConnecting to "); Serial.print(AP_SSID);
//   WiFi.begin(AP_SSID, AP_PASSWORD);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print("\nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
//   WiFi.setAutoReconnect(true);
//   WiFi.persistent(true);
// }

// void initFirebase() {
//   config.database_url = DB_URL;
//   config.signer.tokens.legacy_token = DB_SECRET;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectNetwork(true);
// }

// void updateRelayStatusToFirebase(int status) {
//   if (Firebase.ready()) {
//     if (!Firebase.setInt(fbdo, "RelayStatus", status)) {
//       Serial.println("Failed to update relay status");
//     } else {
//       Serial.println("Relay status updated: " + String(status));
//     }
//   }
// }



#include <SSD1306.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define SDA_PIN D2
#define SCL_PIN D3
// #define AP_SSID "SonLong"
// #define AP_PASSWORD "long6485"
#define AP_SSID "HSU_Students"
 #define AP_PASSWORD "dhhs12cnvch"
#define DB_URL "https://long-bc130-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DB_SECRET "Gj0w1L8JKWJQmHzltn9lzsxxOVJZdbjLhxFIUi7c"

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData fbdo;

SSD1306 oled(0x3c, SDA_PIN, SCL_PIN);

int const SENSOR_MH_D_PIN = D4;
int const SENSOR_MH_A_PIN = A0;
int const T_RELAY_PIN = D5;

int const TIME_TO_GET_SAMPLE = 5000; // 5s
int const SAMPLE_TIME = 500; // 0.5s

int const LED_BLUE_PIN = D7;
int const LED_YELLOW_PIN = D6;
int const LED_RED_PIN = D8;

int const TREE_WATER_LEVEL_HIGH = 600;
int const TREE_WATER_LEVEL_LOW = 300;

int bumpStatus = 0;  // Relay status
int waterStatus = 0; // -1: less water, 0: enough water, +1: more water

void setup() {
  initOled();
  Serial.begin(115200);
  pinMode(SENSOR_MH_D_PIN, INPUT);
  pinMode(T_RELAY_PIN, OUTPUT);

  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  initWifi();
  initFirebase();
}

void loop() {
  wateringProcess();
  showLedInfo();
}

void wateringProcess() {
  int sensorStatus = digitalRead(SENSOR_MH_D_PIN);
  int sensorValue = getSensorSampleValue();
  
  if (sensorStatus == 0) {
    if (sensorValue > TREE_WATER_LEVEL_HIGH) {
      digitalWrite(T_RELAY_PIN, LOW);  // Turn off the relay
      bumpStatus = 0;
      waterStatus = 1;
    } else if (sensorValue < TREE_WATER_LEVEL_LOW) {
      digitalWrite(T_RELAY_PIN, HIGH); // Turn on the relay
      bumpStatus = 1;
      waterStatus = -1;
    } else {
      digitalWrite(T_RELAY_PIN, LOW);  // Turn off the relay
      bumpStatus = 0;
      waterStatus = 0;
    }
  } else {
    digitalWrite(T_RELAY_PIN, HIGH);   // Turn on the relay
    bumpStatus = 1;
    waterStatus = -1;
  }

  updateRelayStatusToFirebase(bumpStatus);
}

int getSensorSampleValue() {
  int total = 0;
  int t = TIME_TO_GET_SAMPLE / SAMPLE_TIME;
  for (int i = 0; i < t; i++) {
    total += analogRead(SENSOR_MH_A_PIN);
    delay(SAMPLE_TIME);
  }
  return total / t;
}

void showLedInfo() {
  if (bumpStatus == 1) {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, LOW);
    printOled("RELAY", "ON");
  } else {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, HIGH);
    printOled("RELAY", "OFF");
  }
  digitalWrite(LED_YELLOW_PIN, bumpStatus == 1 ? HIGH : LOW);
}

void initOled() {
  oled.init();
  oled.flipScreenVertically();
}

void printOled(String line1, String line2) {
  oled.clear();
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_24);
  oled.drawString(0, 0, line1);
  oled.drawString(0, 36, line2);
  oled.display();
}

void initWifi() {
  Serial.print("\nConnecting to "); Serial.print(AP_SSID);
  WiFi.begin(AP_SSID, AP_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void initFirebase() {
  config.database_url = DB_URL;
  config.signer.tokens.legacy_token = DB_SECRET;
  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);
}

void updateRelayStatusToFirebase(int status) {
  if (Firebase.ready()) {
    if (!Firebase.setInt(fbdo, "RelayStatus", status)) {
      Serial.println("Failed to update relay status");
    } else {
      Serial.println("Relay status updated: " + String(status));
    }
  }
}
