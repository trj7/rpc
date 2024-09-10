#pragma once
#include"mprpcconfig.h"
class MprpcApplication
{
private:
    /* data */
    static MprpcConfig m_mprpcconfig;
    MprpcApplication(/* args */){};
    MprpcApplication(const MprpcApplication& m)=delete;

public:
    static void Init(int arc,char **argv);
    static MprpcConfig GetConfig();
    static MprpcApplication& GetInstance();
    ~MprpcApplication();
};

