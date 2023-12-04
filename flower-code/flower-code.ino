#include <ESP32Servo.h>
/******************************************

  This example works for both Industrial and STEM users.

  Developed by Jose Garcia, https://github.com/jotathebest/

** ****************************************/
/****************************************
  Include Libraries
****************************************/
#include "UbidotsEsp32Mqtt.h"
#include "DHT.h"
/****************************************
  Define Constants
****************************************/
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SERVOPIN 5
#define LEDPIN 33
Servo myServo;

const char *UBIDOTS_TOKEN = "BBFF-5ZbbA3wyWGJmJ3uoek9FCQLFKg5Srf"; // Put here your Ubidots TOKEN
const char *WIFI_SSID = "VIVIESCAS"; // Put here your Wi-Fi SSID
const char *WIFI_PASS = "14442547"; // Put here your Wi-Fi password

const char *DEVICE_LABEL = "iot_device_No14"; // Put here your Device labelto which data will be published, replace No with your Kit-IoT box number

const char *VARIABLE_TEMP = "temperature";
const char *VARIABLE_HUM = "humidity";
const char *VARIABLE_SERVO = "servo";
const char *VARIABLE_LED = "led";

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

uint8_t tempPin = DHTPIN;
unsigned long timer;

int servo;
int ledValue;
int pin;
int value;
int pos;
int servoTemp;
String payloadValue;
char* globalTopic;
const char *topicServo = "/v2.0/devices/iot_device_no14/servo/lv" ;
const char *topicLed = "/v2.0/devices/iot_device_no14/led/lv" ;


Ubidots ubidots(UBIDOTS_TOKEN);
/****************************************
  Auxiliar Functions
****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int compareServo = strcmp(topicServo, topic);
  int compareLed = strcmp(topicLed, topic);
  payloadValue = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    payloadValue += (char)payload[i];
  }
  Serial.println(payloadValue);

  if (compareServo == 0) {
    servo = payloadValue.toInt();
  }
  else if (compareLed == 0) {
    ledValue = payloadValue.toInt();
  }
}

/****************************************
  Main Functions
****************************************/
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true); // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_SERVO);
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LED);
  timer = millis();

  Serial.println("------------------------------");
  Serial.println(F("DHTxx test!"));
  dht.begin();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);    // standard 50 hz servo
  myServo.attach(SERVOPIN);     // attaches the servo on pin 18 to the servo object
  pinMode(LEDPIN, OUTPUT);

  value = 0;
  pos = 0;
  servo = 0;
  ledValue = 0;
  servoTemp = 0;



}
void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_SERVO);
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LED);
  }

  /****************************************
    TOUCH INTERACTION
  ****************************************/
  value = touchRead(13);
  if (value <= 40) {
    digitalWrite(LEDPIN, HIGH);

    for (pos = 0; pos <= 180; pos += 1) { // OPEN  FLOWER
      myServo.write(pos);
      delay(45);
    }

    delay(3000); // time to wait to listen to the music

    for (pos = 180; pos >= 0; pos -= 1) { // CLOSE FLOWER
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(45);                       // waits 15ms for the servo to reach the position
    }
    pos = 0;
    digitalWrite(LEDPIN, LOW);
  }


  /****************************************
    UBIDOTS CONTROL
  ****************************************/

  if (ledValue == 0 && servoTemp != 1) {
    digitalWrite(LEDPIN, LOW);
  }
  else {
    digitalWrite(LEDPIN, HIGH);
  }


  if (servo == 1 && pos == 0) {   // OPEN FLOWER
    digitalWrite(LEDPIN, HIGH);

    for (pos = 0; pos <= 179; pos += 1) { // OPEN  FLOWER
      myServo.write(pos);
      delay(45);

    }

    servoTemp = 1;
  }

  if (servo == 0 && servoTemp == 1) { // CLOSE FLOWER

    for (pos = 179; pos > 0; pos -= 1) {
      myServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(45);                       // waits 15ms for the servo to reach the position

    }

    digitalWrite(LEDPIN, LOW);
    servoTemp = 0;
   
  }





  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // if (isnan(h) || isnan(t)) {
  //  Serial.println(F("Failed to read from DHT sensor!"));
  //   Serial.println(t);
  //    Serial.println(h);
  //      return;
  //  }


  if (abs(double(millis() - timer)) > PUBLISH_FREQUENCY) // triggers the routine every 10 seconds
  {
    Serial.print("Servo value: ");
    Serial.println(servo);
    Serial.print("led value: ");
    Serial.println(ledValue);


    float temp_value = t;
    float humid_value = h;
    Serial.print("temperature: ");
    Serial.println(temp_value);
    Serial.print("humidity: ");
    Serial.println(humid_value);
    Serial.print("servo position: ");
    Serial.println(pos);
    //Serial.println(value);
    ubidots.add(VARIABLE_TEMP, temp_value); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_HUM, humid_value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  ubidots.loop();
}
