#pragma once
#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
class MprpcChannel:public google::protobuf::RpcChannel
{
private:
    /* data */
public:
    void CallMethod(const google::protobuf::MethodDescriptor*method,
                    google::protobuf::RpcController* controller,const google::protobuf::Message*request,
                    google::protobuf::Message* resonse,::google::protobuf::Closure*);
    MprpcChannel(/* args */);
    ~MprpcChannel();
};

