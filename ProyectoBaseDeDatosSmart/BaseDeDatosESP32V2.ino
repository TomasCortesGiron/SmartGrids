//Páginas de servicios:
//Página de referencia:
//https://www.youtube.com/watch?v=hvveBxxdx0Q

//Página de servidor gratuito:
//https://co.000webhost.com/

//SitioWeb:
//https://tomascortessitioweb.000webhostapp.com/ESP32Post.php
//Contraseña sitio WEB:
//W(4ybxdTJ#BRBvEN)KH&

//----------------------------------------------------------------Colocamos las bibliotecas--------------------------------------------------------

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

WiFiClient client; 

//Tenemos un objeto para WIFI:
WiFiMulti wifiMulti;

//Bibliotecas de Sebas:
#include <esp_now.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//---------------------VARIABLES GLOBALES-------------------------
//Definimos un tiempo de espera para que detecte una red y se conecte:
const uint32_t TiempoEsperaWifi = 5000;
//Colocamos el servidor:
WiFiServer servidor(80);

//Variables de Sebastián:
#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

//----------------VARIABLES GLOBALES De los DATOS-------------------
//Va a llegar: Orientacion x, orientacion, y, Temperatura, humedad, presion, altitud,  
float orientationX = 5;
float orientationY = 4;
float temperatura = 3;
float humedad = 2;
float presion = 1;
float altitud = 2;


//-------------------- ESTRUCTURA De los DATOS---------------------
// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
    float dato2;
    float dato3;
    float dato4;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};



//---------------- Para las funciones de Sebastián --------------------
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].dato1 = myData.dato1;
  boardsStruct[myData.id-1].dato2 = myData.dato2;
  boardsStruct[myData.id-1].dato3 = myData.dato3;
  boardsStruct[myData.id-1].dato4 = myData.dato4;
  
  Serial.printf("%f",boardsStruct[myData.id-1].dato1);
  Serial.printf("%f",boardsStruct[myData.id-1].dato2);
  Serial.printf("%f",boardsStruct[myData.id-1].dato3);
  Serial.printf("%f",boardsStruct[myData.id-1].dato4);
  Serial.println();
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Nodo1() {
  oled.setCursor(0, 10);       // set position to display
  oled.println("oreintacion X: "+ String(boardsStruct[0].dato1)); // set text
  //Leemos las variables:
  orientationX = boardsStruct[0].dato1;

  oled.setCursor(0, 20);       // set position to display
  oled.println("orientacion Y: "+String(boardsStruct[0].dato2)); // set text
  //Leemos las variables:
  orientationY = boardsStruct[0].dato2;  

  oled.setCursor(0, 30);       // set position to display
  if(boardsStruct[0].dato3 == 0){
    oled.println("el objeto esta lejos"); // set text
  }
  else{
    oled.println("el objeto esta cerca"); // set text
  }
   
  oled.setCursor(0, 40);       // set position to display
  oled.println("T: "+String(boardsStruct[0].dato4)+" C"); // set text

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Nodo2(){
  
  oled.setCursor(0, 10);       // set position to display
  oled.println("Humedad: "+String(boardsStruct[1].dato1)); // set text
  humedad = boardsStruct[1].dato1;

  oled.setCursor(0, 20);       // set position to display
  oled.println("Temperatura: " +String(boardsStruct[1].dato2)+" C"); // set text  
  temperatura = boardsStruct[1].dato2;
  //temperatura = 1;

  oled.setCursor(0, 30);       // set position to display
  oled.println("Presion: "+String(boardsStruct[1].dato3) +" hPa"); // set text 
  presion = boardsStruct[1].dato3;

  oled.setCursor(0, 40);       // set position to display
  oled.println("Altitud: "+String(boardsStruct[1].dato4) + " mts"); // set text
  altitud = boardsStruct[1].dato4;

}


//----------------------------------------------------------------------Setup--------------------------------------------------------------------
void setup() {

  //Pin definido por Sebastián:
  pinMode(4,INPUT);

  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  //De Tomás:
  Serial.begin(115200);
  //Colocamos el mensaje para arrancar la configuración.
  Serial.println("---Inicializando multi WIFI---");
  delay(10);

  MultiRed();

  //Para la parte del servidor:
  Serial.println(WiFi.localIP());
  servidor.begin(); // iniciamos el servidor
  //Fin de Tomás.

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }
  
  if (esp_now_init() != ESP_OK) {
  Serial.println("Error initializing ESP-NOW");
  return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  //!!!Tener cuidado con este tiempo por que se puede colar al tiempo de envío:
  delay(2000);
}


//------------------------------------------------------------------------LOOP------------------------------------------------------------------


void loop() {

  //Código de Sebas
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color

  if(digitalRead(4)== HIGH)
  {
    oled.setCursor(0, 0);       // set position to display
    oled.println("Nodo 1"); // set text
    Nodo1();
  }
  else 
  {
    oled.setCursor(0, 0);       // set position to display
    oled.println("Nodo 2"); // set text
    Nodo2();
  }
  
  oled.display();
  //Fin código de Sebas

  //Enviamos los datos:
  envioDatos();

}


//----------------------------------------------------------------Para el envio de datos-------------------------------------------------------------
void envioDatos(){

  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    //Colocamos en una cadena los datos que vamos a enviar:
    String datos_a_enviar = "orientationX= " + String(orientationX) + "&orientationY= " + String(orientationY) + "&temperatura= " + String(temperatura) +
    "&humedad=" + String(humedad) +"&presion= " + String(presion) + "&altitud=" + String(altitud);

    //Colocamos comentario para verificar que se envíe el mensaje:
    Serial.println("Los datos a enviar son:" + datos_a_enviar);

    //Colocamos la dirección:
    http.begin(client,"http://tomascortessitioweb.000webhostapp.com/ESP32Post.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int codigo_respuesta = http.POST(datos_a_enviar);

    if(codigo_respuesta > 0){

      Serial.println("Código HTTP: " + String(codigo_respuesta));
      if(codigo_respuesta == 200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió: ");
        Serial.println(cuerpo_respuesta);
      }
    }else{
      Serial.print("Error enviado POST, código: ");
      Serial.println(codigo_respuesta);
    }
    http.end();
  }else{
    Serial.println("Error de la conexión WIFI");
  }
  //Enviamos información cada 60 segundos.
  delay(60000);
}





//-----------------------------------------------------------------------------Multi red-------------------------------------------------------------


//Función para conectar WIFI con multiples redes.
void MultiRed(){

  //El siguiente segmento nos ayuda a que el dispositivo nos permita conectarnos a diferentes redes.
    wifiMulti.addAP("flia Cortés giron", "Familia4");
    //Colocar Red de la U
    wifiMulti.addAP("Usergioarboleda", "ceskqyw2012");
    wifiMulti.addAP("LABTINY7053", "12345678");

    WiFi.mode(WIFI_STA);
    Serial.println("Connecting Wifi...");
    //Generamos que el código se refresque:
    while(wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED){
      Serial.print(" (UwU) ");
    }
    
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}
