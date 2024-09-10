#pragma once
#include<unordered_map>
#include<string>
class MprpcConfig
{
private:
    /* data */
    std::unordered_map<std::string,std::string> m_configMap;
public:
    std::string Load(const std::string &key);
    void LoadConfigFile(const char *config_file);
    void Trim(std::string& str);
    MprpcConfig(/* args */);
    ~MprpcConfig();
};