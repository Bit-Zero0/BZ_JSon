#include <string>
#include <list>
#include <map>

using std::string;
namespace BZ_JSON
{
    class Json
    {
    public:
        enum Type
        {
            json_null,
            json_bool,
            json_int,
            json_double,
            json_string,
            json_array,
            json_object
        };

        Json();
        ~Json();

        Json(Type type);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char *value);
        Json(const string &value);
        Json(const Json &other);

        typedef std::list<Json>::iterator iterator;
        iterator begin()
        {
            return (bz_value.bz_array)->begin();
        }

        iterator end()
        {
            return (bz_value.bz_array)->end();
        }

        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isDouble() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        Json::Type type() const;
        int size() const;
        bool empty() const;

        bool asBool() const;
        int asInt() const;
        double asDouble() const;
        string asString() const;

        bool has(int index);
        bool has(const char *key);
        bool has(const string &key);

        void remove(int index);
        void remove(const char *key);
        void remove(const string &key);

        // append value to array at the end.
        void append(const Json &value);

        Json &operator=(const Json &other);
        bool operator==(const Json &other);
        bool operator!=(const Json &other);

        Json &operator[](int index);
        Json &operator[](const char *key);
        Json &operator[](const string &key);

    private:
        void copy(const Json &other);

        union Value
        {
            bool bz_bool;
            int bz_int;
            double bz_double;
            string *bz_string;                 // 字符串类型
            std::list<Json> *bz_array;         // 数组类型
            std::map<string, Json> *bz_object; // 对象类型
        };

        Type bz_type;
        Value bz_value;
    };

}

using namespace BZ_JSON;

Json::Json()
    : bz_type(json_null)
{
}

Json::~Json() {}

Json::Json(Type type)
{
    switch (type)
    {
    case json_null:
        break;
    case json_bool:
        bz_value.bz_bool = false;
        break;
    case json_int:
        bz_value.bz_int = 0;
        break;
    case json_double:
        bz_value.bz_double = 0.0;
        break;
    case json_string:
        bz_value.bz_string = new string("");
        break;
    case json_array:
        bz_value.bz_array = new std::list<Json>();
        break;
    case json_object:
        bz_value.bz_object = new std::map<string, Json>();
        break;
    }
}

Json::Json(bool value)
    : bz_type(json_bool)
{
    bz_value.bz_bool = value;
}

Json::Json(int value)
    : bz_type(json_int)
{
    bz_value.bz_int = value;
}
Json::Json(double value)
    : bz_type(json_double)
{
    bz_value.bz_double = value;
}

Json::Json(const char *value)
    : bz_type(json_string)
{
    bz_value.bz_string = new string(value);
}

Json::Json(const string &value)
    : bz_type(json_string)
{
    bz_value.bz_string = new string(value);
}

void Json::copy(const Json &other)
{
    bz_type = other.bz_type;
    bz_value = other.bz_value;
}

Json::Json(const Json &other)
    : bz_type(json_null)
{
    copy(other);
}

bool Json::isNull() const
{
    return bz_type == json_null;
}
bool Json::isBool() const
{
    return bz_type == json_bool;
}
bool Json::isInt() const
{
    return bz_type == json_int;
}

bool Json::isDouble() const
{
    return bz_type == json_double;
}

bool Json::isString() const
{
    return bz_type == json_string;
}

bool Json::isArray() const
{
    return bz_type == json_array;
}

bool Json::isObject() const
{
    return bz_type == json_object;
}

bool Json::empty() const
{
    switch (bz_type)
    {
    case json_null:
        return true;
        break;
    case json_array:
        return (bz_value.bz_array)->empty();
        break;
    case json_object:
        return (bz_value.bz_array)->empty();
        break;
    default:
        break;
    }
}

bool Json::asBool() const
{
    if (bz_type == json_bool)
    {
        return bz_value.bz_bool;
    }
    throw std::logic_error("function Json::asBool value type error");
}

int Json::asInt() const
{
    if (bz_type == json_int)
    {
        return bz_value.bz_int;
    }
    throw std::logic_error("function Json::asInt value type error");
}

double Json::asDouble() const
{
    if (bz_type == json_double)
    {
        return bz_value.bz_double;
    }
    throw std::logic_error("function Json::asDouble value type error");
}
string Json::asString() const
{
    if (bz_type == json_string)
    {
        return *(bz_value.bz_string);
    }
    throw std::logic_error("function Json::asString value type error");
}

bool Json::has(int index)
{
    if (bz_type != json_array)
    {
        return false;
    }
    int size = (bz_value.bz_array)->size();
    return (index >= 0) && (index < size);
}

bool Json::has(const char *key)
{
    string name(key);
    has(name);
}

bool Json::has(const string &key)
{
    if (bz_type != json_object)
    {
        return false;
    }
    return (bz_value.bz_object)->find(key) != (bz_value.bz_object)->end();
}

void Json::remove(int index)
{
    if (bz_type != json_array)
    {
        return;
    }

    int size = (bz_value.bz_array)->size();
    if (index < 0 || index >= size)
    {
        return;
    }
    auto it = (bz_value.bz_array)->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }

    // it->clear();                    // 清除内容
    (bz_value.bz_array)->erase(it); // 删除节点
}

void Json::remove(const char *key)
{
    string name(key);
    remove(name);
}
void Json::remove(const string &key)
{
    if (bz_type != json_object)
    {
        return;
    }

    auto it = (bz_value.bz_object)->find(key);
    if (it != (bz_value.bz_object)->end())
    {
        // it->second.clear();
        (bz_value.bz_object)->erase(key);
    }
}

// append value to array at the end.
void Json::append(const Json &value)
{
    if (bz_type != json_array)
    {
        // clear();
        bz_type = json_array;
        (bz_value.bz_array) = new std::list<Json>();
    }
    (bz_value.bz_array)->push_back(value);
}

Json &Json::operator=(const Json &other)
{
}
bool Json::operator==(const Json &other)
{
}

bool Json::operator!=(const Json &other)
{
}

Json &Json::operator[](int index)
{
}
Json &Json::operator[](const char *key)
{
}
Json &Json::operator[](const string &key)
{
}