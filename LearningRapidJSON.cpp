// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
 
using namespace rapidjson;
using namespace std::string_literals;
 
int main() {
    // 1. Parse a JSON string into DOM.
    const char* json =  "{\"project\":"
                            "\"rapidjson\",\"stars\":10,\"balls\":10"
                        "}";
                        // "{\"projectB\":"
                        //     "\"rapidjson2\",\"star\":10,\"ball\":10"
                        // "}}";
    
    /*
    const char* json = R"s([
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
    */

    std::cout << json << std::endl;
    
    Document d;
    d.Parse(json);

    // Document respServ;
    // respServ.Parse(jsonServ);
 
    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    
    Value& s2 = d["balls"];
    s2.SetInt(1);
    
    // Value& strServ = respServ["rawValue"];
    // int rawVal = strServ.GetInt();
    // std::cout << rawVal << std::endl;

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
 
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}

