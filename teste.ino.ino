#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "304";
const char* password = "chuveiro";
int LED = 2;

const char* mqtt_server = "test.mosquitto.org";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* message, unsigned int length) {
  //Serial.print("Message arrived on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //Serial.println();

  if (String(topic) == "Curto/tutorial") {
    //Serial.print("Changing output to ");
    if(messageTemp == "H"){
      //Serial.println("on");
      digitalWrite(LED, HIGH);
    }
    else if(messageTemp == "L"){
      //Serial.println("off");
      digitalWrite(LED, LOW);
    }
  }
}

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client11111")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Curto/tutorial");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
  //client.subscribe("Curto/tutorial");
}
