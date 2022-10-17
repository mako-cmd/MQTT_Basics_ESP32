//Blink led 2 via Node-Red

#include <WiFi.h>
#include <PubSubClient.h>

#define LED_BUILTIN 2

//Flag used as latch for switching between led on/off states
bool blink_flag = false;

const char* ssid = "Mako's Galaxy Note20 Ultra 5G";
const char* password = "hqhj6705";

const char* mqtt_server = "192.168.33.197";

WiFiClient espClient;
PubSubClient client(espClient);



void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("Running setup");
  Serial.println("");
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
}

//Connect to wifi
void setup_wifi() 
{
  delay(10);
  Serial.println("Running setup_wifi");
  Serial.println("");
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //retry until connected forever
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    
    Serial.println("done");
    Serial.println("");
  }

  //Connect success
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

  //MQTT msg recieved interrupt
void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageBlink;
  
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageBlink += (char)message[i];
  }
    Serial.println();
    Serial.print("Changing output to ");
    if(messageBlink == "on")
    {
      Serial.println("Blink on");
      //Make the LED blink
      blink_flag = true;
    }
    else if(messageBlink == "off")
    {
      Serial.println("Blink off");
      //Stop the LED from blinking
      blink_flag = false;
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Blinking LED");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  if(blink_flag == true)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
  
}
