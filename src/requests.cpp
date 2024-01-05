#include"requests.hpp"
long long writer(char* buffer, long long size, long long nmemb, string* html) { // запись для CURLOPT_WRITEFUNCTION
    long long result = 0;

    if (buffer != NULL) {
        html->append(buffer, size * nmemb);
        result = size * nmemb;
    }

    return result;
}
json::value weatherRequest::requestWeather(string location)
{
    CURL* curl = curl_easy_init();
    if(curl)
    {
        CURLcode res;
        string baseUrl = "http://api.weatherapi.com/v1/current.json";
        string key = this->key;
        string requestUrl = baseUrl + "?key=" + key +"&q=" + location; // формирование запроса 
        string data;
        curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str()); // постановка ссылка
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer); // постановка функции обработчика
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // постановка переменной для записи
        res = curl_easy_perform(curl); // запрос 
        if(res != CURLE_OK)
        {
            return curl_easy_strerror(res); // обработка ошибок
        }
        curl_easy_cleanup(curl); // клинап 
        
        return json::parse(data); // формирование json
    }
    else
    {
        return "Problem With CUrl"; // ошибка инициализации Curl
    }
}
json::value weatherRequest::requestForecast(string location,string dayAmount)
{
    CURL* curl = curl_easy_init();
    if(curl)
    {
        CURLcode res;
        string baseUrl = "http://api.weatherapi.com/v1/forecast.json";
        string key = this->key;
        string requestUrl = baseUrl + "?key=" + key +"&q=" + location + "&days=" + dayAmount; // формирование запроса 
        string data;
        curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str()); // постановка ссылка
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer); // постановка функции обработчика
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // постановка переменной для записи
        curl_easy_setopt(curl, CURLOPT_UPLOAD_BUFFERSIZE, 100000);
        res = curl_easy_perform(curl); // запрос 
        cout << data.length() << endl;
        if(res != CURLE_OK)
        {
            return curl_easy_strerror(res); // обработка ошибок
        }
        curl_easy_cleanup(curl); // клинап 
        return json::parse(data); // формирование json
    }
    else
    {
        return "Problem With CUrl"; // ошибка инициализации Curl
    }
}
weatherRequest::weatherRequest(string key)
{
    this->key = key;
}