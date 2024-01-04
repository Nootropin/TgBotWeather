#include"requests.hpp"
size_t writer(char* buffer, size_t size, size_t nmemb, std::string* html) { // запись для CURLOPT_WRITEFUNCTION
    int result = 0;

    if (buffer != NULL) {
        html->append(buffer, size * nmemb);
        result = size * nmemb;
    }

    return result;
}
boost::json::value weatherRequest::requestWeather(std::string location)
{
    CURL* curl = curl_easy_init();
    if(curl)
    {
        CURLcode res;
        std::string baseUrl = "http://api.weatherapi.com/v1/current.json";
        std::string key = this->key;
        std::string requestUrl = baseUrl + "?key=" + key +"&q=" + location; // формирование запроса 
        std::string data;
        curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str()); // постановка ссылка
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer); // постановка функции обработчика
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // постановка переменной для записи
        res = curl_easy_perform(curl); // запрос 
        if(res != CURLE_OK)
        {
            return curl_easy_strerror(res); // обработка ошибок
        }
        curl_easy_cleanup(curl); // клинап 
        return boost::json::parse(data); // формирование json
    }
    else
    {
        return "Problem With CUrl"; // ошибка инициализации Curl
    }
}
weatherRequest::weatherRequest(std::string key)
{
    this->key = key;
}