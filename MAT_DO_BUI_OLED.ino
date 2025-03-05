 //#include <TinyGPSPlus.h>

//TinyGPSPlus gps;

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define PHONE_NUMBER   "+84916700177"
const int SCREEN_WIDTH = 128; // OLED display width, in pixels
const int SCREEN_HEIGHT = 64; // OLED display height, in pixels
const byte OLED_RESET   =  -1; // Reset pin # (or -1 if sharing Arduino reset pin)
//const int SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display;  // Create 

const int xacnhan = 12;
double flat = 0.000000;
double flon = 0.000000;
const byte dongy = 5;
const byte thoat = 6;
const byte relay = 3;
#include "DHT.h"
const int DHTPIN = 13;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define MCU_SIM_BAUDRATE        115200
const byte DHTPIN1 = 8;
DHT dht1(DHTPIN1, DHT22);
float t = 0.0;
int h1,Sets = 0;
byte k = 0;
const byte MQ3 = A1;
#include <SoftwareSerial.h>
SoftwareSerial simSerial(10, 11); // RX, TX
/*static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial.available())
      gps.encode(Serial.read());
    flat =  gps.location.lat(), 6;
    flon =  gps.location.lng(), 6;
   // Serial.println("DOC GPS");
    /*Serial.print(flat,6);
      Serial.print(" ---- FLON: ");
      Serial.println(flon,6); 

  }
  while (millis() - start < ms);
}*/
void sim_at_wait()
{
  delay(100);
  /*    while (simSerial.available()) {
          Serial.write(simSerial.read());
      }*/
}
bool sim_at_cmd(String cmd) {
  simSerial.println(cmd);
  sim_at_wait();
}
bool sim_at_send(char c) {
  simSerial.write(c);
}
void sent_sms()

{ display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("DA GUI HO TRO"));
  display.display();
  delay(1000);
  //Serial.println("DANG GUI TIN NHAN");
  sim_at_cmd("AT+CMGF=1");
  String temp = "AT+CMGS=\"";
  temp += (String)PHONE_NUMBER;
  temp += "\"";
  sim_at_cmd(temp);
  sim_at_cmd(F("NGUOI THAN DANG SAY XIN VA CAN GIUP DO! Vi Tri la: http://maps.google.com/maps?q=loc:"));
  sim_at_cmd(String(flat,6) +"," + String(flon,6));
  delay(1000);
  // End charactor for SMS
  sim_at_send(0x1A);
  delay(1000);
}
void setup() {
  Serial.begin(9600);
  Serial.println("ok 1");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.display();
  Serial.println("ok 2");
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.println(F("  TRUY CAP IP  "));
  display.println(F("   192.168.4.1   "));
  Serial.println("ok 3");
  display.println(("WIFI: HAS project"));
  display.println(("PASS: 12345678"));
  display.display();
  pinMode(xacnhan, INPUT_PULLUP);
  pinMode(dongy, INPUT_PULLUP);
  pinMode(thoat, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  delay(500);
  simSerial.begin(MCU_SIM_BAUDRATE);
  delay(5000);
  /*
   sim_at_cmd("AT");
  // Product infor
  sim_at_cmd("ATI");
  // Check SIM Slot
  sim_at_cmd("AT+CPIN?");
  // Check Signal Quality
  sim_at_cmd("AT+CSQ");
  sim_at_cmd("AT+CIMI");
  //sent_sms();
  */
  //delay(60000);
  dht.begin();
  dht1.begin();
  for (int tm = 0; tm <= 50; tm++) {
    Sets = dht.readHumidity();
    //Serial.println(Set);
    delay(10);
    Sets = Sets +2;
  }
 
}
String tt = "   CHUA THOI    ";
long timee;
void loop(){
  double sensorVoltage;
  int UV_index;
  String quality = "";
  sensorVoltage = analogRead(A2);
  sensorVoltage = sensorVoltage / 1024 * 5.0;
  UV_index = sensorVoltage / 0.1;
  double sum = analogRead(MQ3) ;
  sum = (sum/1024.0) * 10.0;
  sum = sum - 1.6;
  Serial.println(sum);
  if (sum <= 0 ) sum = 0;
  
  if (millis() - timee >= 30000) {
    Sets = dht.readHumidity();
    //Serial.println(Sets);
    delay(100);
    timee = millis();
    Sets = Sets +2;
  }
  int h = dht.readHumidity();
  
  t = dht1.readTemperature();
  h1 = dht1.readHumidity();
  //Serial.println(mdb);
 Serial.print(h);
  Serial.print(" ----- ");
  Serial.println(Sets);
  if (h >= Sets) {
    tt = "    DA THOI     ";
    // Serial.println("DA THOI");
  }
  if (sum >= 0.25) {
    k = 1;
  }
  if (digitalRead(xacnhan) == 1 || sum >= 0.25) {
    digitalWrite(relay, LOW);
  }

  else if (h >= Sets && digitalRead(xacnhan) == 0 && sum < 0.25) {
    digitalWrite(relay, HIGH);
  }
  ///////////////////GPS///////////////////////
  //smartDelay(1000);
  /* Serial.print(flat);
    Serial.print("           ");
    Serial.print(flon);
    Serial.println("           ");
  */

  ////////////////////////////////////////////////////////////////////////

  if (k == 1) {
  display.setTextSize(1);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(F("NONG DO CON QUA CAO"));
    display.setCursor(0, 8);
    display.print(F("B: "));
    display.print(sum);
    display.print(F(" mg/l"));
    display.setCursor(0, 16);
    display.print(F("NHAN OK DE GUI HO TRO"));
    display.setCursor(0, 24);
    display.print(F("NHAN EXIT DE TRO VE"));
    //Serial.println(F("NONG DO CON CAO"));
    display.display();
    if (digitalRead(dongy) == 0) {
      Serial.println(F("DONG Y"));
      sent_sms();
      k = 0;
    }
    if (digitalRead(thoat) == 0) {
      Serial.println(("THOAT"));
      k = 0;
    }
  }
  if (k == 0) {
    display.clearDisplay();
    // Display sensor data on the OLED screen
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    // Set the cursor positions and print your data
    display.setCursor(0, 0);
    display.print(("H: "));
    display.print(h1);

    display.setCursor(40, 0);
    display.print(("T: "));
    display.print(t,1);
    
    display.setCursor(90, 0);
    display.print(("U: "));
    display.print(UV_index, 1);
    display.setCursor(0, 10);
    display.print(("B: "));
    display.print(sum);
    display.print((" mg/l"));
    display.setCursor(0, 20);
    display.print(("trang thai: "));
    display.setCursor(48, 20);
    display.print(tt);
    // Update the OLED display
    display.display();
  }


}
