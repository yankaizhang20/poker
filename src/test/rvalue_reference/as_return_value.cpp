//
// Created by zyk on 24-3-10.
//
#include <string>


class X
{
public:
    X(const std::string &name) : name_(name)
    {
    }

    X(const X &) {};
    X(X &&) = delete;

private:
    std::string name_;
};


X func()
{
    X test("test");
//    return test;
}

int main()
{
    func();
    return 0;
}