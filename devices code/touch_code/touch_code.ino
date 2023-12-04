// C++ code
//
const int pin =14;
int value=0;
void setup()
{
  pinMode(pin, OUTPUT);
    Serial.begin(115200);

}

void loop()
{ 

  value = touchRead(13);
  Serial.print("Touch value");
  Serial.println(value);
  if(value<=40){
  digitalWrite(pin,HIGH);
  }else{
  digitalWrite(pin,LOW); 
  }
  delay(1000);
}
