#include"bot.hpp"
#include"requests.hpp"
class ascciException: public::exception //Класс ошибки, если в строке содержатся Unicode символы
{
    string what()
    {
        return "(\"Non Ascii Symbols\")";
    }
};
void startCommandHandler(TgBot::Message::Ptr message, TgBot::Bot& bot) // Обработчик команды /start
{
    try
    {
         bot.getApi().sendMessage(message->chat->id,"Welcome to weather bot");
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    
   
}
bool containUnicode(string str) // Проверка на наличие Unicode символов в строке
{
    for(size_t i = 0;i<str.length();i++)
    {
        if(str[i] > 125 || str[i] < 32) {
            std::cout << str[i] << std::endl;
            return true;
        }
    }
    return false;
}
void weatherCommandHandler(TgBot::Message::Ptr message, TgBot::Bot& bot,string key) // Обработчик команды /weather
{
    try
    {
        if(!containUnicode(message->text)) // проверка на то, что в строке только UTF-8 символы
        {
            string place = StringTools::split(message->text,' ')[1]; // получение места, разбиением строки на две части - "/weather" и само место
            weatherRequest newReq(key);
            json::value jsonValue = newReq.requestWeather(place); // запрос погоды 
            if(!jsonValue.as_object().if_contains("error")) bot.getApi().sendMessage(message->chat->id,to_string(json::value_to<double>((jsonValue.at("current").at("temp_c"))))); // если в json нет поля "error"(то есть запрос удачный), то отправляем в боте в телеграмме текущую температуру
            else bot.getApi().sendMessage(message->chat->id,"Error occured requesting weather"); // если есть поле "error", то возращаем ошибку с получением погоды
        }
        else{
            throw ascciException();
        }
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
        bot.getApi().sendMessage(message->chat->id,"Error occured parsing message");
    }   
}
void anyMessageHandler(TgBot::Message::Ptr message, TgBot::Bot& bot,string key) // Обработчик любых текстовых сообщений
{
    try
    {
        if(!containUnicode(message->text))
        {
            string place = "";
            place = message->text;
            weatherRequest newReq(key);
            json::value jsonValue = newReq.requestWeather(place); // запрос погоды
            if(!jsonValue.as_object().if_contains("error")) bot.getApi().sendMessage(message->chat->id,to_string(json::value_to<double>((jsonValue.at("current").at("temp_c"))))); // если в json нет поля "error"(то есть запрос удачный), то отправляем в боте в телеграмме текущую температуру
            else bot.getApi().sendMessage(message->chat->id,"Error occured requesting weather"); // если есть поле "error", то возращаем ошибку с получением погоды
        }
        else
        {
            throw ascciException();
        }
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
        bot.getApi().sendMessage(message->chat->id,"Error occured parsing message");
    }   
}
void mapDotHandler(TgBot::Message::Ptr message, TgBot::Bot& bot,string key) // Обработчик точек на карте
{
    try
    {
        string place = to_string(message->location->latitude) + "," + to_string(message->location->longitude); // формирование строки вида "lat,long" для запроса 
        weatherRequest newReq(key);
        json::value jsonValue = newReq.requestWeather(place); // запрос погоды
        if(!jsonValue.as_object().if_contains("error")) bot.getApi().sendMessage(message->chat->id,to_string(json::value_to<double>((jsonValue.at("current").at("temp_c"))))); // если в json нет поля "error"(то есть запрос удачный), то отправляем в боте в телеграмме текущую температуру
        else bot.getApi().sendMessage(message->chat->id,"Error occured requesting weather"); // если есть поле "error", то возращаем ошибку с получением погоды
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
        bot.getApi().sendMessage(message->chat->id,"Error occured parsing coordinates");
    }   
}