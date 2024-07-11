#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SSD1306.h> // Thư viện màn hình OLED

// #define AP_SSID "SonLong"
// #define AP_PASSWORD "long6485"
#define AP_SSID "HSU_Students"
#define AP_PASSWORD "dhhs12cnvch"
#define DB_URL "https://long-bc130-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DB_SECRET "Gj0w1L8JKWJQmHzltn9lzsxxOVJZdbjLhxFIUi7c"

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData fbdo;

SSD1306 oled(0x3c, D2, D3); // Địa chỉ I2C và các chân SDA, SCL của màn hình OLED
int const T_RELAY_PIN = D5;
int const LED_RED_PIN = D8;
int const LED_YELLOW_PIN = D6;
int const LED_BLUE_PIN = D7;
int const SOIL_SENSOR_PIN = A0; // Chân kết nối cảm biến độ ẩm đất
int bumpStatus = 0;

void setup() {
  Serial.begin(115200);
  pinMode(T_RELAY_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  // Khởi tạo màn hình OLED
  oled.init();
  oled.flipScreenVertically();
  oled.setFont(ArialMT_Plain_16);
  oled.setTextAlignment(TEXT_ALIGN_CENTER);

  initWifi();
  initFirebase();
}

void loop() {
  checkRelayStatusFromFirebase(); // Kiểm tra trạng thái relay từ Firebase
  checkSoilMoistureStatus(); // Kiểm tra trạng thái độ ẩm đất
  showOledInfo(); // Hiển thị trạng thái relay và độ ẩm lên màn hình OLED
}

void initWifi() {
  Serial.print("\nConnecting to ");
  Serial.print(AP_SSID);
  WiFi.begin(AP_SSID, AP_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void initFirebase() {
  config.database_url = DB_URL;
  config.signer.tokens.legacy_token = DB_SECRET;
  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);
}

void checkRelayStatusFromFirebase() {
  if (Firebase.ready()) {
    if (Firebase.getInt(fbdo, "RelayStatus")) {
      int relayStatus = fbdo.intData();
      if (relayStatus == 1) {
        digitalWrite(T_RELAY_PIN, HIGH); // Bật relay
        digitalWrite(LED_RED_PIN, HIGH); // Đèn đỏ sáng
        digitalWrite(LED_YELLOW_PIN, HIGH); // Đèn vàng sáng
        digitalWrite(LED_BLUE_PIN, LOW); // Đèn xanh tắt
        bumpStatus = 1;
      } else {
        digitalWrite(T_RELAY_PIN, LOW);  // Tắt relay
        digitalWrite(LED_RED_PIN, LOW); // Đèn đỏ tắt
        digitalWrite(LED_YELLOW_PIN, LOW); // Đèn vàng tắt
        digitalWrite(LED_BLUE_PIN, HIGH); // Đèn xanh sáng
        bumpStatus = 0;
      }
      Serial.println("Relay status from Firebase: " + String(relayStatus));
    } else {
      Serial.println("Failed to get relay status from Firebase");
    }
  }
}

void checkSoilMoistureStatus() {
  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);
  if (soilMoistureValue < 500) {
    Serial.println("Soil is wet");
  } else if (soilMoistureValue > 750) {
    Serial.println("Soil is dry");
  } else {
    Serial.println("Soil moisture is normal");
  }
}

void showOledInfo() {
  oled.clear();
  if (bumpStatus == 1) {
    oled.drawString(64, 20, "Relay ON");
  } else {
    oled.drawString(64, 20, "Relay OFF");
  }

  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);
  String moistureStatus = (soilMoistureValue < 500) ? "Wet" : (soilMoistureValue > 750) ? "Dry" : "Normal";
  oled.drawString(64, 40, "Soil: " + moistureStatus);

  oled.display();
}

// #include <SSD1306.h>
// #define SDA_PIN D2
// #define SCL_PIN D3
// SSD1306 oled(0x3c, SDA_PIN, SCL_PIN);
// #include <ESP8266WiFi.h>
// #include <FirebaseESP8266.h>
// FirebaseConfig config;
// FirebaseAuth auth;
// FirebaseData fbdo;
// #define DB_URL "https://long-bc130-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #define DB_SECRET "Gj0w1L8JKWJQmHzltn9lzsxxOVJZdbjLhxFIUi7c"
// #define AP_SSID "SonLong_5G"
// #define AP_PASSWORD "long6485"
// int const SENSOR_MH_D_PIN = D4;
// int const SENSOR_MH_A_PIN = A0;
// int const T_RELAY_PIN = D5;

// int const TIME_TO_GET_SAMPLE = 5000; //5s
// int const SAMPLE_TIME = 500; //0.5 s
// int const LED_PIN = 13;

// int const LED_BLUE_PIN = D7;
// int const LED_YEWLOW_PIN = D6;
// int const LED_RED_PIN = D8;

// int const TREE_WATER_LEVEL_HIGH = 600;
// int const TREE_WATER_LEVEL_LOW = 300;

// int sensorMHValue = 0;//store sensor value

// int bumpStatus = 0;
// int waterStatus = 0;// -1: less water, 0: enough water, +1 : more water

// void setup() {
//   initOled();
//   Serial.begin(115200);
//   pinMode(SENSOR_MH_D_PIN, INPUT);
//   pinMode(T_RELAY_PIN, OUTPUT);

//   pinMode(LED_PIN, OUTPUT);
//   pinMode(LED_BLUE_PIN, OUTPUT);
//   pinMode(LED_YEWLOW_PIN, OUTPUT);
//   pinMode(LED_RED_PIN, OUTPUT);
// }

// void loop() {
//   // process and checking to watering
//   wateringProcess();

//   // Send information to serial port
//   //printToSerialPort();

//   //show the led status
//   showLedInfo();
// }

// void wateringProcess(){
//   int sensorStatus = digitalRead(SENSOR_MH_D_PIN);
//   int sensorValue = getSensorSampleValue();
//   if(sensorStatus == 0){
//     if(sensorValue > TREE_WATER_LEVEL_HIGH){
//       digitalWrite(T_RELAY_PIN, HIGH);
//       bumpStatus = 1;
//       waterStatus = -1;
//     } else if(sensorValue < TREE_WATER_LEVEL_LOW){
//       digitalWrite(T_RELAY_PIN, LOW);
//       bumpStatus = 0;
//       waterStatus = +1;
//     } else {
//       digitalWrite(T_RELAY_PIN, LOW);
//       bumpStatus = 0;
//       waterStatus = 0;
//     }
//   } else {
//     digitalWrite(T_RELAY_PIN, HIGH);
//     bumpStatus = 1;
//     waterStatus = -1;
//   }
// }

// int getSensorSampleValue(){
//   int value = 0;
//   int t = TIME_TO_GET_SAMPLE / SAMPLE_TIME;
//   int total = 0;
//   for(int i = 0; i < t; i++){
//     total += analogRead(SENSOR_MH_A_PIN);
//     delay(t);
//   }
//   return total / t;
// }

// void printToSerialPort(){
//   Serial.print("Bumper enable: "); Serial.println(bumpStatus);
//   Serial.print("Sensor value: "); Serial.println(sensorMHValue);
// }

// void showLedInfo(){
//   if(bumpStatus == 1){
//     digitalWrite(LED_YEWLOW_PIN, HIGH);
//   } else {
//     digitalWrite(LED_YEWLOW_PIN, LOW);
//   }

//   if(waterStatus == 0){
//     digitalWrite(LED_RED_PIN, LOW);
//     digitalWrite(LED_BLUE_PIN, HIGH);
//     printOled("OLED", "Tắt");
//   } else {
//     digitalWrite(LED_RED_PIN, HIGH);
//     digitalWrite(LED_BLUE_PIN, LOW);
//     printOled("OLED", "Bật");
//   }
// }

// void initOled(){
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



























