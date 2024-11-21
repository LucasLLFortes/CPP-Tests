// LearningStringManipulation
// compile With:
// g++ -Wall -pthread -std=c++14 -o LearningStringManipulation LearningStringManipulation.cpp

// Mandatory
#include <string>
#include <algorithm>

// Others
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

// #include "Logger.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// using namespace rapidjson;
using namespace std::string_literals;
using string = std::string;


typedef struct{
    int32_t power_station_id  = -1;
    int32_t skid_id           = -1;
    float   tm_value          ;
    int32_t teleobject_id     = -1;
    // TIMESTAMP timestamp         ;
}TMInfo;

typedef long TO_CODE;


std::string trimSpacesFromString(const std::string& untrimmed_string) {
    std::string trimmed_string;
    std::remove_copy_if(untrimmed_string.begin(), untrimmed_string.end(), std::back_inserter(trimmed_string), [](char ch) {
        return std::isspace(ch);
    });
    return trimmed_string;
}


int main() {
    // 1. Parse a JSON string into DOM.
//     const char* test_json =  "{\"projectA\":"
//                             "\"rapidjson\",\"stars\":10,\"balls\":10"
//                         "}";
//                         // "{\"projectB\":"
//                         //     "\"rapidjson2\",\"star\":10,\"ball\":10"
//                         // "}}";
    
    // /*
    const char* untrimmed_text = R"s([
    {
        "rawValue": 60.0,
        "highCode": "70000",
        "time": "2022-10-21T09:53:57Z"
    },
    {
        "rawValue": 20.0,
        "highCode": "70010",
        "time": "2022-04-03T01:01:01Z"
    },
    {
        "rawValue": 30.0,
        "highCode": "70020",
        "time": "2022-05-03T01:01:01Z"
    },
    {
        "rawValue": 40.0,
        "highCode": "70030",
        "time": "2022-06-03T01:01:01Z"
    },
    {
        "rawValue": 50.0,
        "highCode": "70040",
        "time": "2022-08-03T01:01:01Z"
    }
])s";

    string untrimmed_string(untrimmed_text);
    string untrimmed_string2("{\"powerStationId\": 111, \"skidId\": 11, \"teleObjectId\":123456, \"value\":33.0, \"time\":\"2022-05-18T14:48:50.12\"}\"");


    // before
    std::cout << "\n>>> Untrimmed String:\n" << untrimmed_string << std::endl;
    string trimmed_string = trimSpacesFromString(untrimmed_string);

    // after
    std::cout << "\n>>> After: Untrimmed String:\n" << untrimmed_string << std::endl;
    std::cout << "\n>>> Trimmed String:\n" << trimmed_string << std::endl;


    // before 2
    std::cout << "\n>>> Untrimmed String:\n" << untrimmed_string2 << std::endl;
    string trimmed_string2 = trimSpacesFromString(untrimmed_string2);

    // after 2
    std::cout << "\n>>> After: Untrimmed String 2:\n" << untrimmed_string2 << std::endl;
    std::cout << "\n>>> Trimmed String 2:\n" << trimmed_string2 << std::endl;


    return 0;


}

