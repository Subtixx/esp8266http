#ifndef _ESP8266Http_h
#define _ESP8266Http_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <map>

struct HttpRequest
{
	const String Host;
	const int Port;
	const String Path;
	const int Timeout;
	std::map<String, String> Headers;

	HttpRequest(String host, int port, String path, int timeout = 5000) :
		Host(host),
		Port(port),
		Path(path),
		Timeout(timeout),
		Headers({})
	{
		Headers.insert(std::pair<String, String>(String("User-Agent"), String("Esp8266Http/1.0.0")));
		Headers.insert(std::pair<String, String>(String("Accept"), String("*/*")));
	}

	HttpRequest(String host, int port, String path, std::map<String, String> headers, int timeout = 5000) :
		Host(host),
		Port(port),
		Path(path),
		Timeout(timeout),
		Headers(headers)
	{
	}

	void AddHeader(String key, String value)
	{
		if(Headers.find(key) != Headers.end())
		{
			Serial.println("[Esp8266Http] Header " + key + " is added twice.");
			return;
		}
		Headers.insert(std::pair<String, String>(key, value));
	}
};

struct HttpResponse
{
	String StatusCode;
	String Body;
	std::map<String, String> Headers;

	HttpResponse(String body, std::map<String, String> headers) :
		Body(body),
		Headers(headers)
	{
	}

	HttpResponse() :
		StatusCode(-1),
		Body(""),
		Headers({})
	{
	}
};

class Esp8266Http
{
	static String ParseResponse(String response);

public:
	static String Get(String url);

	/**
	 * \brief Performs a synchronous HTTP GET Request
	 * \param request 
	 * \return 
	 */
	static HttpResponse Get(HttpRequest request);
};


#endif
