#include "json.h"

std::shared_ptr<Json::Node> Json::readJson(const std::string &filename) {
    std::ifstream jsonFile;
    jsonFile.open(filename);
    std::string line;
    //std::cout << filename << " is open: " << jsonFile.is_open() << std::endl;
    if (jsonFile.is_open()) {
        while (std::getline(jsonFile, line)) {
            std::cout << " - " << line << std::endl;
        }
    } 
    jsonFile.close();
    return std::make_shared<Json::Value>("test", "b");
}

std::string Json::eval(std::shared_ptr<Node> &n, const std::string &arg) {
    return arg;
}