/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: practica 4 - RFID
   Dev: Jefferson Yahir Gonzàlez Gonzàlez
   Fecha: 26 de julio.
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9
#define SS_PIN    10
#define Verde   3  
#define Rojo    2   
#define buzz    4

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte tagActual[4];
byte tagConocido[4] = { 0x03,0x33, 0xAC, 0x0D};  // UID conocido

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(Verde, OUTPUT);
  pinMode(Rojo, OUTPUT);
  pinMode(buzz, OUTPUT);
  digitalWrite(Rojo, HIGH); 
  digitalWrite(Verde, LOW);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) 
    return;

  if (!mfrc522.PICC_ReadCardSerial()) 
    return;

  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    tagActual[i] = buffer[i];
  }
  Serial.println(); 
  if (compararArray(tagActual, tagConocido)) {
    Serial.println(" Acceso Permitido");
    digitalWrite(Verde, HIGH);
    digitalWrite(Rojo, LOW);
    tone(buzz, 200);
    delay(4000);
    noTone(buzz);
    digitalWrite(Verde, LOW);
    digitalWrite(Rojo, HIGH);


  } 
  else {
    Serial.println(" Desconocido");
    digitalWrite(Verde, LOW);
    digitalWrite(Rojo, HIGH); // Rojo encendido para tarjeta desconocida
    tone(buzz, 400);
    delay(1000);
    noTone(buzz);
  }
}

boolean compararArray(byte array1[], byte array2[]) {
  for (byte i = 0; i < 4; i++) {
    if (array1[i] != array2[i]) return false;
  }
  return true;
}
