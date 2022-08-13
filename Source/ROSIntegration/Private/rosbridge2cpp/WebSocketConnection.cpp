#include "WebSocketConnection.h"

#include "ROSIntegrationCore.h"

#include <iomanip>
#include <Serialization/ArrayReader.h>

#include "WebSocketsModule.h"
#include "Containers/UnrealString.h"
#include "Containers/StringConv.h"

bool WebSocketConnection::Init(std::string ip_addr, int port)
{
	UE_LOG(LogROS, Warning, TEXT("Using WebSocketConnection"));
	_sock = FWebSocketsModule::Get().CreateWebSocket(TEXT("ws://127.0.0.1:9090/rosbridge"));
	CleanupTempMessage();

	_sock->OnRawMessage().AddLambda([&](const void* data, size_t size, size_t bytes_remaining) -> void {
		if (0 == _message_len) // alloc. for new messsage
			_message = new uint8_t[size + bytes_remaining];

		memcpy(_message + _message_len, data, size);
		_message_len += size;
		if (0 < bytes_remaining)
			return; // partial, wait for more

		if (bson_only_mode_)
		{
			bson_t b;
			bool init = bson_init_static(&b, _message, _message_len);

			if (!init)
			{
				UE_LOG(LogROS, Error, TEXT("Error on BSON parse - Ignoring message"));
				CleanupTempMessage();
				return;
			}
			if (incoming_message_callback_bson_)
			{
				incoming_message_callback_bson_(b);
			}
		}
		else
		{
			json j;
			j.Parse(std::string(reinterpret_cast<const char*>(data)));

			if (_incoming_message_callback)
				_incoming_message_callback(j);
		}
		CleanupTempMessage();
		});

	_sock->Connect();
	
	// Setting up the receiver thread
	UE_LOG(LogROS, Display, TEXT("Setting up receiver thread ..."));


	return true;
}

bool WebSocketConnection::SendMessage(std::string data)
{
	const uint8 *byte_msg = reinterpret_cast<const uint8*>(data.c_str());
	int32 bytes_sent = 0;
	// TODO check proper casting

	// TODO check errors on send
	_sock->Send(byte_msg, data.length(), true);
	UE_LOG(LogROS, VeryVerbose, TEXT("Send data: %s"), *FString(data.c_str()));
	UE_LOG(LogROS, Warning, TEXT("Send data: %s"), *FString(data.c_str()));

	return true;
}

bool WebSocketConnection::SendMessage(const uint8_t *data, unsigned int length)
{
	_sock->Send(data, length, true);
	return true;
}

void WebSocketConnection::RegisterIncomingMessageCallback(std::function<void(json&)> fun)
{
	_incoming_message_callback = fun;
	_callback_function_defined = true;
}

void WebSocketConnection::RegisterIncomingMessageCallback(std::function<void(bson_t&)> fun)
{
	incoming_message_callback_bson_ = fun;
	_callback_function_defined = true;
}

void WebSocketConnection::RegisterErrorCallback(std::function<void(rosbridge2cpp::TransportError)> fun)
{
	_error_callback = fun;
}

void WebSocketConnection::ReportError(rosbridge2cpp::TransportError err)
{
	if (_error_callback)
		_error_callback(err);
}


void WebSocketConnection::SetTransportMode(rosbridge2cpp::ITransportLayer::TransportMode mode)
{
	switch (mode) {
	case rosbridge2cpp::ITransportLayer::JSON:
		bson_only_mode_ = false;
		break;
	case rosbridge2cpp::ITransportLayer::BSON:
		bson_only_mode_ = true;
		break;
	default:
		UE_LOG(LogROS, Error, TEXT("Given TransportMode not implemented!"));
	}
}

bool WebSocketConnection::IsHealthy() const
{
	return run_receiver_thread;
}

void WebSocketConnection::CleanupTempMessage()
{
	if (0 < _message_len)
	{
		delete[] _message;
		_message_len = 0;
	}
}
