/*
in the digital pin line, 3v3 seems to be faulty
in the opposite line, the last ground towards micro USB port seems to be faulty
Servo Motor   D0              16
Red           D1              5
Blue          D2              4
Green         D3              0
Buzzer        D4              2
Hall Sensor   A0              A0

For Lock
  recieve 0
  rotate servo 0
  turn off blue led
  turn on green led and buzzer for 1 second
  turn on blue led, turn off green led
For Unlock
  recieve 1
  rotate servo 90
  turn off blue led
  turn on green led and buzzer for 0.3 seconds, twice with a 0.5 second interval
  turn on blue led, turn off green led

For Intrusion Detection
  if magnet value less than 100
  publish 2
  (either the door was locked or unlocked in the last 10 minutes), then ok
  recieve nothing
  else receive 3
  ring buzzer and blink red led for 10 seconds
Pin	    Code	        Arduino alias	
A0	    A0	          A0	
D0	    GPIO 16	      16	
D1	    GPIO 5	      5	
D2	    GPIO 4	      4	
D3	    GPIO 0	      0	
D4	    GPIO 2	      2	
D5	    GPIO 14	      14	
D6	    GPIO 12	      12
D7	    GPIO 13	      13
D8	    GPIO 15	      15
SD2	    GPIO 9	      9
SD3	    GPIO 10	      10
RX	    GPIO 3	      3
TX	    GPIO 1	      1


Current problems:
- Buzzer does  not low after receving 3, intrusion detection
- ESP may glitch out randomly
- Magnetic Sensor data may not be read and/or sent properly. tip check websocket function evt to possibly send from code. [fixed]
- Possible Power Issue
- Servo Motor Wires
- Heat Shrink


*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Servo.h>
#include <Hash.h>

const char* ssid = "piano";
const char* password = "speakers";

Servo lock_motor;
int sensorPin = A0;
int val;
int flag = 0;
#define red 5
#define blue 4
#define green 0
#define buzzer 2

String WebPage = "<!DOCTYPE html><html><style>input[type=\"text\"]{width: 90%; height: 3vh;}input[type=\"button\"]{width: 9%; height: 3.6vh;}.rxd{height: 90vh;}textarea{width: 99%; height: 100%; resize: none;}</style><script>var Socket;function start(){Socket=new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage=function(evt){document.getElementById(\"rxConsole\").value +=evt.data;}}function enterpressed(){Socket.send(document.getElementById(\"txbuff\").value); document.getElementById(\"txbuff\").value=\"\";}</script><body onload=\"javascript:start();\"> <div><input class=\"txd\" type=\"text\" id=\"txbuff\" onkeydown=\"if(event.keyCode==13) enterpressed();\"><input class=\"txd\" type=\"button\" onclick=\"enterpressed();\" value=\"Send\" > </div><br><div class=\"rxd\"> <textarea id=\"rxConsole\" readonly></textarea> </div></body></html>";

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("");
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, HIGH);
  digitalWrite(buzzer, LOW);
  lock_motor.attach(16);  //D0
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(blue, HIGH);
    digitalWrite(red, LOW);
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", []() {
    server.send(200, "text/html", WebPage);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (Serial.available() > 0) {
    char c[] = { (char)Serial.read() };
    webSocket.broadcastTXT(c, sizeof(c));
  }
  if (int(analogRead(sensorPin)) < 135 && int(analogRead(sensorPin)) > 115) {  //door open
      Serial.println("door has been opened. is this value of flag 0?");
    if (flag == 0) {
      Serial.println("as Value of flag is 0 sending 2");
      Serial.print("Value of magnet: ");
      Serial.println(int(analogRead(sensorPin)));  //door opened
      char valueToSend[] = { "2" };
      webSocket.broadcastTXT(valueToSend, sizeof(valueToSend));
      flag = 1;
      Serial.println("value of flag is now set to 1. blocked for intrusion detection until further notice.");
    }
  }
    else{
      Serial.print("-");
      Serial.print(int(analogRead(sensorPin)));
      Serial.print(" ");
    }
  delay(100);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.print("Received: ");
    Serial.println((char*)payload);
    if (strcmp((char*)payload, "0") == 0) {  //lock
      digitalWrite(LED_BUILTIN, HIGH);
      lock_motor.write(0);
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(buzzer, LOW);
      flag = 0;
      Serial.println("value of flag is now set to 0 anyways because user pressed lock. ready for intrusion detection");
    } else if (strcmp((char*)payload, "1") == 0) {  //unlock
      digitalWrite(LED_BUILTIN, LOW);
      lock_motor.write(90);
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(green, LOW);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(green, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(buzzer, LOW);
      flag = 0;
      Serial.println("value of flag is now set to 0 anyways because user pressed unlocked. ready for intrusion detection ");
    } else if (strcmp((char*)payload, "3") == 0) {  // intrusion detected
      Serial.println("Intrusion is detected. Performing rituals");
      lock_motor.write(0);
      digitalWrite(blue, LOW);
      for (int i = 0; i <= 2; i++) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(red, HIGH);
        digitalWrite(buzzer, LOW);
        delay(1000);
        digitalWrite(red, LOW);
        digitalWrite(buzzer, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
      }
      digitalWrite(blue, HIGH);
      digitalWrite(buzzer, LOW);
      flag = 0;
      Serial.println("value of flag is now set to 0. ready for intrusion detection ");
    } else if (strcmp((char*)payload, "4") == 0) {  // the door has been opened, but legitemately recently, so keeping ready for further intrusion 
      flag = 0;
      Serial.println("it seems the door has been opened legitemately recently. this is not an intrusion. value of flag is set to 0, ready for intrusion detection.");
    }
  }
}