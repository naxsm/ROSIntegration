#pragma once

#include <iostream>
//#include <stdio.h>
//#include <string>
//#include <chrono>
#include <thread>

#include <functional> // std::function

// #include "json.hpp"
#include <CoreMinimal.h>
//#include <ThreadingBase.h>
//#include <Sockets.h>
//#include <SocketSubsystem.h>
//#include <Networking.h> // Unreal networking

#include "IWebSocket.h"       // Socket definition

#include "itransport_layer.h"
#include "types.h"
//

#include "rapidjson/document.h"
using json = rapidjson::Document;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


#pragma warning(disable:4265)
class WebSocketConnection : public rosbridge2cpp::ITransportLayer {
public:
	WebSocketConnection() {
	}
	~WebSocketConnection() {
		run_receiver_thread = false;
		if (receiverThreadSetUp) {
			receiverThread.join(); // Wait for the receiver thread to finish
		}
		if (_sock != nullptr) {
			_sock->Close();
			//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(_sock);
		}
	}

	bool Init(std::string ip_addr, int port);
	bool SendMessage(std::string data);
	bool SendMessage(const uint8_t *data, unsigned int length);
	// int ReceiverThreadFunction();
	void RegisterIncomingMessageCallback(std::function<void(json&)> fun);
	void RegisterIncomingMessageCallback(std::function<void(bson_t&)> fun);
	void RegisterErrorCallback(std::function<void(rosbridge2cpp::TransportError)> fun);
	void ReportError(rosbridge2cpp::TransportError err);
	void SetTransportMode(rosbridge2cpp::ITransportLayer::TransportMode);

	bool IsHealthy() const;

private:
	std::string _ip_addr;
	int _port;

	TSharedPtr<IWebSocket> _sock = nullptr;
	// int sock = socket(AF_INET , SOCK_STREAM , 0);
	// struct sockaddr_in connect_to;
	std::thread receiverThread;
	bool run_receiver_thread = true;
	bool receiverThreadSetUp = false;
	bool _callback_function_defined = false;
	bool bson_only_mode_ = false;
	std::function<void(json&)> _incoming_message_callback;
	std::function<void(bson_t&)> incoming_message_callback_bson_;
	std::function<void(rosbridge2cpp::TransportError)> _error_callback;
	uint8_t* _message;
	size_t _message_len;
};
#pragma warning(default:4265)
