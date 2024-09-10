#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>
void ShowArgsHelp()
{
    std::cout << "format command -i <configfile>" << std::endl;
    exit(EXIT_FAILURE);
}
MprpcConfig  MprpcApplication::m_mprpcconfig;
void MprpcApplication::Init(int arc, char **argv)
{
    if (arc < 2)
    {
        ShowArgsHelp();
    }
    int c = 0;
    std::string config_file;
    while ((c = getopt(arc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            break;
        case ':':
            ShowArgsHelp();
        default:
            break;
        }
    }
    //加载配置文件  rpc_server_ip rpcserver_port zookeeper_ip= zookeeper_port=
    m_mprpcconfig.LoadConfigFile(config_file.c_str());
    std::cout<<"rpcserverip:"<<m_mprpcconfig.Load("rpcserverip")<<std::endl;
    std::cout<<"rpcserverport:"<<m_mprpcconfig.Load("rpcserverport")<<std::endl;
    std::cout<<"zookeeperip:"<<m_mprpcconfig.Load("rpcserverip")<<std::endl;
    std::cout<<"zookeeperport:"<<m_mprpcconfig.Load("zookeeperport")<<std::endl;
}

MprpcConfig MprpcApplication::GetConfig()
{
    return m_mprpcconfig;
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcApplication::~MprpcApplication()
{
}
