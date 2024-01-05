#ifndef BOT_H
#define BOT_H
#include<tgbot/tgbot.h>
#include<string>
#include<boost/json.hpp>
#include<chrono>
#include<filesystem>
namespace json = boost::json;
namespace fs = std::filesystem;
using namespace std;
string deleteSlashInFolder(string input);
json::value getJsonFromFile(string path);
class weatherBot
{
    public:
        void startCommandHandler(TgBot::Message::Ptr message);
        void weatherCommandHandler(TgBot::Message::Ptr messag);
        void anyMessageHandler(TgBot::Message::Ptr message);
        void mapDotHandler(TgBot::Message::Ptr message);
        void forecastCommandHandler(TgBot::Message::Ptr message);
        weatherBot(TgBot::Bot& bot,string apiKey,string weathersFolder,string forecastsFolder); 
    private:
        void botSendMessageFromJson(TgBot::Message::Ptr message,json::value jsonValue);
        void writeJsonInFile(string place, json::value jsonValue);
        int lastTimeUpdateCheck(string file);
        void botSendMessageWithCurrentWeather(TgBot::Message::Ptr message ,string place);
        void sendForecast(TgBot::Message::Ptr message,json::value jsonValue);
        TgBot::Bot* bot;
        string apiKey,weathersFolder,placesFolder,forecastsFolder;
};
#endif