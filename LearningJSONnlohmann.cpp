// LearningExceptionHandling
// compile With:
// g++ -Wall -pthread -std=c++14 -o LearningJSONnlohmann LearningJSONnlohmann.cpp

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
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


int main() {
    // 1. Parse a JSON string into DOM.
//     const char* test_json =  "{\"projectA\":"
//                             "\"rapidjson\",\"stars\":10,\"balls\":10"
//                         "}";
//                         // "{\"projectB\":"
//                         //     "\"rapidjson2\",\"star\":10,\"ball\":10"
//                         // "}}";
    
//     // /*
//     const char* literal_json = R"s([
//     {
//         "rawValue": 60.0,
//         "highCode": "70000",
//         "time": "2022-10-21T09:53:57Z"
//     },
//     {
//         "rawValue": 20.0,
//         "highCode": "70010",
//         "time": "2022-04-03T01:01:01Z"
//     },
//     {
//         "rawValue": 30.0,
//         "highCode": "70020",
//         "time": "2022-05-03T01:01:01Z"
//     },
//     {
//         "rawValue": 40.0,
//         "highCode": "70030",
//         "time": "2022-06-03T01:01:01Z"
//     },
//     {
//         "rawValue": 50.0,
//         "highCode": "70040",
//         "time": "2022-08-03T01:01:01Z"
//     }
// ])s";



    std::vector<TO_CODE> highcode_list;
    std::vector<TMInfo> TM_List, output_tm_list;

    // TIMESTAMP start_ts, stop_ts;
    TMInfo tm;
    string endpoint;
    // TO_CODE single_highcode = 70000;
    // TMInfo single_TM;

    srand(time(0)); //gerar valores diferentes a cada execucao

    // Medida 1
    tm.teleobject_id = 70000;
    tm.tm_value = rand() % 10 + 100;
    TM_List.push_back(tm);

    // // Medida 2
    tm.power_station_id = 998877;
    tm.skid_id = 987;
    tm.teleobject_id = 70010;
    tm.tm_value = rand() % 10;
    TM_List.push_back(tm);

    // // Medida 3
    tm.teleobject_id = 70020;
    tm.tm_value = rand() % 10 + 50;
    TM_List.push_back(tm);

    // // Medida 4
    tm.power_station_id = 998877;
    tm.skid_id = 876;
    tm.teleobject_id = 70030;
    tm.tm_value = rand() % 100 * 0.1;
    TM_List.push_back(tm);

    // // Medida 5
    tm.teleobject_id = 70040;
    tm.tm_value = 10;
    TM_List.push_back(tm);

    // Colocando highcodes no vetor de highcodes
    for(std::vector<TMInfo>::iterator it = TM_List.begin(); it != TM_List.end(); ++it){
        // Logger::lprintf(LOCAL_DEBUG, "SSITServer::integratedTestInfluxDB() - Highcode: %ld , Val: %f , TS: %s \n",it->teleobject_id, it->tm_value);      
        highcode_list.push_back(it->teleobject_id);
    }
    // */



    std::vector<TMInfo> tm_info = TM_List;
    string body;
    unsigned int multiple_lines = 0;

    // Exemplo de formato a ser inserido:
    //   {"powerStationId": 111, "skidId": 11, "teleObjectId":123456, "value":33.0, "time":"2022-05-18T14:48:50.12"},

    json test_json_serialization;

    body += "[";
    for(std::vector<TMInfo>::iterator it = tm_info.begin(); it != tm_info.end(); ++it){
        // Logger::lprintf(NORMAL, "DBProxy::insertTelemeasures() - pwstation_id: %ld skid_id: %ld Tobj_id: %ld , Val: %f , Tstamp: %s \n",it->power_station_id, it->skid_id, it->teleobject_id, it->tm_value);
        
        if(multiple_lines)
            body +=",";
        
        test_json_serialization["powerStationId"]  = it->power_station_id;
        test_json_serialization["skidId"]          = it->skid_id;
        test_json_serialization["value"]           = it->tm_value;
        test_json_serialization["teleObjectId"]    = it->teleobject_id;

        // body += "{\"powerStationId\": " + std::to_string(it->power_station_id)   + ",";
        // body += "\"skidId\": "          + std::to_string(it->skid_id)            + ",";
        // body += "\"teleObjectId\": "    + std::to_string(it->teleobject_id)      + ",";
        // body += "\"value\": "           + std::to_string(it->tm_value)           + "\"}";

        body += test_json_serialization.dump();
        multiple_lines++;

        std::cout << "\n>>> nlohmann JSON (exec " << std::to_string(multiple_lines) << ") \n" << test_json_serialization.dump().c_str() << std::endl;
        std::cout << "\n";
    }
    body += "]";

    // std::cout << "\n>>> Test_json:\n" << test_json << std::endl;
    // std::cout << "\n>>> Literal_json:\n" << literal_json << std::endl;
    
    
    // std::cout << "\n>>> nlohmann JSON:\n" << testJson.dump().c_str() << std::endl;
    // std::cout << "\n>>> Literal_json:\n" << literal_json << std::endl;

    std::cout << "\n>>> body:\n" << body << std::endl;
    

    std::cout << "\n\n\n\n Now lets serialize:\n>>> body:\n" << body << std::endl;
    

    json test_json_deserialization;
    test_json_deserialization = json::parse(body);

    output_tm_list = test_json_deserialization["powerStationId"].get();

    std::cout << "\n>>> parsed_body:\n" << test_json_deserialization.dump(4) << std::endl;


    return 0;


}

