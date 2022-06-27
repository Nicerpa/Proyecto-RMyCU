#include <phyphoxBle.h> 
#include <Arduino_LSM9DS1.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>


//Se definen las variables
int red, green, blue, ambient;
char board_name[] = "Arduino Nano 33 BLE"; // Cambiar 
float pressure,temperature, humidity, ambient_float;
float choice = 0.0;
const int analogInPin1 = A0;
const int analogInPin2 = A1;
const int analogInPin3 = A2;
float voltage1, voltage2, voltage3;
unsigned long initial_time, first_time, fourth_time;
float first_difference_float, fourth_difference_float;
unsigned int period = 50;
const int ledPin = 22;
const int ledPin2 = 23;
const int ledPin3 = 24;


void setup(){
  Serial.begin(9600);
  PhyphoxBLE::minConInterval = 6;
  PhyphoxBLE::maxConInterval = 6;
  PhyphoxBLE::slaveLatency = 0;
  PhyphoxBLE::timeout = 10; 
  /*-------------------------------*/
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(13, LOW);  
  // Chequear que los sensores esten funcionando
  if (!BARO.begin()){while (1);}
  if (!HTS.begin()){while (1);}
  /*-------------------------------*/

  PhyphoxBLE::start(board_name);  // Activar la placa Arduino
  PhyphoxBleExperiment myExperiment; // Inicio de experimento
  PhyphoxBleExperiment::View myView; // Crear Vista para los datos del experimento
  myExperiment.setTitle("Estación Meteorológica"); //nombre experimento
  myView.setLabel("Ver Datos");//nombre de la vista
  /*---------- PRIMER GRÁFICO ---------- */
  PhyphoxBleExperiment::Graph pressureGraph;
  pressureGraph.setLabel("Presión v/s Tiempo");
  pressureGraph.setUnitX("[s]");
  pressureGraph.setUnitY("[kPa]");
  pressureGraph.setLabelX("Tiempo");
  pressureGraph.setLabelY("Presión");
  pressureGraph.setChannel(1,4);
  /*---------- SEGUNDO GRÁFICO ---------- */
  PhyphoxBleExperiment::Graph temperatureGraph;
  temperatureGraph.setLabel("Temperatura v/s tiempo");
  temperatureGraph.setUnitX("[s]");
  temperatureGraph.setUnitY("[°C]");
  temperatureGraph.setLabelX("Tiempo");
  temperatureGraph.setLabelY("Temperatura");
  temperatureGraph.setChannel(1,2);
  /*---------- TERCER GRÁFICO ---------- */
  PhyphoxBleExperiment::Graph humidityGraph;
  humidityGraph.setLabel("Humedad v/s tiempo");
  humidityGraph.setUnitX("[s]");
  humidityGraph.setUnitY("%");
  humidityGraph.setLabelX("Tiempo");
  humidityGraph.setLabelY("Humedad");
  humidityGraph.setChannel(1,3);
  
   /*Añadir elementos a la vista y el experimento al servidor de phyphox*/
  myView.addElement(humidityGraph);
  myView.addElement(temperatureGraph);
  myView.addElement(pressureGraph);
  myExperiment.addView(myView);
  PhyphoxBLE::addExperiment(myExperiment);
}

void loop(){
   if (PhyphoxBLE::currentConnections > 0){
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
   }else{
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
   }
    
    SensorReading();

}
void SensorReading() {
  if ((unsigned long)(millis() - first_time) > period*2) {
    first_time = millis();
    temperature = HTS.readTemperature();
    humidity = HTS.readHumidity();
    pressure = BARO.readPressure();
    first_difference_float = ((float)first_time-(float)initial_time)/1000;
    PhyphoxBLE::write(first_difference_float, temperature, humidity, pressure);
  }
}
