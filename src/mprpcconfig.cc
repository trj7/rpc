#include "mprpcapplication.h"
#include "mprpcconfig.h"
#include <iostream>
#include <string>
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}

void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        std::cout << config_file << "is invaild" << std::endl;
    }
    // 1.注释 //2.正确的配置项 3.去掉多余空格
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);
        std::string str(buf);
        Trim(str);

        if (str[0] == '#' || str.empty())
        {
            continue;
        }
        int idx = str.find('=');
        std::string key = str.substr(0, idx);
        int endidx=str.find('\n');
        std::string value = str.substr(idx + 1, endidx-idx-1);
        Trim(value);
        m_configMap.insert({key, value});
    }
}

void MprpcConfig::Trim(std::string &str)
{
    int idx = str.find_first_not_of(' ');
    if (idx != -1)
    {
        str = str.substr(idx, str.size() - idx);
    }
    idx = str.find_last_not_of(' ');
    if (idx != -1)
    {
        str = str.substr(0, idx + 1);
    }
}

MprpcConfig::MprpcConfig()
{
}

MprpcConfig::~MprpcConfig()
{
}
