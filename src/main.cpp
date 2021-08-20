

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> //libreria para conectarse al servidor mqtt

const char* ssid     = "TP-LINK_51B440";
const char* password = "telecomunicaciones";

const char *mqtt_server = "timbrewifi.ga";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "121212";

WiFiClient espClient; //libreria    ue pertenece al esp32
PubSubClient client(espClient); //declaramos una instancia como cliente, todo lo que hagamos que lo haga a travez de la conexio wifi del esp32

long lastMsg = 0; //guarda ultimo mensaje
char msg[25];//guardo mensajes

int temp1 = 0;
int temp2 = 1;
int volts = 2;

//*****************************
//*** DECLARACION FUNCIONES ***
//*****************************
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
/******************************/
/******************************/

void setup() {
	pinMode(BUILTIN_LED, OUTPUT);//led por defecto, puesto como salida
	Serial.begin(115200);
	randomSeed(micros());//planta semilla, para valores aleatorios
	setup_wifi();//funcion para la conexion
	client.setServer(mqtt_server, mqtt_port);//seteamos los parametros al servidor
	client.setCallback(callback);//funcion 'callback'para que cuando llegue el mensaje, es el que vigila cuando llega el mensaje
}

void loop() {
	if (!client.connected()) {//si no estamos conectados, entonces reconectamos
		reconnect();
	}

	client.loop();//!!este metodo vigila si se ha caido la conexion, o colgado, hay que llamarlo constantemente y no hay que poner ningun delay

	long now = millis();//guarda los milisegundos que han transcurrido desde que arranco la placa
	if (now - lastMsg > 500){ //now - 0 >500
		lastMsg = now;
		temp1++;
		temp2++;
		volts++;

		String to_send = String(temp1) + "," + String(temp2) + "," + String(volts);
		to_send.toCharArray(msg, 25);//hay qie enviarle como charArray
		Serial.print("Publicamos mensaje -> ");
		Serial.println(msg);
		client.publish("values", msg);//enviamos
	}
}



//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi(){
	delay(10);
	// Nos conectamos a nuestra red Wifi
	Serial.println();
	Serial.print("Conectando a ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());
}
/*************************************/
/*************************************/

void callback(char* topic, byte* payload, unsigned int length){//topico, carga, longitud
	String incoming = "";
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i]; //casteamos a char
	}
	incoming.trim();//quitamos espacios
	Serial.println("Mensaje -> " + incoming);

	if ( incoming == "on") {//si es 'on' enciende el led, sino lo apaga
		digitalWrite(BUILTIN_LED, HIGH);
	} else {
		digitalWrite(BUILTIN_LED, LOW);
	}
}

void reconnect() {

	while (!client.connected()) {//si no estamos conectados
		Serial.print("Intentando conexión Mqtt...");
		// Creamos un cliente ID
		String clientId = "esp32_";//este sera el encabezado para el id del dispositivo
		clientId += String(random(0xffff), HEX);//contatenamos la cabecera con un valor randomico
		// Intentamos conectar
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {//pasamos un charArray, el cual es clientId.c_str, porque asi lo pide la funcion
			Serial.println("Conectado!");
			// Nos suscribimos
			client.subscribe("led1");//nos suscribimos a los topicos led1 y led2
			client.subscribe("led2");
		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");

			delay(5000);
		}
	}
}
