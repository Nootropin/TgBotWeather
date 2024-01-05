#ifndef REQUESTS_H
#define REQUESTS_H
#include<curl/curl.h>
#include<iostream>
#include<fstream>
#include<boost/json.hpp>
using namespace std;
namespace json = boost::json;
class weatherRequest
{
    public:
        string key;
        weatherRequest(string apiKey);
        json::value requestWeather(string place);
        json::value requestForecast(string place,string dayAmount);
};
#endif