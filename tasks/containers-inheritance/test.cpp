/*
class Container
{
protected:
    int N = 0;
public:
    int get_size() // функция общая для всех классов
    {
        return N;
    }
    virtual string get_data() // "сериализация" данных и вывод в строку
    {
        // для преобразования данных можно использовать функцию std::to_string()
        return "empty container";
    }
    virtual string get_type()
    {
        return "Container";
    }

};

class MyArray : Container
{
public:
    string get_data() override
    {
        return "";
    }
    string get_type() override
    {
        return "Stack";
    }
};

class Stack : Container
{
public:
    string get_data() override
    {
        return "";
    }
    string get_type() override
    {
        return "Stack";
    }
};
int main()
{
    char s[] = "hello", t[] = "hello";
    if (s == t)
        cout << "True";
    return 0;
}*/