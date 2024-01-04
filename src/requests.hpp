#ifndef REQUESTS_H
#define REQUESTS_H
#include<curl/curl.h>
#include<iostream>
#include<fstream>
#include<boost/json.hpp>
using namespace std;
class weatherRequest
{
    public:
        string key;
        weatherRequest(string apiKey);
        boost::json::value requestWeather(string place);
};
#endif