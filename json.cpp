#include "json.h"

std::shared_ptr<Json::Node> Json::readJson(const std::string &filename) {
    std::ifstream jsonFile;
    std::shared_ptr<Json::Node> base;
    jsonFile.open(filename);
    if (jsonFile.is_open()) {
        base = Json::parseJson(jsonFile, "root");
    } else {
        throw std::invalid_argument("File doesn't exist");
    }
    jsonFile.close();
    return base;
}

std::shared_ptr<Json::Node> Json::parseJson(std::istream &file, const std::string &key) {
    char current;
    file >> current;
    switch (current) {
        case '{':
            return Json::makeObject(file, key);
        case '[':
            return Json::makeArray(file, key);
        case ':':
        case ']':
        case '}':
            throw std::invalid_argument("Json File is malformed");
        default:
            file.unget();
            return Json::makeValue(file, key);
    }
}

std::shared_ptr<Json::Object> Json::makeObject(std::istream &file, const std::string &objectKey) {
    std::stringstream raw;
    raw << "{";
    auto object = std::make_shared<Json::Object>(objectKey,"");
    // read object key
    char current;
    file >> current;
    while (current != '}') {
        if (current != '"') {
            throw std::invalid_argument("Object expected an \"");
        }
        std::stringstream key;
        raw << "\"";
        file >> current;
        do {
            key << current;
            file.get(current);
        } while (current != '"');
        raw  << key.str() << "\" ";
        file >> current;
        if (current != ':') {
            throw std::invalid_argument("Object expected an :");
        }
        std::shared_ptr<Json::Node> node = Json::parseJson(file, key.str());
        object->addProp(key.str(), node);
        raw << ": " << node->GetRaw();
        file >> current;
        if (current != ',' && current != '}') {
            std::cout << "incorrect: " << current << std::endl;
            throw std::invalid_argument("expected in end of object or another property");
        }
        raw << current;
        if (current == ',') {
            file >> current;
        }
    }
    object->setRaw(raw.str());
    return object;
}

void Json::Object::addProp(const std::string &k, const std::shared_ptr<Json::Node> &node) {
    properties.emplace(k, node);
}

std::shared_ptr<Json::Array> Json::makeArray(std::istream &file, const std::string &arrayKey) {
    std::stringstream raw;
    raw << "[";
    auto array = std::make_shared<Json::Array>(arrayKey,"");
    
    char current; 
    file >> current;
    while (current != ']') {
        file.unget();
        std::shared_ptr<Json::Node> node = Json::parseJson(file, std::to_string(array->GetSize()));
        array->append(node);
        
        raw << node->GetRaw();
        file >> current;
        if (current != ',' && current != ']') {
            std::cout << "incorrect: " << current << std::endl;
            throw std::invalid_argument("expected in end of array or another property");
        }
        raw << current;
        if (current == ',') {
            file >> current;
        }
    }
    array->setRaw(raw.str());
    return array;
}

void Json::Array::append(const std::shared_ptr<Json::Node> &toAdd) {
    arrayNodes.emplace(size, toAdd);
    size++;
}

std::shared_ptr<Json::Value> Json::makeValue(std::istream &file, const std::string &valueKey) {
    std::stringstream value;
    char current;
    file.get(current);
    while(Json::isValue(current)) {
        value << current;
        file.get(current);
    }
    auto jsonValue = std::make_shared<Json::Value>(valueKey, value.str());
    jsonValue->setRaw(value.str());
    file.unget();
    return jsonValue;
}

bool Json::isValue(const char test) {
    return (test >= '0' && test <= '9') || //Number
    test == '"' || // Quote
    test == '.' || // period
    test == ' ' || // space
    (test >= 'A' && test <= 'Z') || // capital letters
    (test >= 'a' && test <= 'z'); // lowercase letters
}


std::string Json::eval(std::shared_ptr<Json::Node> &n, const std::string &arg) {
    return arg;
}

  //auto value = std::dynamic_pointer_cast<Json::Value>(json);