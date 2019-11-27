/*
Codigo: Matias Caparros
Para observatorio UTN FRM

Basado en ejemplos de arduino (web repeater client, etc)
 
 */
// Se importan las librerías
#include <SFE_BMP180.h> //Cargo la libreria del BMP180
#include "DHT.h" //Cargo la libreria del DHT11
#include <Wire.h>

#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN 2 //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT11 //Se selecciona el DHT11 (hay otros DHT)

DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

//Se declara una instancia de la librería
SFE_BMP180 pressure;

//Se declaran las variables. Es necesario tomar en cuenta una presión inicial
//esta será la presión que se tome en cuenta en el cálculo de la diferencia de altura
double PresionBase;

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192,168,0,177);

// fill in your Domain Name Server address here:
//IPAddress myDns(1,1,1,1);

// initialize the library instance:
EthernetClient client;

char server[] = "www.openmeteoarg.890m.com";

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 5000;  // delay between updates, in milliseconds

//Declaro todas las variables que voy a utilizar
double Presion = 0;
double Altura = 0;
double Temperatura = 0;
float templm = 0;
float tempdht = 0;
float tempbmp = 0;
float tempprom = 0;
float presion = 0;
float humedadamb = 0;
float humedadsuelo1 = 0;
float humedadsuelo2 = 0;
float humedadsuelo3 = 0;
float luz = 0;
float procio = 0;
float velocidadviento = 0;
float stermica = 0;
//Declaracion de la variable que almacena y convierte el valor de la luminosidad
double Light (int RawADC0){
double Vout=RawADC0*0.0048828125;
int lux=500/(10*((5-Vout)/Vout));
return lux;
}
//FIN DE VARIABLES

char status;


void setup() {
  // start serial port:
  Serial.begin(9600);
  SensorStart();

  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  //Primero realizo la lectura de todos los sensores
  
  ReadSensor(); //Leo presion y temperatura BMP180
  leerDHT(); //Leo humedad y temperatura DHT11
  
  //Lectura del sensor LM35
  float Temp = analogRead(A0);
  templm = (Temp/1024.0)*500;
  
  //Lectura de la cantidad de luz en el ambiente
  luz = Light(analogRead(1));
  
  //Lectura de la humedad del suelo
  float Suelo1 = analogRead(A2);
  humedadsuelo1 = ((Suelo1*100)/1023);
  
  //Calculo y almacenamiento de punto de rocio
  procio = ((pow(humedadamb/100,1/8)*(112+(0.9*templm)))+(0.1*templm)-112);
  
  //Convierto valores de tipo double a float
  tempbmp = int(Temperatura);
  presion = int(Presion);
  
  Serial.println(tempbmp);
  
  //Calculo de la temperatura promedio
  tempprom = (templm+tempdht+tempbmp)/3;
  
  
  if (client.available()) {
    char c = client.read();
    //Serial.print(c);
    Serial.println("Dato enviado");
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /estacionarduino.php/?id=Rivadavia");
    client.print("&templm=");
    client.print(templm);
    client.print("&tempdht=");
    client.print(tempdht);
    client.print("&tempbmp=");
    client.print(tempbmp);
    client.print("&tempprom=");
    client.print(tempprom);
    client.print("&presion=");
    client.print(presion);
    client.print("&humedadamb=");
    client.print(humedadamb);
    client.print("&humedadsuelo1=");
    client.print("Sensor desconectado");
    client.print("&humedadsuelo2=");
    client.print("Sensor desconectado");
    client.print("&humedadsuelo3=");
    client.print("Sensor desconectado");
    client.print("&luz=");
    client.print("Sensor desconectado");
    client.print("&procio=");
    client.print(procio);
    client.print("&velocidadviento=");
    client.print("Sensor desconectado");
    client.print("&stermica=");
    client.print("Sensor desconectado");
    client.println(" HTTP/1.1");
    /////
    client.println("Host: www.openmeteoarg.890m.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

void SensorStart() {
 
  //Secuencia de inicio del sensor
 
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
 
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1);
  }
 
  //Se inicia la lectura de temperatura
  status = pressure.startTemperature();
  if (status != 0)  {
    delay(status);
    //Se lee una temperatura inicial
    status = pressure.getTemperature(Temperatura);
    if (status != 0)    {
      //Se inicia la lectura de presiones
      status = pressure.startPressure(3);
      if (status != 0)      {
        delay(status);
        //Se lee la presión inicial incidente sobre el sensor en la primera ejecución
        status = pressure.getPressure(PresionBase, Temperatura);
      }
    }
  }
}
 
 
void ReadSensor() {
  //En este método se hacen las lecturas de presión y temperatura y se calcula la altura
 
  //Se inicia la lectura de temperatura
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    //Se realiza la lectura de temperatura
    status = pressure.getTemperature(Temperatura);
    if (status != 0)
    {
      //Se inicia la lectura de presión
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        //Se lleva a cabo la lectura de presión,
        //considerando la temperatura que afecta el desempeño del sensor
 
        status = pressure.getPressure(Presion, Temperatura);
        if (status != 0)
        {
          //Se hace el cálculo de la altura en base a la presión leída en el Setup
          Altura = pressure.altitude(Presion, PresionBase);
        }
        else Serial.println("error en la lectura de presion\n");
      }
      else Serial.println("error iniciando la lectura de presion\n");
    }
    else Serial.println("error en la lectura de temperatura\n");
  }
  else Serial.println("error iniciando la lectura de temperatura\n");
 
}

void leerDHT() {
  humedadamb = dht.readHumidity();
  tempdht = dht.readTemperature();
}




