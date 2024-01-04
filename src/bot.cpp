#include"bot.hpp"
#include"requests.hpp"
weatherBot::weatherBot(TgBot::Bot& bot,string apiKey,string forecastsFolder)
{
    this->apiKey = apiKey;
    this->bot = &bot;
    this->forecastsFolder = deleteSlashInFolder(forecastsFolder);
}
class ascciException: public::exception //Класс ошибки, если в строке содержатся Unicode символы
{
    string what()
    {
        return "Non Ascii Symbols";
    }
};
void weatherBot::startCommandHandler(TgBot::Message::Ptr message) // Обработчик команды /start
{
    try
    {
        this->bot->getApi().sendMessage(message->chat->id,"Welcome to weather bot");
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
}
string deleteSlashInFolder(string str)
{
    if(str.at(str.length()-1) == '/') return str.substr(0,str.length()-1);
    else return str;
}
bool containUnicode(string str) // Проверка на наличие Unicode символов в строке
{
    for(size_t i = 0;i<str.length();i++)
    {
        if(str[i] > 125 || str[i] < 32) {
            cout << str[i] << endl;
            return true;
        }
    }
    return false;
}
void weatherBot::weatherCommandHandler(TgBot::Message::Ptr message) // Обработчик команды /weather
{
    try
    {
        if(!containUnicode(message->text)) // проверка на то, что в строке только UTF-8 символы
        {
            string place = StringTools::split(message->text,' ')[1]; // получение места, разбиением строки на две части - "/weather" и само место
            botSendMessageWithWeather(message,place); // отправляем запрос пользователю
        }
        else{
            throw ascciException();
        }
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
        this->bot->getApi().sendMessage(message->chat->id,"Error occured parsing message");
    }   
}
void weatherBot::anyMessageHandler(TgBot::Message::Ptr message) // Обработчик любых текстовых сообщений
{
   try
    {
        if(!containUnicode(message->text)) // проверка на то, что в строке только UTF-8 символы
        {
            string place = message->text; // получение места, разбиением строки на две части - "/weather" и само место
            botSendMessageWithWeather(message,place); // отправляем запрос пользователю
        }
        else{
            throw ascciException();
        }
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
        this->bot->getApi().sendMessage(message->chat->id,"Error occured parsing message");
    }   
}
void weatherBot::mapDotHandler(TgBot::Message::Ptr message) // Обработчик точек на карте
{
    string place = to_string(message->location->latitude) + "," + to_string(message->location->longitude); // формирование строки вида "lat,long" для запроса 
    botSendMessageWithWeather(message,place);
}
void weatherBot::botSendMessageWithWeather(TgBot::Message::Ptr message,string place)
{
    json::value jsonValue;  
    long long currentTimeSeconds = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count(); //текущее время в секундах
    if((currentTimeSeconds - lastTimeUpdateCheck(place)) > 900) // Погода обновляется каждые 15 минут, обновляем погоду если время > 15*60
    {
        weatherRequest newReq(this->apiKey);
        jsonValue = newReq.requestWeather(place); // запрос погоды 
        writeForecastForPlace(place,jsonValue); // записываем погоду в файл
    }
    else
    {
        jsonValue = getJsonFromFile(this->forecastsFolder + "/" + place); // считываем уже записанный прогноз
    }
    if(!jsonValue.as_object().if_contains("error")) // если в json нет поля "error"(то есть запрос удачный), то отправляем в боте в телеграмме текущую погоду
    {
            string location = json::value_to<string>((jsonValue.at("location").at("name")));
            double temp_c = json::value_to<double>((jsonValue.at("current").at("temp_c")));
            double wind_kph = json::value_to<double>((jsonValue.at("current").at("wind_kph")));
            int humidity = json::value_to<int>((jsonValue.at("current").at("humidity")));
            string textWeather = json::value_to<string>((jsonValue.at("current").at("condition").at("text")));
            double feelsLike = json::value_to<double>((jsonValue.at("current").at("feelslike_c"))); // получаем нужные для отправки пользователю переменные
            textWeather[0] = tolower(textWeather[0]); // Overcast -> overcast
            string replyString = "Current Weather: \nTemprature: " + to_string(temp_c) + "C \nWind speed: "
            + to_string(wind_kph) + " kph\nHumidity: " + to_string(humidity) 
            + "%\nFeels like " + to_string(feelsLike) + "C\n" + "Weather is " + textWeather ; // создаем строку для ответа пользователю
            /*
                Current Weather: 
                Temprature: 22C 
                Wind speed: 15 kph
                Humidity: 80 %
                Feels like 25C
                Weather is overcast
            */
            this->bot->getApi().sendMessage(message->chat->id,replyString);  // отвечаем пользователю
        }
        else this->bot->getApi().sendMessage(message->chat->id,"Error occured requesting weather"); // если есть поле "error", то возращаем ошибку с получением погоды
}
void weatherBot::writeForecastForPlace(string file,json::value jsonValue)
{
    string pathToForecastFile = this->forecastsFolder + "/" + file;
    string write = json::serialize(jsonValue); // преобразуем json в строку
    ofstream f(pathToForecastFile); // открываем файл с погодой
    f.clear(); // удаляем предыдущий прогноз
    f << write; // записываем новый прогноз
    f.close(); // закрываем поток
}
int weatherBot::lastTimeUpdateCheck(string file)
{
    string path = this->forecastsFolder + "/" + file;
    if(fs::exists(path))
    {
        ifstream f(path);
        string noSepString;
        getline(f,noSepString);
        int lastTime = json::value_to<int>(getJsonFromFile(path).at("current").at("last_updated_epoch"));
        return lastTime;
    }
    else
    {
        return 0;
    }
}
json::value getJsonFromFile(string path)
{
    if(fs::exists(path))
    {
        fstream f(path);
        json::value jsonValue = json::parse(f);
        return jsonValue;
    }
    return 0;
}