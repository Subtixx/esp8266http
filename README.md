<p align="center">

</p>

<p align="center">
    <img src="https://img.shields.io/maintenance/yes/2018.svg?style=flat-square">
</p>

<h3 align="center">
  ESP8266Http
</h3>

<p align="center">
    ESP8266Http is an HTTP Request library, despite it's name ESP8266Http can support other architectures, I just don't have the hardware to test it on.
</p>

<p align="center">
</p>

# Table of contents

* [Examples](#examples)
    * [HTTP GET Request](#http-get-request)
    * [HTTP POST Request](#http-post-request)
    * [HTTP PATCH Request](#http-patch-request)
    * [HTTP PUT Request](#http-put-request)
    * [HTTP DELETE Request](#http-delete-request)
    * [Sending Custom Headers](#sending-custom-headers)

# Examples

By default the library sets the following headers wether you specify it or not:

* User-Agent: ESP8266Http/1.0.0
* Accept: */*

If it's a POST request it also specifies:

* Content-Type: application/x-www-form-urlencoded
* Content-Length

For more in-depth examples look in the [examples folder](https://github.com/Subtixx/esp8266http/blob/master/examples/)

## HTTP GET Request

This will make an HTTP GET Request to https://api.ipify.org

```cpp
HttpResponse response = Esp8266Http::Request(
    HttpRequest(
        HttpRequest::GET, // Specifies that this should be a GET request.
        "https://api.ipify.org" // Specifies the URL where it should make the request to.
    )
);
```

## HTTP POST Request

This will make an HTTP POST Request to https://htttpbin.org/post with `someValue` set to `1024`

```cpp
HttpResponse response = Esp8266Http::Request(
    HttpRequest(
        HttpRequest::POST, // Specifies that this should be a GET request.
        "https://httpbin.org/post?someValue=1024" // Specifies the URL where it should make the request to.
    )
);
```

## HTTP PATCH Request

The library doesn't support `PATCH` requests yet. If you need it please open an issue

## HTTP PUT Request

The library doesn't support `PUT` requests yet. If you need it please open an issue

## HTTP DELETE Request

The library doesn't support `DELETE` requests yet. If you need it please open an issue

## Sending custom headers

Sometimes it's necessary to send custom headers like cookies. This example shows how to do that.

```cpp
HttpResponse response = Esp8266Http::Request(
    HttpRequest(
        HttpRequest::POST, // Specifies that this should be a GET request.
        "https://httpbin.org/cookies", // Specifies the URL where it should make the request to.
        {
            std::make_pair("Cookie", "logged_in=yes"),
        }
    )
);
```