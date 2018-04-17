#include <ESP8266WiFi.h>

const char* ssid     = "ssid";
const char* password = "password";

void blinkLED(int duration) {
  digitalWrite(LED_BUILTIN, LOW);
  delay(duration);
  digitalWrite(LED_BUILTIN, HIGH);
}

void setupBlink() {
  blinkLED(400);
  delay(200);
  blinkLED(200);
  delay(200);
  blinkLED(400);
}

void connectToWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(115200);
  delay(10);

  setupBlink();
  connectToWiFi();
}

void loop() {
  delay(5000);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char* host   = "192.168.5.4";
  const int httpPort = 5000;
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  const char* url = "/";

  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}
