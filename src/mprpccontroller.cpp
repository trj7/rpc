#include "mprpccontroller.h"

mprpcController::mprpcController()
{
    m_failed=false;
    m_errText="";
}

mprpcController::~mprpcController()
{
}

void mprpcController::Reset()
{
}

bool mprpcController::Failed() const
{
    return m_failed;
}

std::string mprpcController::ErrorText() const
{
    return m_errText;
}

void mprpcController::SetFailed(const std::string &reson)
{
    m_failed=false;
    m_errText=reson;
}

void mprpcController::StartCancel()
{
}

bool mprpcController::IsCanceled() const
{
    return false;
}

void mprpcController::NotifyOnCancel(google::protobuf::Closure *callback)
{
}
