#include <iostream>
#include <filesystem>
#include "requests.hpp"
#include "bot.hpp"
namespace json = boost::json;
using namespace TgBot;
using namespace std;
namespace fs = std::filesystem;
void initFolders(string,string);
int main()
{
    string weathersFolder = "weathers",placesFolder = "places";
    initFolders(placesFolder,weathersFolder);
    string tgBotToken,apiKey;
    ifstream apiKeyFile("key.env"); // файл с ключем api для погоды
    ifstream tgKeyFile("tgbot.env");// файл с ключем api для бота в телеграмме
    getline(apiKeyFile,apiKey);
    getline(tgKeyFile,tgBotToken);
    apiKeyFile.close();
    tgKeyFile.close(); // Получение ключей из файлов
    TgBot::Bot bot(tgBotToken); // Создание бота
    weatherBot wBot(bot,apiKey,weathersFolder,placesFolder);
    bot.getEvents().onCommand("start",[&](TgBot::Message::Ptr mess){wBot.startCommandHandler(mess);});
    bot.getEvents().onCommand("weather",[&](TgBot::Message::Ptr mess){wBot.weatherCommandHandler(mess);});
    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr mess){
        if(mess->text != "") // Проверка на наличие текста  
        {
            if(mess->text[0] != '/') wBot.anyMessageHandler(mess); // если не команда, то обрабатываем в обработчике комманд
        }
        else if(mess->location != NULL) wBot.mapDotHandler(mess); // если текста нет и есть локация, то обрабатываем как точку на карте
    }); // Добавление обработчиков событий

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }
}
void initFolders(string places,string forecasts)
{
    fs::path placesFolders(places); // папка для хранения мест пользователя
    fs::path forecastsFolders(forecasts); // папка для хранения прогнозов, обновляется каждый час
    if(!fs::exists(placesFolders)) fs::create_directory(places);
    if(!fs::exists(forecastsFolders)) fs::create_directory(forecasts);
}