#pragma once
#include<google/protobuf/service.h>
#include<memory>
#include<muduo/net/TcpServer.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/TcpConnection.h>
class RpcProvider
{
private:
    /* data */
    std::unique_ptr<muduo::net::TcpServer> m_tcpServerPtr;
    muduo::net::EventLoop m_eventLoop;
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    void OnMessage(const muduo::net::TcpConnectionPtr&,
                            muduo::net::Buffer*,
                            muduo::Timestamp);
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&,google::protobuf::Message*);
    struct ServiceInfo
    {
        google::protobuf::Service *m_services;
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_mthodMap;
    };
    std::unordered_map<std::string,ServiceInfo> m_servcieMap;
public:
    void NotifyService(google::protobuf::Service* );
    RpcProvider(/* args */);
    ~RpcProvider();
    void Run();
};

