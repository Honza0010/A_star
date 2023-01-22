#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifndef GRAFEXCEPTION_H
#define GRAFEXCEPTION_H

#include <exception>
#include <string>


class MyException : public std::exception {
    std::string message;
public:
    MyException(std::string msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }

};

#endif // GRAFEXCEPTION_H


#endif // EXCEPTION_H


