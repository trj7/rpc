#pragma once
#include<google/protobuf/service.h>
class mprpcController:public google::protobuf::RpcController
{
private:
    /* data */
    bool m_failed;
    std::string m_errText;
public:
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string&reson);

    void StartCancel();
    bool IsCanceled()const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
    mprpcController(/* args */);
    ~mprpcController();
};
