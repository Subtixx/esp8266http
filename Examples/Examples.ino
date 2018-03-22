#include "WiFiCredentials.h"
#include <ESP8266WiFi.h>
#include <ESP8266Http.h>

#include "WiFiCredentials.h"

void setup() {
	Serial.begin(115200);
	delay(10);
	
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	HttpResponse response = Esp8266Http::Get(HttpRequest("192.168.178.26", 80, "/api.php?ver=1.1"));
}

void loop() {
  
}
