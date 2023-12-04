// Set up the rgb led names
uint8_t ledR = 2;
uint8_t ledG = 4;
uint8_t ledB = 5;

bool ledsOn = false; // Variable para controlar el estado de los LEDs

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  // Encender los tres LEDs RGB al mismo tiempo
  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
  ledsOn = true;

  // Esperar 5 segundos
  delay(5000);

  // Apagar los tres LEDs RGB
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);
  ledsOn = false;
}

// void loop runs over and over again
void loop() {
  // Este código está vacío ya que los LEDs se encienden y apagan en el setup
}
