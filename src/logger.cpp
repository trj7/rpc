#include "logger.h"
#include<time.h>
#include<iostream>
Logger& Logger::GetInstance()
{
    // TODO: 在此处插入 return 语句
    static Logger logger;
    return logger;
}

void Logger::setLevel(LogLevel loglevel)
{
    m_loglevel=loglevel;
}

void Logger::Log(std::string msg)
{
    m_messageQueue.push(msg);
}

Logger::Logger()
{
    std::thread writeLogTask([&](){
        for(;;)
        {
            time_t now=time(nullptr);
            tm *nowtm=localtime(&now);
            char file_name[128];
            sprintf(file_name,"%d-%d-%d-log.txt",nowtm->tm_yday+1900,nowtm->tm_mon+1,nowtm->tm_mday);
            FILE *pf=fopen(file_name,"a+");
            if(pf==nullptr)
            {
                std::cout<<"logger file:"<<file_name<<"open error!"<<std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg=m_messageQueue.pop();
            char time_buf[128];
            sprintf(time_buf,"%d:%d:%d=>[%s]",nowtm->tm_hour,nowtm->tm_min,nowtm->tm_sec,m_loglevel==INFO?"Info":"Error");
            msg.insert(0,time_buf);
            msg.append("\n");
            fputs(msg.c_str(),pf);
            fclose(pf);
        }
    });
    writeLogTask.detach();
}

Logger::~Logger()
{

}
