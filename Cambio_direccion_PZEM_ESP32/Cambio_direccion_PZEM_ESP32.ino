/************************************************************************
 * Programa que realiza el cambio de direccion a el modulo              *
 * PZEM004Tv3 por medio de una placa ESP8266                            *
 *                                                                      *
 * VERSION DE LIBRERIA                                                  *
 * PZEM004Tv30 v1.1.1 - Jakub Mandula                                   *
 *                                                                      *
 * VERSION DE GESTOR DE PLACA                                           *
 * esp8266 v3.0.2 - ESP8266 Community                                   *
 * https://arduino.esp8266.com/stable/package_esp8266com_index.json     *
 ************************************************************************/

//Libreria a utilizar
#include <PZEM004Tv30.h>

//Definimos los pines y el serial 
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2

//Inicializa del pzem con PZEM_SERIAL y los pines RX y Tx
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

//Se define la direccion para el PZEM
#define SET_ADDRESS 0x04

void setup() {
  //Velocidad a la que trabaja la placa
  Serial.begin(115200);
}

void loop() {
    //Se establece la direccion
    static uint8_t addr = SET_ADDRESS;

    //Imprime la direccion personalizada actual
    Serial.print("Direccion anterior:   0x");
    Serial.println(pzem.readAddress(), HEX);

    //Imprime la direccion personalizada
    Serial.print("Establecer direccion a: 0x");
    Serial.println(addr, HEX);

    //Fallo en la configuracion
    if(!pzem.setAddress(addr))
    {
      //Fallo, probablemente ningun PZEM conectado
      Serial.println("Error al establecer direccion");
    } else {
      //Imprime la nueva direccion personalizada
      Serial.print("Direccion actual:    0x");
      Serial.println(pzem.readAddress(), HEX);
      Serial.println();
    }

    //Reatardo entre ciclos
    delay(5000);
}
