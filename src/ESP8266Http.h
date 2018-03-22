#ifndef _ESP8266Http_h
#define _ESP8266Http_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <map>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#else
#error Unsupported architecture.
#endif

struct HttpRequest
{
	enum RequestType
	{
		GET = 0,
		POST
	};

private:
	HttpRequest(RequestType type, String host, int port, String path, std::map<String, String> headers, String body, int timeout = 5000) :
		Type(type),
		Host(host),
		Port(port),
		Path(path),
		Timeout(timeout),
		Headers(headers),
		QueryString(body)
	{
		AddHeader(String("User-Agent"), String("ESP8266Http/1.0.0"));
		AddHeader(String("Accept"), String("*/*"));
		if (type == POST)
		{
			AddHeader("Content-Type", "application/x-www-form-urlencoded");
			AddHeader("Content-Length", String(QueryString.length()));
		}
	}

	/**
	 * \brief Parses a URL to Host, Port, Path and QueryString
	 * (eg. http://192.168.178.26:8080/api.php?username=root&password=123456)
	 */
	void ParseUrl(String url)
	{
		String host;
		int port = 80;
		String path;
		String body;

		// http://google.com:8080/api/request/1
		if (url.startsWith("http://"))
			url = url.substring(7, url.length());

		if (url.startsWith("https://")) {
			url = url.substring(8, url.length());
			port = 443;
		}

		// google.com:8080/api/request/1
#ifdef _DEBUG
		Serial.println("Url: " + url);
#endif

		// Find first /
		int indexOfFirstSlash = url.indexOf("/");

		if (url.indexOf(":") != -1)
		{
			// google.com:8080
			String urlWithPort = url.substring(0, indexOfFirstSlash);

			// 8080
			port = urlWithPort.substring(urlWithPort.indexOf(":") + 1, urlWithPort.length()).toInt();

			// google.com
			host = urlWithPort.substring(0, urlWithPort.indexOf(":"));

			// /api/request/1
			path = url.substring(indexOfFirstSlash, url.length());
		}
		else
		{
			//google.com/api/request/1

			// google.com
			host = url.substring(0, indexOfFirstSlash);

			// /api/request/1
			path = url.substring(indexOfFirstSlash, url.length());
		}

		if(path.indexOf("/") == -1)
		{
			path = "/" + path;
		}

		// /api/request?test=t
		if(path.indexOf("?") != -1)
		{
			int queryIndex = path.indexOf("?");

			// ?test=t
			body = path.substring(queryIndex+1, path.length());

			// /api/request
			path = path.substring(0, queryIndex);
		}

#ifdef _DEBUG
		Serial.println("Host: " + host);
		Serial.println("Port: " + String(port));
		Serial.println("Path: " + path);

		if (body != "")
			Serial.println("Query: " + body);
#endif

		Host = host;
		Path = path;
		Port = port;
		if (body != "") {
			QueryString = body;
			if(Headers.count("Content-Length") == 1)
				Headers["Content-Length"] = String(QueryString.length());
			else
				Headers.insert(std::pair<String, String>("Content-Length", String(QueryString.length())));
		}
	}

public:
	/**
	 * \brief 0 = GET, 1 = POST
	 */
	const RequestType Type;

	/**
	 * \brief The hostname to connect to
	 */
	String Host;

	/**
	 * \brief The port to connect to (HTTP = 80, HTTPS = 443)
	 */
	int Port;

	/**
	 * \brief The path to make the request to.
	 */
	String Path;

	/**
	 * \brief The timeout in Milliseconds
	 */
	const int Timeout;

	/**
	 * \brief Map containing request headers
	 */
	std::map<String, String> Headers;

	/**
	 * \brief Contains either the post body, or the query string for GET requests.
	 */
	String QueryString;

	HttpRequest(String host, int port, String path, int timeout = 5000) :
		HttpRequest(GET, host, port, path, {}, "", timeout)
	{
	}

	HttpRequest(String host, int port, String path, std::map<String, String> headers = std::map<String, String>(), int timeout = 5000) :
		HttpRequest(GET, host, port, path, headers, "", timeout)
	{
	}


	HttpRequest(String host, int port, String path, std::map<String, String> headers, String body, int timeout = 5000) :
		HttpRequest(POST, host, port, path, headers, body, timeout)
	{
	}

	HttpRequest(RequestType type, String url, std::map<String, String> headers = std::map<String, String>(), int timeout = 5000) :
		HttpRequest(type, "", 80, "", headers, "", timeout)
	{
		ParseUrl(url);
	}	

	void AddHeader(String key, String value)
	{
		if (Headers.count(key) == 1)
		{
#ifdef _DEBUG
			Serial.println("[Esp8266Http] Header " + key + " is added twice.");
#endif
			return;
		}
		Headers.insert(std::pair<String, String>(key, value));
	}
};

struct HttpResponse
{
	/**
	 * \brief An HTTP Status code (-1 When host is unreachable, and -2 if host timed out.)
	 * \see https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
	 */
	int StatusCode;

	/**
	 * \brief The body of the response or the error message.
	 */
	String Body;

	/**
	 * \brief All response headers
	 */
	std::map<String, String> Headers;

	HttpResponse(int statusCode, String body, std::map<String, String> headers) :
		StatusCode(statusCode),
		Body(body),
		Headers(headers)
	{
	}

	HttpResponse(int statusCode, String body) : HttpResponse(statusCode, body, {})
	{
	}

	HttpResponse() : HttpResponse(-1, "", {})
	{
	}
};

class Esp8266Http
{
public:
	/**
	 * \brief Performs a synchronous HTTP GET Request
	 * \param request 
	 * \return 
	 */
	static HttpResponse Get(HttpRequest request);

	/**
	 * \brief Performs a synchronous HTTP POST Request
	 * \param request 
	 * \return 
	 */
	static HttpResponse Post(HttpRequest request);

	/**
	 * \brief Performs a synchronous HTTP Request
	 */
	static HttpResponse Request(HttpRequest request);
};


#endif
