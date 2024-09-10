#include<iostream>
#include"user.pb.h"
#include"mprpcapplication.h"
#include"mprpcchannel.h"

int main(int argc,char **argv)
{
    MprpcApplication::Init(argc,argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginResult req;
    fixbug::LoginResponse response;
    req.set_name("zhangsan");
    req.set_pwd("123");
    stub.Login(nullptr,&req,&response,nullptr);
    if(0==response.result().errcode())
    {
        std::cout<<"rpc login response success"<<response.success()<<std::endl;
    }
    else
    {
        std::cout<<"rpc login response fail"<<response.result().errmsg()<<std::endl;
    }
}
