#pragma once
#include"messagequeue.h"
#include<string>
#define LOG_INFO(logmsgformat,...)\
    do\
    {\
       Logger::GetInstance().setLevel(INFO);\
       char c[1024];\
       snprintf(c,1024,logmsgformat,##_VA__ARGS__);\
       Logger::GetInstance().Log(c);\
    }while(0);
#define LOG_ERROR(logmsgformat,...)\
    do\
    {\
       Logger::GetInstance().setLevel(ERROR);\
       char c[1024];\
       snprintf(c,1024,logmsgformat,##_VA__ARGS__);\
       Logger::GetInstance().Log(c);\
    }while(0);
enum LogLevel
{
    INFO,
    ERROR,
};
class Logger
{
private:
    /* data */
    int m_loglevel;
    MessageQueue<std::string> m_messageQueue;
public:
    static Logger& GetInstance();
    void setLevel(LogLevel loglevel);
    void Log(std::string msg);
    Logger(/* args */);
    Logger(const Logger&)=delete;
    Logger(Logger&&)=delete;
    ~Logger();
};

