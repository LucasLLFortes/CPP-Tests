// LearningDataBucketingInIntervals

// compile With:
// g++ -Wall -pthread -std=c++17 -o LearningDataBucketingInIntervals LearningDataBucketingInIntervals.cpp

// Header declarations
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>

struct DataPoint {
    std::string time;
    double value;
};

std::vector<DataPoint> parseData(const std::vector<std::string>& rawData) {
    std::vector<DataPoint> data;
    for (const auto& entry : rawData) {
        std::istringstream iss(entry);
        std::string time;
        double value;
        char comma;
        iss >> time >> value >> comma;
        data.push_back({time, value});
    }
    return data;
}

int timeToMinutes(const std::string& time) {
    int hours, minutes, seconds;
    sscanf(time.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
    return hours * 60 + minutes;
}

void calculateAverages(const std::vector<DataPoint>& data) {

    short bucket_size_in_mins = 5;

    int startInterval = timeToMinutes(data.front().time) / bucket_size_in_mins * bucket_size_in_mins;
    int endInterval = startInterval + bucket_size_in_mins;
    double sum = 0;
    int count = 0;

    // Create buckets filling them with zeros
    std::map<int, double> time_buckets;

    for(int bucket = 0; bucket < 24 * 60; bucket += bucket_size_in_mins)
    {
        time_buckets.insert({bucket,0});
    }

    // Process data to fill in buckets through mapping
    for (const auto& dp : data) {
        int minutes = timeToMinutes(dp.time);
        
        if (minutes < startInterval)
        {
            // skip data if its older
            continue;
        }
        
        if (minutes < endInterval) 
        {
            sum += dp.value;
            count++;
        } 
        else 
        {
            // std::cout << "Average from " << startInterval << " to " << endInterval << " minutes: "
            //             << std::fixed << std::setprecision(3) << sum / count << std::endl;
            
            time_buckets[startInterval]= sum / count;
            startInterval = minutes / bucket_size_in_mins * bucket_size_in_mins;
            endInterval = startInterval + bucket_size_in_mins;
            
            // Reinicia a contagem
            sum = dp.value;
            count = 1;
        }
    }

    if (count > 0) {
        // std::cout << "Average from " << startInterval << " to " << endInterval << " minutes: "
        //           << std::fixed << std::setprecision(3) << sum / count << std::endl;
    }

    time_buckets[startInterval]= sum / count;

    // Print all buckets
    for(const auto& [entry, average_value] : time_buckets)
    {

        if(average_value > 0)
        std::cout   << "Average from " << (entry - (entry % 60))/60 << ":" << entry % 60
                    << " to " << (entry + bucket_size_in_mins - ((entry + bucket_size_in_mins) % 60))/60 << ":" << (entry + bucket_size_in_mins) % 60 
                    << " minutes: " << std::fixed << std::setprecision(3) << average_value << std::endl;
        // if(average_value > 0)
        //     std::cout   << "Average from " << entry
        //                 << " to " << entry + bucket_size_in_mins                     
        //                 << " minutes: " << std::fixed << std::setprecision(3) << average_value << std::endl;

    }
}

int main() {
    std::vector<std::string> rawData = {
        "10:10:59 0,250", 
        "10:11:50 0,722", 
        "10:12:50 2,678", 
        "10:13:50 2,807",
        "10:14:59 4,089", 
        "10:15:50 4,736", 
        "10:16:50 6,539", 
        "10:20:50 6,742",
        "10:21:59 8,197", 
        "10:24:50 9,147", 
        "10:25:50 9,899", 
        "10:26:50 10,162",
        "10:27:59 11,038", 
        "10:28:50 11,991", 
        "10:29:50 12,582", 
        "10:30:50 12,761",
        "10:31:50 14,159", 
        "10:32:50 15,821", 
        "10:33:50 17,099", 
        "10:34:50 17,755",
        "10:35:50 18,263", 
        "10:36:50 18,998", 
        "10:37:50 19,405", 
        "10:38:50 20,555",
        "10:39:50 21,920",
        "10:34:50 1,998", 
        "10:10:59 7,250", 
        "10:11:50 8,722", 
        "10:12:50 9,678", 
        "10:13:50 0,807",
        "10:14:59 5,089", 
        "10:15:50 3,736", 
        "10:16:50 3,539", 
        "10:20:50 3,742"
    };

    // Replace commas with dots for correct parsing
    for (auto& str : rawData) {
        std::replace(str.begin(), str.end(), ',', '.');
    }

    std::vector<DataPoint> data = parseData(rawData);
    calculateAverages(data);

    return 0;
}