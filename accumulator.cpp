#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <iostream>

namespace fs = std::filesystem;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    
    json j;
    try {
        j = json::parse(argv[1]);
    } catch (...) {
        std::cerr << "Invalid JSON\n";
        return 1;
    }
    
    fs::create_directories("tests");
    std::string path = "tests/spy_live_data.csv";
    bool exists = fs::exists(path);
    
    std::ofstream csv(path, std::ios::app);
    if (!csv) return 1;
    
    if (!exists) {
        csv << "Timestamp,Price,High,Low,Open,PrevClose\n";
    }
    
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    csv << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S") << ","
        << j.value("c", 0.0) << "," << j.value("h", 0.0) << ","
        << j.value("l", 0.0) << "," << j.value("o", 0.0) << ","
        << j.value("pc", 0.0) << "\n";
    
    std::cout << "Data saved to " << path << std::endl;
    return 0;
}
