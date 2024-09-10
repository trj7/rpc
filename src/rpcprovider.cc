#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <string>
#include <google/protobuf/descriptor.h>
#include "rpcheader.pb.h"
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
{
    std::string response_str;
    if (response->SerializeToString(&response_str))
    {
        conn->send(response_str);
    }
    {
        std::cout << "Serialize response_str error!" << std::endl;
    }
    conn->shutdown();
}
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp)
{
    std::string recv_buf = buffer->retrieveAllAsString();

    uint32_t header_size = 0;
    recv_buf.copy((char *)header_size, 4, 0);
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcheader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcheader.ParseFromString(rpc_header_str))
    {
        service_name = rpcheader.service_name();
        method_name = rpcheader.method_name();
        args_size = rpcheader.args_size();
    }
    else
    {
        std::cout << "rpc_head_str:" << rpc_header_str << "prase error!" << std::endl;
        return;
    }
    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    std::cout << "=========================" << std::endl;
    std::cout << "header_size" << header_size << std::endl;
    std::cout << "service_name" << service_name << std::endl;
    std::cout << "method_name" << method_name << std::endl;
    std::cout << "args_size" << args_size << std::endl;
    std::cout << "args_str" << args_str << std::endl;
    std::cout << "=========================" << std::endl;
    auto it = m_servcieMap.find(service_name);
    if (it == m_servcieMap.end())
    {
        std::cout << service_name << ":service not exists" << std::endl;
        return;
    }
    google::protobuf::Service *service = it->second.m_services;
    auto mit = it->second.m_mthodMap.find(method_name);
    if (mit == it->second.m_mthodMap.end())
    {
        std::cout << method_name << ":method not exists" << std::endl;
        return;
    }
    const google::protobuf::MethodDescriptor *method = mit->second;
    // 生成request response
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str))
    {
        std::cout << "request parse error,content" << args_str << std::endl;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    service->CallMethod(method, nullptr, request, response, google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr &, google::protobuf::Message *>(this, &RpcProvider::SendRpcResponse, conn, response));
}

// 发布rpc服务接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    // 获取对象的描述信息
    const google::protobuf::ServiceDescriptor *pseviceDs = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pseviceDs->name();
    int methodecnt = pseviceDs->method_count();
    for (int i = 0; i < methodecnt; i++)
    {
        const google::protobuf::MethodDescriptor *pmethodDesc = pseviceDs->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_mthodMap.insert({method_name, pmethodDesc});
    }
    service_info.m_services = service;
    m_servcieMap.insert({service_name, service_info});
}

RpcProvider::RpcProvider()
{
}

RpcProvider::~RpcProvider()
{
}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);
    // 创建TcpServer；
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server.setThreadNum(4);
    std::cout << "RpcProvider start serviver at ip:" << ip << " port:" << port << std::endl;
    server.start();

    m_eventLoop.loop();
}
