#include "mprpcchannel.h"
#include <rpcheader.pb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <error.h>
#include "mprpcapplication.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method, google::protobuf::RpcController *controller, const google::protobuf::Message *request, google::protobuf::Message *resonse, ::google::protobuf::Closure *)
{
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        std::cout << "seralize request error" << std::endl;
    }
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcheader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        std::cout << "seralize header error" << std::endl;
        return;
    }
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char *)header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;
    std::cout << "=========================" << std::endl;
    std::cout << "header_size" << header_size << std::endl;
    std::cout << "service_name" << service_name << std::endl;
    std::cout << "method_name" << method_name << std::endl;
    std::cout << "args_size" << args_size << std::endl;
    std::cout << "args_str" << args_str << std::endl;
    std::cout << "=========================" << std::endl;
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        std::cout << "error:" << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddr_in server_addr;
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (-1 == connect(clientfd, (sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cout << "connect error! error:" << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        std::cout << "send_error! error:" << errno << std::endl;
        close(clientfd);
        return;
    }
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        std::cout << "recv error!errno:" << errno << std::endl;
        close(clientfd);
        return;
    }
    if (!resonse->ParseFromArray(recv_buf, recv_size))
    {
        std::cout << "parse error! response_srt" << std::endl;
    }
    close(clientfd);
}

MprpcChannel::MprpcChannel()
{
}

MprpcChannel::~MprpcChannel()
{
}
