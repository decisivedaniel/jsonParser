#ifndef JSON_EVAL
#define JSON_EVAL

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <unordered_map>
#include <memory>

namespace Json {

enum class node_type : char {
    value,
    array,
    object
};

class Node {
    private:
        std::string key;
        const node_type type;
    public:
        Node(const std::string &k, node_type t) : key(k), type(t) {}
        virtual ~Node() {}
        node_type GetType() const {return type;}
        std::string GetKey() const {return key;}
};

class Value : public Node {
    private:
        std::string value;
    public:
        Value(const std::string &k, const std::string &v) : Node(k, node_type::value), value(v) {}
        ~Value() override {}
        std::string asString() const {return value;}

};

class Array : public Node {
    private:
        std::unordered_map<int, Node> values;
    public:
        Array(const std::string &k, const std::string &v) :
            Node(k, node_type::array) {}
        ~Array() override {}
        const Array &operator[](int index);
};

class Object : public Node {
    private:
        std::unordered_map<std::string, Node> values;
    public:
        Object(const std::string &k, const std::string &v) : 
            Node(k, node_type::object) {}
        ~Object() override {}
        const Object &operator[](const std::string &key);
};

std::shared_ptr<Node> readJson(const std::string &filename);
std::shared_ptr<Node> parseJson(std::ifstream *file);
std::string eval(std::shared_ptr<Node> &n, const std::string &arg);

std::shared_ptr<Object> makeObject(std::ifstream *file);
std::shared_ptr<Array> makeArray(std::ifstream *file);
std::shared_ptr<Value> makeValue(std::ifstream *file, const int arrayKey = -1);
bool isValue(const char test);


}

#endif
