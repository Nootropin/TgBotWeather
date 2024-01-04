#ifndef BOT_H
#define BOT_H
#include<tgbot/tgbot.h>
#include<string>
#include<boost/json.hpp>
namespace json = boost::json;
using namespace std;
void startCommandHandler(TgBot::Message::Ptr, TgBot::Bot&);
void weatherCommandHandler(TgBot::Message::Ptr, TgBot::Bot&,string);
void anyMessageHandler(TgBot::Message::Ptr, TgBot::Bot&,string);
void mapDotHandler(TgBot::Message::Ptr, TgBot::Bot&,string);
void botSendMessageFromJson(TgBot::Message::Ptr, TgBot::Bot&,json::value);
#endif