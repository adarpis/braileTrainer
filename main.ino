#include "DFRobotDFPlayerMini.h"

int ledPin = 5;
int motor = 27;
int volme = 4;
int volma = 0;
int volumen = 20;
int play = 21;
int adkey1 = 25;
int adkey2 = 26;
int io2 = 2;
int io1 = 15;
int busy = 36;

int idioma = 1;
int fila = -1;
const int Fin_de_carrera[] = {14, 19, 12, 23, 13, 18};

DFRobotDFPlayerMini myDFPlayer;
HardwareSerial Serial2(2);

// #########################################################
int vector[][6] = {
  {1, 0, 1, 1, 1, 1},//a
  {1, 0, 1, 0, 1, 1},//b
  {0, 0, 1, 1, 1, 1},//c
  {0, 0, 0, 1, 1, 1},//d
  {1, 0, 0, 1, 1, 1},//e
  {0, 0, 1, 0, 1, 1},//f
  {0, 0, 0, 0, 1, 1},//g
  {1, 0, 0, 0, 1, 1},//h
  {0, 1, 1, 0, 1, 1},//i
  {0, 1, 0, 0, 1, 1},//j
  {1, 0, 1, 1, 1, 0},//k
  {1, 0, 1, 0, 1, 0},//l
  {0, 0, 1, 1, 1, 0},//m
  {0, 0, 0, 1, 1, 0},//n
  {1, 0, 0, 1, 1, 0},//o
  {0, 0, 1, 0, 1, 0},//p
  {0, 0, 0, 0, 1, 0},//q
  {1, 0, 0, 0, 1, 0},//r
  {0, 1, 1, 0, 1, 0},//s
  {0, 1, 0, 0, 1, 0},//t
  {1, 0, 1, 1, 0, 0},//u
  {1, 0, 1, 0, 0, 0},//v
  {0, 0, 1, 1, 0, 0},//x
  {0, 0, 0, 1, 0, 0},//y
  {1, 0, 0, 1, 0, 0},//z
};
int entradas[] = {0, 0, 0, 0, 0, 0};
// #########################################################

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(Fin_de_carrera[0], INPUT);
  pinMode(Fin_de_carrera[1], INPUT);
  pinMode(Fin_de_carrera[2], INPUT);
  pinMode(Fin_de_carrera[3], INPUT);
  pinMode(Fin_de_carrera[4], INPUT);
  pinMode(Fin_de_carrera[5], INPUT);
  pinMode(volme, INPUT);
  pinMode(volma, INPUT);
  pinMode(play, INPUT);

  pinMode(busy, INPUT);
  //pinMode(io2, OUTPUT);
  //pinMode(io1, OUTPUT);
  //pinMode(adkey1, OUTPUT);
  //pinMode(adkey2, OUTPUT);

  //digitalWrite(adkey1, LOW);
  //digitalWrite(adkey1, LOW);
  //digitalWrite(io2, HIGH);
  //digitalWrite(io1, HIGH);

  Serial.begin(115200);
  Serial2.begin(9600);

  if (!myDFPlayer.begin(Serial2)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(volumen);  //Set volume value. From 0 to 30
}
// ===========================================================
int reconocer_letras()
{
  int controlador = -1;
  for (int i = 0; i < 25; i++)
  {
    if (entradas[0] == vector[i][0] &&
        entradas[1] == vector[i][1] &&
        entradas[2] == vector[i][2] &&
        entradas[3] == vector[i][3] &&
        entradas[4] == vector[i][4] &&
        entradas[5] == vector[i][5])
    {
      controlador = i;
      break;
    }
  }
  return controlador;
}
// ===========================================================
void leer_entradas()
{
  for (int i = 0; i < 6 ; i++)
  {
    entradas[i] = digitalRead(Fin_de_carrera[i]);
    //Serial.println("Entrada " + String(i) + " tiene como resultado " + String(entradas[i]));
  }
}
// ===========================================================
void reproducir(int carpeta, int track)
{
  myDFPlayer.playFolder(carpeta, track);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  delay(1000);
}

// ===========================================================
void loop()
{
  if (digitalRead(volma) == LOW && digitalRead(volme) == HIGH)
  { // Check if button has been pressed
    volumen = volumen + 5;
    if (volumen > 30) {
      volumen = 30;
    }
    myDFPlayer.volume(volumen);  //Set volume value. From 0 to 30
    while (digitalRead(volma) == LOW); // Wait for button to be released
  } else if (digitalRead(volme) == LOW && digitalRead(volma) == HIGH)
  { // Check if button has been pressed
    volumen = volumen - 5;
    if (volumen < 0) {
      volumen = 0;
    }
    myDFPlayer.volume(volumen);  //Set volume value. From 0 to 30
    while (digitalRead(volme) == LOW); // Wait for button to be released
  } else if (digitalRead(volma) == LOW  && digitalRead(volme) == LOW)
  { // Check if button has been pressed
    idioma = idioma + 1;
    if (idioma > 3)
      idioma = 1;
    digitalWrite(motor, HIGH);
    delay(100);
    digitalWrite(motor, LOW);
    while (digitalRead(volme) == LOW && digitalRead(volma) == LOW);
  }

  if (digitalRead(play) == LOW) {
    leer_entradas();
    fila = reconocer_letras();
    if (fila == -1) {
      digitalWrite(motor, HIGH);
      delay(500);
      digitalWrite(motor, LOW);

    } else {
      if (idioma == 1) {
        reproducir(1, fila);
      }
      else if (idioma == 2) {
        reproducir(2, fila);
      }
      else if (idioma == 3) {
        reproducir(3, fila);
      }
    }
    while (digitalRead(play) == LOW );
  }

  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}

