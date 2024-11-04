#ifndef JSON_EVAL
#define JSON_EVAL

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <unordered_map>
#include <memory>

#define MAX "max("
#define MIN "min("
#define SIZE "size("

namespace Json {



enum class node_type : char {
    value,
    array,
    object
};

enum class value_type : char {
    string,
    number,
    real
};

class Node {
    private:
        std::string key;
        const node_type type;
        std::string raw;
    public:
        Node(const std::string &k, node_type t) : key(k), type(t) {}
        virtual ~Node() {}
        node_type GetType() const {return type;}
        std::string GetKey() const {return key;}
        void setRaw(const std::string &derivedRaw) {raw = derivedRaw;}
        std::string GetRaw() const {return raw;}
};

class Value : public Node {
    private:
        std::string value;
        value_type valueType;
    public:
        Value(const std::string &k, const std::string &v, const value_type type) : Node(k, node_type::value), value(v), valueType(type) {}
        ~Value() override {}
        value_type getValueType() const {return valueType; }
        std::string asString() const;

};

class Array : public Node {
    private:
        std::unordered_map<int, std::shared_ptr<Node>> arrayNodes;
        int size = 0;
    public:
        Array(const std::string &k, const std::string &v) :
            Node(k, node_type::array) {}
        void append(const std::shared_ptr<Node> &toAdd); 
        ~Array() override {}
        int GetSize() const {return size;}
        std::shared_ptr<Node> at(int index);
};

class Object : public Node {
    private:
        std::unordered_map<std::string, std::shared_ptr<Node>> properties;
    public:
        Object(const std::string &k, const std::string &v) : 
            Node(k, node_type::object) {}
        ~Object() override {}
        void addProp(const std::string &k, const std::shared_ptr<Node> &node);
        int getSize() const { return properties.size(); }
        std::shared_ptr<Node> at(const std::string &key);
};

std::shared_ptr<Node> readJson(const std::string &filename);
std::shared_ptr<Node> parseJson(std::istream &file, const std::string &key);
std::string eval(std::shared_ptr<Node> &n, const std::string &arg);
std::shared_ptr<Node> findNode(std::shared_ptr<Node> &n, const std::string &arg);
std::string size(std::shared_ptr<Node> &node, const std::string &arg);

std::shared_ptr<Object> makeObject(std::istream &file, const std::string &objectKey);
std::shared_ptr<Array> makeArray(std::istream &file, const std::string &arrayKey);
std::shared_ptr<Value> makeValue(std::istream &file, const std::string &valueKey);
bool isValue(const char test);
void clearWhitespace(std::istream &file);


}

#endif
