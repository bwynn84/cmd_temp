#include <iostream>
#include <string>
#include <iomanip>

#include <curl/curl.h>
#include <json/json.h>


std::string contents;

size_t handle_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t numbytes = size * nmemb;
    
    char lastchar = *((char *) ptr + numbytes - 1);
    *((char *) ptr + numbytes - 1) = '\0';
    contents.append((char *) ptr);
    contents.append(1, lastchar);
    *((char *) ptr + numbytes - 1) = lastchar;
    return size * nmemb;
}

int main(int argc, char* argv[])
{
    
    if(argc < 2)
    {
        std::cout << "Usage is:  " << argv[0] << " + zip";
        exit(0);
    }
    
    std::string zip(argv[1]);
    std::string key = " "; //api key goes here. you can get from the url in the readme
    std::string url = ("http://api.openweathermap.org/data/2.5/weather?zip=" + zip + ",us&units=imperial&appid=" + key); //api url location + zip + api key
    
    CURL* curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_data);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    
    Json::Value root;
    std::string errors;

    
    bool parsingSuccessful = reader->parse(contents.c_str(), contents.c_str() + contents.size(), &root, &errors);
    delete reader;
    
    if(!parsingSuccessful)
    {
        std::cout << contents << std::endl;
        std::cout << errors << std::endl;
    }

    std::setprecision(2); //to only show 2 digits of temp.
    std::string js_temp = root["main"]["temp"].asString(); // assigns main/temp to string
    int temp = std::stoi(js_temp); // converts string to int for setprecision

    
    std::cout << "The temperature for " << root["name"].asString() << " is " << temp  << "° F." << std::endl;

    
    return 0;
}

