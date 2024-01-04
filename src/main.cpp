#include <iostream>
#include "requests.hpp"
#include "bot.hpp"
namespace json = boost::json;
using namespace TgBot;
using namespace std;
int main()
{
    string tgBotToken,apiKey;
    ifstream apiKeyFile("key.env"); // файл с ключем api для погоды
    ifstream tgKeyFile("tgbot.env");// файл с ключем api для бота в телеграмме
    getline(apiKeyFile,apiKey);
    getline(tgKeyFile,tgBotToken);
    apiKeyFile.close();
    tgKeyFile.close(); // Получение ключей из файлов

    TgBot::Bot bot(tgBotToken); // Создание бота
    bot.getEvents().onCommand("start",[&bot](TgBot::Message::Ptr mess){startCommandHandler(mess,bot);});
    bot.getEvents().onCommand("weather",[&bot,&apiKey](TgBot::Message::Ptr mess){weatherCommandHandler(mess,bot,apiKey);});
    bot.getEvents().onAnyMessage([&bot,&apiKey](TgBot::Message::Ptr mess){
        if(mess->text != "") // Проверка на наличие текста  
        {
            if(mess->text[0] != '/') anyMessageHandler(mess,bot,apiKey); // если не команда, то обрабатываем в обработчике комманд
        }
        else if(mess->location != NULL)mapDotHandler(mess,bot,apiKey); // если текста нет и есть локация, то обрабатываем как точку на карте
    }); // Добавление обработчиков событий

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
}