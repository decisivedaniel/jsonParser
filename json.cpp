#include "json.h"

std::shared_ptr<Json::Node> Json::readJson(const std::string &filename) {
    std::ifstream jsonFile;
    std::shared_ptr<Json::Node> base;
    jsonFile.open(filename);
    if (jsonFile.is_open()) {
        base = Json::parseJson(jsonFile);
    } else {
        throw std::invalid_argument("File doesn't exist");
    }
    jsonFile.close();
    return base;
}

std::shared_ptr<Json::Node> Json::parseJson(std::istream &file) {
    std::cout << "get to parse"<< std::endl;
    char current;
    do {
        file.get(current);
        std::cout<<"current: " << current <<std::endl;
    } while (current == ' ');

    switch (current) {
        case '{':
            return Json::makeObject(file);
        case '[':
            return Json::makeArray(file);
        case ':':
        case ']':
        case '}':
            throw std::invalid_argument("Json File is malformed");
        default:
            file.unget();
            return Json::makeValue(file);
    }
}

std::shared_ptr<Json::Object> Json::makeObject(std::istream &file) {
    return std::make_shared<Json::Object>("key", "value");
}
std::shared_ptr<Json::Array> Json::makeArray(std::istream &file) {
    return std::make_shared<Json::Array>("key", "value");
}
std::shared_ptr<Json::Value> Json::makeValue(std::istream &file, const int arrayKey) {
    std::stringstream key("");
    char current;
    if (arrayKey == -1) {
        // read object key
        file.get(current);
        while (current == ' ') {
            file.get(current);
        }
        if (current == '"') {
            do {
                key << current;
                file.get(current);
            } while (current != '"');
        }
    } else {
        key << arrayKey;
    }
    std::stringstream value("");
    file.get(current);
    while(Json::isValue(current)) {
        value << current;
    };
    return std::make_shared<Json::Value>(key.str(), value.str());
}

bool Json::isValue(const char test) {
    return (test > 15 && test < 26) || //Number
    test == 2 || //Quote
    test == 14 || // period
    test == 32 || // space
    (test > 64 && test < 91) || // capital letters
    (test > 96 && test < 123); // lowercase letters
}


std::string Json::eval(std::shared_ptr<Json::Node> &n, const std::string &arg) {
    return arg;
}

  //auto value = std::dynamic_pointer_cast<Json::Value>(json);