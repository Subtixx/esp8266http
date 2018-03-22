#include <map>

#include <ESP8266WiFi.h>

#include "ESP8266Http.h"

String Esp8266Http::Get(String url)
{
	/*WiFiClient client;
	if(!client.connect())*/
}

HttpResponse Esp8266Http::Get(HttpRequest request)
{
	WiFiClient client;
	if(!client.connect(request.Host.c_str(), request.Port))
	{
		Serial.println("[Esp8266Http]: Host unreachable");
		return HttpResponse();
	}
	client.print(String("GET ") + request.Path + " HTTP/1.1\r\n");
	client.print("Host: " + request.Host + "\r\n");
	for (std::pair<const String, String> headerPair : request.Headers)
	{
		client.print(headerPair.first + ": " + headerPair.second + "\r\n");
	}

	client.print("Connection: close\r\n\r\n");

	unsigned long requestSend = millis();
	while (client.available() == 0) {
		if (millis() - requestSend > request.Timeout) {
			Serial.println("[Esp8266Http]: Connection to host timed out");
			client.stop();
			return HttpResponse();
		}
	}

	HttpResponse httpResponse;

	String responseStatusCode = client.readStringUntil('\r');
	httpResponse.StatusCode = ParseResponse(responseStatusCode);

#ifdef _DEBUG
	Serial.println("[Esp8266Http]: Response code: " + httpResponse.StatusCode);
#endif

	while (client.available()) {
		String headerLine = client.readStringUntil('\r');
#ifdef _DEBUG
		Serial.println("[Esp8266Http]: Parsing header: " + headerLine);
#endif

		size_t pos = headerLine.indexOf(": ");
		if (pos == -1)
			break;
#ifdef _DEBUG
			Serial.println("[Esp8266Http]: Header: " + headerLine.substring(0, pos) + " - Value: " + headerLine.substring(pos + 2));
#endif
			httpResponse.Headers.insert(std::pair<String, String>(headerLine.substring(0, pos), headerLine.substring(pos + 2)));
	}
	httpResponse.Body = client.readStringUntil('\r');
#ifdef _DEBUG
	Serial.println("[Esp8266Http]: Body: " + httpResponse.Body);
#endif
	client.stop();
	return httpResponse;

	/*
	WiFi connected
	IP address: 
	192.168.178.41
	[Esp8266Http]: Response code: 200
	[Esp8266Http]: Parsing header: 
	Host: 192.168.178.26
	[Esp8266Http]: Header: 
	Host - Value: 192.168.178.26
	[Esp8266Http]: Parsing header: 
	Connection: close
	[Esp8266Http]: Header: 
	Connection - Value: close
	[Esp8266Http]: Parsing header: 
	X-Powered-By: PHP/5.6.18
	[Esp8266Http]: Header: 
	X-Powered-By - Value: PHP/5.6.18
	[Esp8266Http]: Parsing header: 
	Content-type: text/html; charset=UTF-8
	[Esp8266Http]: Header: 
	Content-type - Value: text/html; charset=UTF-8
	[Esp8266Http]: Parsing header: 

	[Esp8266Http]: Body: 
	1.1
	 */
}

String Esp8266Http::ParseResponse(String response)
{
	// HTTP/1.1 200 OK
	// HTTP/1.1 500 Internal Server Error
	int found = 0;
	int maxIndex = response.length() - 1;

	for (int i = 0; i <= maxIndex; i++) {
		if (response.charAt(i) == ' ' || i == maxIndex) {
			return response.substring(i + 1, i + 1 + 3);
		}
	}
	return "";
}