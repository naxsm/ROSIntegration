#pragma once


#include <CoreMinimal.h>
#include <Sockets.h>
#include <SocketSubsystem.h>
#include "Common/TcpListener.h"
#include "TCPClientListener.h"

#include "itransport_layer.h"
#include "types.h"


#include "rapidjson/document.h"
using json = rapidjson::Document;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


#pragma warning(disable:4265)
class TCPConnection : public rosbridge2cpp::ITransportLayer {
public:
	TCPConnection() 
	{}

	virtual ~TCPConnection() 
	{
		if (_listener)
		{
			if (_listener->IsActive())
				_listener->Stop();
			delete _listener;
			_listener = nullptr;
		}
	}

	bool Init(std::string address, int port);
	bool SendMessage(std::string data);
	bool SendMessage(const uint8_t *data, unsigned int length);
	uint16_t Fletcher16(const uint8_t *data, int count);
	bool ReceiverThreadFunction(FSocket* sock);
	void RegisterIncomingMessageCallback(std::function<void(json&)> fun);
	void RegisterIncomingMessageCallback(std::function<void(bson_t&)> fun);
	void RegisterErrorCallback(std::function<void(rosbridge2cpp::TransportError)> fun);
	void ReportError(rosbridge2cpp::TransportError err);
	void SetTransportMode(rosbridge2cpp::ITransportLayer::TransportMode);

	bool IsHealthy() const;

private:
	//std::string _ip_addr;
	//int _port;

	FSocket *_sock;
	FTCPClientListener* _listener;
	// int sock = socket(AF_INET , SOCK_STREAM , 0);
	// struct sockaddr_in connect_to;
	//std::thread receiverThread;
	bool run_receiver_thread = true;
	bool receiverThreadSetUp = false;
	bool _callback_function_defined = false;
	bool bson_only_mode_ = false;
	std::function<void(json&)> _incoming_message_callback;
	std::function<void(bson_t&)> incoming_message_callback_bson_;
	std::function<void(rosbridge2cpp::TransportError)> _error_callback;
};
#pragma warning(default:4265)
