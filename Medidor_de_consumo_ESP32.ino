/************************************************************************
 * Este programa fue elaborado por el estudiante Miguel Angel           *
 * para la Universidad Tecnologica Emiliano Zapata (UTEZ) como proyecto *
 * interno para la liberación de Estadias                               *
 *                                                                      *
 * Programa que realiza mediciones de consumo electrico utilizando      *
 * sensores PZEM004Tv30 y envia los datos obtenidos a la plataforma     *
 * ThingsBoard por medio de una placa ESP32                             *
 *                                                                      *
 * VERSIONES DE LIBRERIAS                                               *
 * ArduioHttpClient v0.4.0 - Arduino                                    *
 * ArduinoJSON v6.10.1 - Benoit Blanchon                                *
 * PubSubClient v2.6.0 - Nick O'Leary                                   *
 * PZEM004Tv30 v1.1.1 - Jakub Mandula                                   *
 * TBPubSubClient v2.9.1 - ThingsBoard Team                             *
 * ThingsBoard v0.4.0 - Thingsoard Team                                 *
 *                                                                      *
 * VERSION DE GESTOR DE PLACA                                           *
 * esp32 v1.0.6 - Espressif Systems                                     *
 * https://dl.espressif.com/dl/package_esp32_index.json                 *
 *                                                                      *
 * Fecha de realización Mayo/2023                                       *
 ************************************************************************/
 
//Librerias
#include <Arduino.h>
#include "ThingsBoard.h"
#include <WiFi.h>
#include <PZEM004Tv30.h>

//Definicion de pines y serial utilizado
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2

//Se crea la instancia pzems
PZEM004Tv30 pzemL1;
PZEM004Tv30 pzemL2;
PZEM004Tv30 pzemL3;

//Configuracion del nombre y contraseña Wifi
#define WIFI_AP             /*"LG K20"*/        "OpenWrt"
#define WIFI_PASSWORD       /*"1234567890"*/    "1OpenWrt!"

//Congiguracion para Thingsboard
#define TOKEN               "dBhLaHi0vLO22at11XLt"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

//Velocidad a la que trabajara nuestra placa
#define SERIAL_DEBUG_BAUD   115200

//Inicializa el cliente ThingsBoard
WiFiClient espClient;
ThingsBoard tb(espClient);

//Declaración del estatus del Wifi
int status = WL_IDLE_STATUS;

//Configuracion de variables para Leds
int conexion = 2; //PIN D2 - GPIO2
int enviar = 4; //PIN D4 - GPIO4

void setup() {
  //Configuración de pines como salida
  pinMode(conexion, OUTPUT);
  pinMode(enviar, OUTPUT);
  
  //Inicializa el serial y el wifi
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();

  //Inicializa el arreglo de PZEM
  pzemL1 = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x02);
  pzemL2 = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x03);
  pzemL3 = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x04);
}

void loop() {
  //Tiempo entre cada ciclo
  delay(5000);

  //Instruccion para reconectar el wifi
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  //Conexion con Thingsboard
  if (!tb.connected()) {
    
    Serial.print("Conectando a: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" Con el TOKEN ");
    Serial.println(TOKEN);
    
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Fallo la conexion");
      digitalWrite(conexion, LOW);
      return;
    }
  }

  //Lee los datos del sensor
  float voltsL1 = pzemL1.voltage();
  float ampsL1 = pzemL1.current();
  float wattsL1 = pzemL1.power();
  float kiloWattsL1 = pzemL1.energy();
  float hertzL1 = pzemL1.frequency();
  float factorL1 = pzemL1.pf();
  
  float voltsL2 = pzemL2.voltage();
  float ampsL2 = pzemL2.current();
  float wattsL2 = pzemL2.power();
  float kiloWattsL2 = pzemL2.energy();
  float hertzL2 = pzemL2.frequency();
  float factorL2 = pzemL2.pf();
  
  float voltsL3 = pzemL3.voltage();
  float ampsL3 = pzemL3.current();
  float wattsL3 = pzemL3.power();
  float kiloWattsL3 = pzemL3.energy();
  float hertzL3 = pzemL3.frequency();
  float factorL3 = pzemL3.pf();
  
  //Imprime la direccion del PZEM L1
  Serial.print("PZEM L1");
  Serial.print(" - Direccion:");
  Serial.println(pzemL1.getAddress(), HEX);
  
  //Imprimir mediciones del PZEM
  Serial.print("Voltaje L1: "); Serial.print(voltsL1,3); Serial.println("V");
  Serial.print("Corriente L1: "); Serial.print(ampsL1,3); Serial.println("A");
  Serial.print("Potencia L1: "); Serial.print(wattsL1,3); Serial.println("W");
  Serial.print("Energia L1: "); Serial.print(kiloWattsL1,3); Serial.println("kWh");
  Serial.print("Frecuencia L1: "); Serial.print(hertzL1,2); Serial.println("Hz");
  Serial.print("FP L1: "); Serial.println(factorL1,3);
  Serial.println(" ");

  //Imprime la direccion del PZEM L2
  Serial.print("PZEM L2");
  Serial.print(" - Direccion:");
  Serial.println(pzemL2.getAddress(), HEX);
  
  //Imprimir mediciones del PZEM L2
  Serial.print("Voltaje L2: "); Serial.print(voltsL2,3); Serial.println("V");
  Serial.print("Corriente L2: "); Serial.print(ampsL2,3); Serial.println("A");
  Serial.print("Potencia L2: "); Serial.print(wattsL2,3); Serial.println("Wa");
  Serial.print("Energia L2: "); Serial.print(kiloWattsL2,3); Serial.println("kWh");
  Serial.print("Frecuencia L2: "); Serial.print(hertzL2,2); Serial.println("Hz");
  Serial.print("FP L2: "); Serial.println(factorL2,3);
  Serial.println(" ");

  //Imprime la direccion del PZEM L3
  Serial.print("PZEM L3");
  Serial.print(" - Direccion:");
  Serial.println(pzemL3.getAddress(), HEX);
  
  //Imprimir mediciones del PZEM L3
  Serial.print("Voltaje L3: "); Serial.print(voltsL3,3); Serial.println("V");
  Serial.print("Corriente L3: "); Serial.print(ampsL3,3); Serial.println("A");
  Serial.print("Potencia L3: "); Serial.print(wattsL3,3); Serial.println("W");
  Serial.print("Energia L3: "); Serial.print(kiloWattsL3,3); Serial.println("kWh");
  Serial.print("Frecuencia L3: "); Serial.print(hertzL3,2); Serial.println("Hz");
  Serial.print("FP L3: "); Serial.println(factorL3,3);
  Serial.println(" ");
  
  //Mensaje de envio de datos
  Serial.println("Enviando a la plataforma...");
  Serial.println("");

  //Envio de datos a Thingsboard PZEM L1
  tb.sendTelemetryFloat("Voltaje L1", voltsL1);
  tb.sendTelemetryFloat("Corriente L1", ampsL1);
  tb.sendTelemetryFloat("Potencia L1", wattsL1);
  tb.sendTelemetryFloat("Energia L1", kiloWattsL1);
  tb.sendTelemetryFloat("Frecuencia L1", hertzL1);
  tb.sendTelemetryFloat("FP L1", factorL1);

  //Envio de datos a Thingsboard PZEM L2
  tb.sendTelemetryFloat("Voltaje L2", voltsL2);
  tb.sendTelemetryFloat("Corriente L2", ampsL2);
  tb.sendTelemetryFloat("Potencia L2", wattsL2);
  tb.sendTelemetryFloat("Energia L2", kiloWattsL2);
  tb.sendTelemetryFloat("Frecuencia L2", hertzL2);
  tb.sendTelemetryFloat("FP L2", factorL2);

  //Envio de datos a Thingsboard PZEM L3
  tb.sendTelemetryFloat("Voltaje L3", voltsL3);
  tb.sendTelemetryFloat("Corriente L3", ampsL3);
  tb.sendTelemetryFloat("Potencia L3", wattsL3);
  tb.sendTelemetryFloat("Energia L3", kiloWattsL3);
  tb.sendTelemetryFloat("Frecuencia L3", hertzL3);
  tb.sendTelemetryFloat("FP L3", factorL3);
  
  //Indicador led de envio de datos
  for (int conteo=0; conteo<5; conteo++) {
    digitalWrite(enviar, HIGH);
    delay(50);
    digitalWrite(enviar, LOW);
    delay(50);
  }
  tb.loop();
}

//Conexión Wifi
void InitWiFi()
{
  Serial.println("Conectando a AP...");

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(conexion, LOW);
  }
  Serial.println("Conectado a AP");
  digitalWrite(conexion, HIGH);
}

//Reconexion Wifi
void reconnect() {
  
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      digitalWrite(conexion, LOW);
    }
    Serial.println("Conectado a AP");
    digitalWrite(conexion, HIGH);
  }
}
