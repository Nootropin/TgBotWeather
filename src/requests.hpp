#ifndef REQUESTS_H
#define REQUESTS_H
#include<curl/curl.h>
#include<iostream>
#include<fstream>
#include<boost/json.hpp>
class weatherRequest
{
    public:
        std::string key;
        weatherRequest(std::string);
        boost::json::value requestWeather(std::string);
};
#endif