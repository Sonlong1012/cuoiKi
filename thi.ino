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
int const SOIL_DRY_THRESHOLD = 750; // > ngưỡng này: đất khô
int const SOIL_WET_THRESHOLD = 500; // < ngưỡng này: đất ẩm
int bumpStatus = 0;
int autoMode = 1; // 1: tự động, 0: thủ công từ web

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
  handleRelayControlMode(); // Điều khiển relay theo AutoMode/RelayStatus
  showOledInfo(); // Hiển thị trạng thái relay và độ ẩm lên màn hình OLED
  delay(1000);
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

void applyRelayOutput() {
  digitalWrite(T_RELAY_PIN, bumpStatus == 1 ? HIGH : LOW);
  digitalWrite(LED_RED_PIN, bumpStatus == 1 ? HIGH : LOW);    // Đèn đỏ = bật
  digitalWrite(LED_YELLOW_PIN, bumpStatus == 1 ? HIGH : LOW); // Đèn vàng phụ trạng thái bơm
  digitalWrite(LED_BLUE_PIN, bumpStatus == 1 ? LOW : HIGH);   // Đèn xanh = tắt
}

void handleRelayControlMode() {
  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

  if (Firebase.ready()) {
    // Đọc cờ chế độ từ web: 1 tự động, 0 thủ công
    if (Firebase.getInt(fbdo, "AutoMode")) {
      autoMode = fbdo.intData() == 1 ? 1 : 0;
    }

    if (autoMode == 1) {
      // Đất khô -> bật bơm, đất ẩm -> tắt bơm.
      // Khoảng giữa giữ trạng thái trước đó để tránh nhấp nháy relay.
      if (soilMoistureValue > SOIL_DRY_THRESHOLD) {
        bumpStatus = 1;
      } else if (soilMoistureValue < SOIL_WET_THRESHOLD) {
        bumpStatus = 0;
      }
    } else {
      // Chế độ thủ công: lấy RelayStatus từ web
      if (Firebase.getInt(fbdo, "RelayStatus")) {
        bumpStatus = fbdo.intData() == 1 ? 1 : 0;
      }
    }

    applyRelayOutput();
    Firebase.setInt(fbdo, "RelayStatus", bumpStatus);
    Firebase.setInt(fbdo, "SoilMoisture", soilMoistureValue);
    Firebase.setInt(fbdo, "AutoMode", autoMode);
  } else {
    // Mất mạng/Firebase: giữ trạng thái gần nhất đã tính và xuất ra relay
    applyRelayOutput();
  }

  Serial.println(
    "Mode: " + String(autoMode == 1 ? "AUTO" : "MANUAL") +
    " | Soil: " + String(soilMoistureValue) +
    " | Relay: " + String(bumpStatus == 1 ? "ON" : "OFF")
  );
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



























