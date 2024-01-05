#include <iostream>
#include <filesystem>
#include <fstream>
#include <tgbot/tgbot.h>
#include <boost/json.hpp>
#include "requests.hpp"
#include "bot.hpp"
namespace json = boost::json;
using namespace TgBot;
using namespace std;
namespace fs = std::filesystem;
void initFolders(string,string);
int main()
{
    string weathersFolder = "weathers",forecastsFolder="forecasts";
    initFolders(weathersFolder,forecastsFolder);
    string tgBotToken,apiKey;
    ifstream apiKeyFile("key.env"); // файл с ключем api для погоды
    ifstream tgKeyFile("tgbot.env");// файл с ключем api для бота в телеграмме
    getline(apiKeyFile,apiKey);
    getline(tgKeyFile,tgBotToken);
    apiKeyFile.close();
    tgKeyFile.close(); // Получение ключей из файлов
    TgBot::Bot bot(tgBotToken); // Создание бота
    weatherBot wBot(bot,apiKey,weathersFolder,forecastsFolder);
    cout << "Inited bots" << endl;
    bot.getEvents().onCommand("start",[&](TgBot::Message::Ptr mess){wBot.startCommandHandler(mess);});
    bot.getEvents().onCommand("weather",[&](TgBot::Message::Ptr mess){wBot.weatherCommandHandler(mess);});
    bot.getEvents().onCommand("forecast",[&](TgBot::Message::Ptr mess){wBot.forecastCommandHandler(mess);});
    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr mess){
        if(mess->text != "") // Проверка на наличие текста  
        {
            if(mess->text[0] != '/') wBot.anyMessageHandler(mess); // если не команда, то обрабатываем в обработчике комманд
        }
        else if(mess->location != NULL) wBot.mapDotHandler(mess); // если текста нет и есть локация, то обрабатываем как точку на карте
    }); // Добавление обработчиков событий
    try {
        printf("Bot username");
        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        cout << e.what();
    }
}
void initFolders(string weathers,string forecasts)
{
    fs::path weathersFolder(weathers); // папка для хранения погоды, обновляется каждые 15 минут
    fs::path forecastsFolder(forecasts); // папка для хранения прогнозов, обновляется каждые 15 минут
    if(!fs::exists(weathersFolder)) fs::create_directory(weathers);
    if(!fs::exists(forecastsFolder)) fs::create_directory(forecasts);
}