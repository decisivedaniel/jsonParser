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
    raw << "{ ";
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
    raw << "[ ";
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
            raw << " ";
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
    Json::value_type valueType = Json::value_type::number;
    char current;
    file.get(current);
    if (current == '"') {
        valueType = Json::value_type::string;
    }
    while(Json::isValue(current)) {
        value << current;
        file.get(current);
        if (current == '.') {
            switch (valueType) {
                case Json::value_type::number:
                    valueType = Json::value_type::real;
                    break;
                case Json::value_type::real:
                    throw std::invalid_argument("Real number should have only one decimal point");
                case Json::value_type::string:
                    break;
            }
        }
    }
    auto jsonValue = std::make_shared<Json::Value>(valueKey, value.str(), valueType);
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

std::string Json::Value::asString() const {
    std::stringstream returnValue;
    returnValue << GetRaw();
    if (getType() == value_type::string) {
        returnValue.str("");
        std::string::size_type lastquote = GetRaw().find_last_of('"'); 
        returnValue << GetRaw().substr(1,lastquote-1);
    }
    return returnValue.str();
}


std::string Json::eval(std::shared_ptr<Json::Node> &n, const std::string &arg) {
    auto node = Json::findNode(n, arg);
    if (node->GetType() == Json::node_type::value) {
        auto value = std::dynamic_pointer_cast<Json::Value>(node);
        if (value->getType() == Json::value_type::string) {
            return value->asString();
        }
    }
    return node->GetRaw();
}

std::shared_ptr<Json::Node> Json::findNode(std::shared_ptr<Json::Node> &n, const std::string &arg) {
    if (arg == "") {
        return n;
    }
    std::shared_ptr<Json::Node> returnValue;
    std::string currentArg = arg;
    std::string remainder = "";
    std::string::size_type dotPos = arg.find('.');
    if (dotPos != std::string::npos) {
        currentArg = arg.substr(0, dotPos);
        remainder = arg.substr(dotPos+1);
    } 
    switch (n->GetType()) {
        case Json::node_type::object: 
        {
            auto object = std::dynamic_pointer_cast<Json::Object>(n);
            std::string::size_type bracketPos = currentArg.find('[');
            if (bracketPos != std::string::npos) {
                currentArg = arg.substr(0, bracketPos);
                remainder = arg.substr(bracketPos+1);
            }
            auto next = object->at(currentArg);
            returnValue = Json::findNode(next, remainder);
            break;
        }
        case Json::node_type::array: 
        {
            auto array = std::dynamic_pointer_cast<Json::Array>(n);
            if (currentArg[0] == '[') {
                throw std::invalid_argument("Missing start bracket for array accessor");
            }
            std::string::size_type endBracketPos = currentArg.find(']');
            if (endBracketPos != std::string::npos) {
                currentArg = arg.substr(0, endBracketPos);
            } else {
                throw std::invalid_argument("Missing end bracket for array accessor");
            }
            auto next = array->at(std::stoi(currentArg));
            returnValue = Json::findNode(next, remainder);
            break;
        }
        case Json::node_type::value:
        {
            throw std::invalid_argument("Value type does not have additional properties");
            break;
        }
    }
    return returnValue;
}

std::shared_ptr<Json::Node> Json::Object::at(const std::string &key) {
    return properties.at(key);
}

std::shared_ptr<Json::Node> Json::Array::at(const int key) {
    return arrayNodes.at(key);
}

  