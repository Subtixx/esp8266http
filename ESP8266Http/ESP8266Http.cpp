#include "ESP8266Http.h"

HttpResponse Esp8266Http::Post(HttpRequest request)
{
	return Request(request);
	/*
	WiFiClient client;
	if (!client.connect(request.Host.c_str(), request.Port))
	{
		Serial.println("[Esp8266Http]: Host unreachable");
		return HttpResponse();
	}
	client.print("POST " + request.Path + " HTTP/1.1\r\n");
	client.print("Host: " + request.Host + "\r\n");
	for (std::pair<const String, String> headerPair : request.Headers)
	{
		client.print(headerPair.first + ": " + headerPair.second + "\r\n");
	}

	client.print("\r\n");

	client.print(request.QueryString);

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
	*/
}

HttpResponse Esp8266Http::Get(HttpRequest request)
{
	return Request(request);

	#ifdef ARDUINO_ARCH_ESP8266
	/*WiFiClient client;
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
	*/
	#endif

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

HttpResponse Esp8266Http::Request(HttpRequest request)
{
#ifdef _DEBUG
	Serial.print("[Esp8266Http]: Making HTTP ");
	if(request.Type == HttpRequest::POST)
		Serial.print("POST");
	else
		Serial.print("GET");
	Serial.println(" request to "); 
	Serial.println("\tHost: " + request.Host);
	Serial.println("\tPort: " + String(request.Port));
	Serial.println("\tPath: " + request.Path);
	Serial.println("\tQuery: " + request.QueryString);
#endif

	WiFiClient client;
	if (!client.connect(request.Host.c_str(), request.Port))
	{
		Serial.println("[Esp8266Http]: Host " + request.Host + " (Port: " + String(request.Port) + ") unreachable");
		return HttpResponse(-1, "Host unreachable");
	}

	if (request.Type == HttpRequest::POST)
	{
		client.print(String("POST ") + request.Path + " HTTP/1.1\r\n");
	}
	else
		client.print(String("GET ") + request.Path + "?" + request.QueryString + " HTTP/1.1\r\n");

	client.print("Host: " + request.Host + "\r\n");
	for (std::pair<const String, String> headerPair : request.Headers)
	{
		client.print(headerPair.first + ": " + headerPair.second + "\r\n");
#ifdef _DEBUG
		Serial.println(headerPair.first + ": " + headerPair.second);
#endif
	}

	client.print("Connection: close\r\n\r\n");

	if (request.Type == HttpRequest::POST)
	{
		client.print(request.QueryString);
	}

	unsigned long requestSend = millis();
	while (client.available() == 0)
	{
		if (millis() - requestSend > request.Timeout)
		{
			Serial.println("[Esp8266Http]: Connection to host timed out");
			client.stop();
			return HttpResponse(-2, "Host timed out");
		}
	}

	HttpResponse httpResponse;

	// HTTP/1.1 200 OK
	// HTTP/1.1 500 Internal Server Error
	String responseStatusCode = client.readStringUntil('\r');
	int httpVerIndex = responseStatusCode.indexOf(' '); // HTTP/1.1
	httpResponse.StatusCode = responseStatusCode.substring(httpVerIndex, httpVerIndex + 4).toInt();

	#ifdef _DEBUG
	Serial.println("[Esp8266Http]: Response code: " + String(httpResponse.StatusCode));
	#endif

	while (client.available())
	{
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
	if(httpResponse.Headers.count("Content-Length") == 1)
	{
		if(httpResponse.Body.length() != httpResponse.Headers["Content-Length"].toInt())
		{
			Serial.println("[Esp8266Http]: Content length doesn't match body length");
		}
	}
#endif

	#ifdef _DEBUG
	Serial.println("[Esp8266Http]: Body: " + httpResponse.Body);
	#endif
	client.stop();
	return httpResponse;
}
