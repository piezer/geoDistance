#include <iostream>
#include"json.hpp"
#include <curl/curl.h>
#include<cstdint>
#include<memory>
#include<string>
#include<algorithm>
using json = nlohmann::json;

namespace{
    std::size_t callback(
        const char* in,
        std::size_t size,
        std::size_t num,
        std::string* out)
    {const std::size_t totalBytes(size*num);
    out->append(in, totalBytes);
    return totalBytes;
    }
}

std::string city;
std::string city2;
int main(){
    //get city names
    std::cout << "Enter a city, state: ";
    std::getline(std::cin, city);
    std::cout << "Enter the second city, state: ";
    std::getline(std::cin, city2);
    //replace the spaces with plusses to be put in the url
    std::replace(city.begin(),city.end(),' ','+');
    std::replace(city2.begin(),city2.end(),' ','+');
    //construct the url
    const std::string apiKey("&key=AIzaSyAng1sL5Qz65EsVnkZezHewgN4dGRxeKfk");
    const std::string urlEdit("https://maps.googleapis.com/maps/api/geocode/json?address=");
    const std::string url1=urlEdit+city+apiKey;
    const std::string url2=urlEdit+city2+apiKey;
    //std::cout <<url1;
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url1.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    int httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_perform(curl);
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if(httpCode==200){
        json j = json::parse(*httpData);
        std::cout <<std::setw(4)<<j<<"\n\n";
    }
    return 0;
}