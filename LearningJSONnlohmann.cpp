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


typedef struct {
    long alarm_id;
    std::string alarm_date;
    long power_station_id;
    long ets_tag_id;
    long skid_id;
    long equipment_id;
    long teleobject_id;
    int alarm_severity_id;
    std::string alarm_description;
} NotificationAlarmInfo;

typedef struct {
    std::string power_station_name = "";
    std::string skid_name = "";
    std::string equipment_name = "";
    std::string tele_object_name = "";
    std::string severity_name = "";
} AlarmDetails;

// Struct for User Information
typedef struct{
    long user_id;
    std::string name;
    std::string email;
    std::string telegram_notification;
    std::string telegram_enabled;
    std::string whatsapp_notification;
    std::string email_notification;
    std::string email_enabled;
} UserInfo;

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

    // output_tm_list = test_json_deserialization["powerStationId"].get();

    std::cout << "\n>>> parsed_body:\n" << test_json_deserialization.dump(4) << std::endl;


    json test_json_structuring;
    json json1, json2;

    // test_json_structuring["layer0"]["layer1"] = {"another", "list"};

    // json1["json1"] = {"some", "info"};
    // json2["json2"] = {"some", "info 2"};
    // test_json_structuring = 
    // {
    //     1 >= 0 ? json1 : json2
    // };

    if(true){
        test_json_structuring["telegram"] = 
        {   
            {"phone", "blah blah"},
            {"message", "blah blah"}
        };
    }
        
    if(-1){
        test_json_structuring["mail"] = 
        {   
            {"to", "blah blah"},
            {"message", "blah blah"}
        };
    }

    std::cout << "\n>>> Quick structure building tests:\n" << test_json_structuring.dump(4) << std::endl;


//     bool AlarmNotifsHandler::preparePayload(UserInfo user_info)
// {
//     LogMethod(LOCAL_DEBUG);

    UserInfo user_info;
    user_info = {
        1, //user_id
        "user_name", //name
        "user_email", //email
        "+553195589558", //telegram_notification
        "TRUE", //telegram_enabled
        "+553195589558", //whatsapp_notification
        "lllfortes@hotmail.com", //email_notification
        "TRUE" //email_enabled
    };

    NotificationAlarmInfo alarm_info;
    alarm_info = {
            1234, // alarm_id
            "12-12-2024 12:34:56", // alarm_date
            88,   // power_station_id
            1010, // ets_tag_id
            5,    // skid_id
            896,  // equipment_id
            17458,// teleobject_id
            6,    // alarm_severity_id
            "Alarme Urgente" // alarm_description
        };
    
    bool result = false;
    json request_json;
    std::string request_body;
    std::string client_message;


    AlarmDetails alarm_details;
    alarm_details = {
        "power_station_name",
        "skid_name",
        "equipment_name",
        "tele_object_name",
        "severity_name"
    };

    
    
    request_body.clear();
    request_json.clear();

    client_message  = "Olá " + user_info.name + ",\n\nVocê tem uma nova notificação de alarme. Seguem as informações do alarme:\n\n";
    client_message += "Usina:\t"            + alarm_details.power_station_name + "\n";
    client_message += "Equipamento: "       + alarm_details.equipment_name;
    if(!alarm_details.skid_name.empty())
        client_message += " - " + alarm_details.skid_name; 
    client_message += "\n\n";
    client_message += "Descrição:\t"        + alarm_info.alarm_description  + "\n";
    client_message += "Severidade:\t"       + alarm_details.severity_name   + "\n";
    client_message += "Data de Início:\t"   + alarm_info.alarm_date         + "\n\n\n\n\n\n";
    client_message += "© 2025 ATI. All rights reserved.\n\nSe você tiver dúvidas, entre em contato conosco: suporteati@ati.com.br";

    bool HTML_formatting_enabled = true;

    // Se estiver habilitado para email, envia email
    if(!user_info.email_enabled.compare("TRUE"))
    {
        request_json["mail"] = 
        {                
            {"to", user_info.email_notification},
            {"message", client_message},
            {"html", HTML_formatting_enabled}
        };
    }

    // Se estiver habilitado para telegram, adiciona parte de mensagem para telegram
    if(!user_info.telegram_enabled.compare("TRUE"))
    {
        request_json["telegram"] =
        {
            {"phoneNumber", user_info.telegram_notification},
            {"message", client_message}
        };
    }

    request_body = request_json.dump(4);

    #ifdef DEBUG_HTTP_REQUEST
        Logger::lprintf(LOCAL_DEBUG, "AlarmNotifsHandler::preparePayload() - request_body: %s", request_body.c_str());
    #endif
    
    
    // }

    // std::cout << "\n>>> Quick structure building tests:\n" << request_body << std::endl;

    std::cout << "\n>>> Quick structure building tests:\n" << request_body << std::endl;
    std::cout << "\n>>> Quick structure building tests:\n" << request_body.substr(0,10) << std::endl;
    std::cout << "\n>>> Quick structure building tests:\n" << request_body << std::endl;

    return 0;


}

