#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include"rpcprovider.h"
class UserService : public fixbug::UserServiceRpc // rpc服务发布端(提供者)
{
private:
    /* data */
public:
    UserService(/* args */);
    ~UserService();
    bool Login(std::string name,std::string pwd);
    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginResult *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done);
};

UserService::UserService(/* args */)
{
}

UserService::~UserService()
{
}
bool UserService::Login(std::string name, std::string pwd)
{
    std::cout<<"do local service:Login"<<std::endl;
    std::cout<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
    return true;
}
void UserService::Login(::google::protobuf::RpcController *controller,
                        const ::fixbug::LoginResult *request,
                        ::fixbug::LoginResponse *response,
                        ::google::protobuf::Closure *done)
{
    //获取相应数据
    std::string name=request->name();
    std::string pwd=request->pwd();
    //本地业务
    bool login_result=Login(name,pwd);
    //写入响应
    fixbug::ResultCode *code=response->mutable_result();
    code->set_errcode(0);
    code->set_errmsg("");

    response->set_success(login_result);
    //执行回调操作

    done->Run();
}

int main(int argc,char **argv)
{
    //初始化
    MprpcApplication::Init(argc,argv);
    //发布到rpc节点
    RpcProvider provider;
    provider.NotifyService(new UserService());
    //启动一个roc服务发布节点
    provider.Run();
}