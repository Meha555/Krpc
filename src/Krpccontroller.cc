#include "Krpccontroller.h"

KrpcController::KrpcController()
    : m_failed(false)
    , m_canceled(false)
{
}

void KrpcController::Reset()
{
    m_failed = false;
    m_canceled = false;
    m_errText.clear();
}

bool KrpcController::Failed() const
{
    return m_failed;
}

std::string KrpcController::ErrorText() const
{
    return m_errText;
}

void KrpcController::SetFailed(const std::string &reason)
{
    m_failed = true;
    m_errText = reason;
}

void KrpcController::StartCancel()
{
    m_canceled = true;
}

bool KrpcController::IsCanceled() const
{
    return m_canceled;
}

void KrpcController::NotifyOnCancel(google::protobuf::Closure *callback)
{
    assert(m_canceled);
}