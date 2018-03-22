#include "WiFiCredentials.h"
#include <ESP8266WiFi.h>
#include <ESP8266Http.h>

// This should contain 2 defines for WIFI_SSID and WIFI_PASS
#include "WiFiCredentials.h"

#pragma region WiFi Connection
// Code in here is just for the connection to your WiFi
void ConnectToWiFi()
{
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
}
#pragma endregion

void setup() {
	Serial.begin(115200);
	delay(10);
	ConnectToWiFi();

	// This request is synchronous.
	HttpResponse response = Esp8266Http::Request(
		HttpRequest(
			HttpRequest::GET, // Specifies that this should be a GET request.
			"https://api.ipify.org" // Specifies the URL where it should make the request to.
		)
	);

	if (response.StatusCode == 200) { // Was request successful?
		// Prints out your IP
		Serial.println(response.Body);
	}
	else {
		Serial.println("There was an error trying to reach the server.");
		if(response.StatusCode < 0) // If there was an error while connecting (Timeout or Unknown host) then body contains an error message.
			Serial.println(response.Body);
	}
}

void loop(){}