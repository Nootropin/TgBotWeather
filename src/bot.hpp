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
        void botSendMessageFromJson(TgBot::Message::Ptr message,json::value jsonValue);
        void writeForecastForPlace(string place, json::value jsonValue);
        int lastTimeUpdateCheck(string file);
        void botSendMessageWithWeather(TgBot::Message::Ptr message ,string place);
        TgBot::Bot* bot;
        string apiKey,forecastsFolder;
        weatherBot(TgBot::Bot& bot,string apiKey,string forecastsFolder);
};
#endif