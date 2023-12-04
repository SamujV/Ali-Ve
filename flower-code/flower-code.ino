/******************************************

  This example works for both Industrial and STEM users.

  Developed by Jose Garcia, https://github.com/jotathebest/

** ****************************************/
/****************************************
  Include Libraries
****************************************/
#include "UbidotsEsp32Mqtt.h"
#include "DHT.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(16, 17); // RX, TX (ajusta los pines según tu conexión)

// Comandos del DFPlayer Mini MP3
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_SONG 0x03
#define CMD_SET_VOLUME 0x06

/****************************************
  Define Constants
****************************************/
// Define stepper motor connections and steps per revolution:
#define dirPin 18   //right to left 3
#define stepPin 19   //right to left 4
#define stepsPerRevolution 1000
#define en 13

//**********************************************************
// Define Dht11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//***********************************************************



// connection settings
const char *UBIDOTS_TOKEN = "BBFF-5ZbbA3wyWGJmJ3uoek9FCQLFKg5Srf"; // Put here your Ubidots TOKEN
const char *WIFI_SSID = "VIVIESCAS"; // Put here your Wi-Fi SSID
const char *WIFI_PASS = "14442547"; // Put here your Wi-Fi password

//label device
const char *DEVICE_LABEL = "iot_device_No14"; // Put here your Device labelto which data will be published, replace No with your Kit-IoT box number

//Variables
const char *VARIABLE_TEMP = "temperature";
const char *VARIABLE_HUM = "humidity";
const char *VARIABLE_SERVO = "servo";
const char *VARIABLE_LED = "led";

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds


// Set up the rgb led names
uint8_t ledR = 2;
uint8_t ledG = 34;
uint8_t ledB = 35;

bool ledsOn = false; // Variable para controlar el estado de los LEDs

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


// SOUND
    sendCommand(CMD_SEL_DEV, 0, 1); // Seleccionar dispositivo (tarjeta SD)

//LEDS
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  //Servo 
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW); // Activate with LOW
  

  value = 0;
  pos = 0;
  servo = 0;
  ledValue = 0;
  servoTemp = 0;



}

void openFlower()
{
  // Encender los tres LEDs RGB al mismo tiempo
  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
  ledsOn = true;

  
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
  
  }

void closeFlower()
{
  // Apagar los tres LEDs RGB
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);
  ledsOn = false;
  
   // Move back (counterclockwise):
  digitalWrite(dirPin, LOW);
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
  }


  // Enviar comando al DFPlayer Mini MP3
void sendCommand(uint8_t command, uint8_t param1, uint8_t param2) {
  uint8_t data[10] = {0x7E, 0xFF, 0x06, command, 0x00, param1, param2, 0xEF};
  mySerial.write(data, 8);
}

// Reproducir una canción desde la tarjeta SD
void playSongOnSD(uint16_t number) {
  sendCommand(CMD_PLAY_SONG, highByte(number), lowByte(number));
  delay(1000); // Esperar un segundo para asegurarse de que se inicie la reproducción
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
  value = touchRead(14);
  Serial.print("Touch value: ");
  Serial.println(value);
  if (value <= 40) {
    
   openFlower();

     playSongOnSD(1);
     delay(31000);

   closeFlower();
  }


  /****************************************
    UBIDOTS CONTROL
  ****************************************/

//LED CONTROL
  if (ledValue == 0 && servoTemp != 1) {
  }
  else {
  }


// FLOWER CONTROL

  if (servo == 1 && pos == 0) {   // OPEN FLOWER
   openFlower();
    servoTemp = 1;
  }

  if (servo == 0 && servoTemp == 1) { // CLOSE FLOWER
    closeFlower();
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
