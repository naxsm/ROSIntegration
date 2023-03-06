
#pragma once

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FTcpClientListenerCallback, FSocket*)


class FTCPClientListener : public FRunnable
{
public:

	FTCPClientListener(FSocket& socket)
		: Socket(&socket)
		, bStopping(false)
	{
		Thread = FRunnableThread::Create(this, TEXT("FTCPClientListener"), 8 * 1024, TPri_Normal);
	}

	~FTCPClientListener()
	{
		if (Thread != nullptr)
		{
			Thread->Kill(true);
			delete Thread;
		}

		if (Socket != nullptr)
		{
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
			Socket = nullptr;
		}
	}

	bool IsActive() const
	{
		return (Socket != nullptr) && !bStopping;
	}

	virtual bool Init() override
	{
		return Socket != nullptr;
	}

	virtual uint32 Run() override
	{
		while (!bStopping)
			if (callback.IsBound())
				callback.Execute(Socket);
		return 0;
	}

	virtual void Stop() override
	{
		bStopping = true;
	}


private:
	FSocket* Socket;
	bool bStopping;
	FRunnableThread* Thread;

public:
	FTcpClientListenerCallback callback;
};
