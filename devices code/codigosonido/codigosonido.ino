#include <SoftwareSerial.h>

SoftwareSerial mySerial(16, 17); // RX, TX (ajusta los pines según tu conexión)

// Comandos del DFPlayer Mini MP3
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_SONG 0x03
#define CMD_SET_VOLUME 0x06
void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  delay(1000);

  sendCommand(CMD_SEL_DEV, 0, 1); // Seleccionar dispositivo (tarjeta SD)
  delay(500);

}

void loop() {
    playSongOnSD(1); // Número de la canción que deseas reproducir
    delay(31000);
  // Puedes agregar otras lógicas aquí si es necesario
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

void setVolume(uint8_t volume) {
  sendCommand(CMD_SET_VOLUME, 50, volume);
}
