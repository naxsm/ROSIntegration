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
				delete[] _message;
				_message_len = 0;
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
		delete[] _message;
		_message_len = 0;
		});

	_sock->Connect();
	
	// Setting up the receiver thread
	UE_LOG(LogROS, Display, TEXT("Setting up receiver thread ..."));


	return true;
}

bool WebSocketConnection::SendMessage(std::string data)
{
	// std::string msg = "{\"args\":{\"a\":1,\"b\":2},\"id\":\"call_service:/add_two_ints:23\",\"op\":\"call_service\",\"service\":\"/add_two_ints\"}";
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
/*
int WebSocketConnection::ReceiverThreadFunction()
{
	TArray<uint8> binary_buffer;
	uint32_t buffer_size = 10 * 1024 * 1024;
	binary_buffer.Reserve(buffer_size);
	bool bson_state_read_length = true; // indicate that the receiver shall only get 4 bytes to start with
	int32_t bson_msg_length = 0;
	int32_t bson_msg_length_read = 0;
	int return_value = 0;

	while (run_receiver_thread) {

		ESocketConnectionState ConnectionState = _sock->GetConnectionState();
		if (ConnectionState != ESocketConnectionState::SCS_Connected) {
			if (ConnectionState == SCS_NotConnected) {
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			} else {
				UE_LOG(LogROS, Error, TEXT("Error on connection"));
				ReportError(rosbridge2cpp::TransportError::R2C_SOCKET_ERROR);
				run_receiver_thread = false;
				return_value = 2; // error while receiving from socket
				continue;
			}
		}

		const FTimespan OneSecond(10 * 1000 * 1000);
		if (!_sock->Wait(ESocketWaitConditions::WaitForRead, OneSecond))
		{
			continue; // check if any errors occured
		}

		if (bson_only_mode_) {
			if (bson_state_read_length) {
				bson_msg_length_read = 0;
				binary_buffer.SetNumUninitialized(4, false);
				int32 bytes_read = 0;
				if (_sock->Recv(binary_buffer.GetData(), 4, bytes_read) && bytes_read > 0) {
					bson_msg_length_read += bytes_read;
					if (bytes_read == 4) {
#if PLATFORM_LITTLE_ENDIAN
						bson_msg_length = (
							binary_buffer.GetData()[3] << 24 |
							binary_buffer.GetData()[2] << 16 |
							binary_buffer.GetData()[1] << 8 |
							binary_buffer.GetData()[0]
						);
#else
						bson_msg_length = *((uint32_t*)&binary_buffer[0]);
#endif
						// Indicate the message retrieval mode
						bson_state_read_length = false;
						binary_buffer.SetNumUninitialized(bson_msg_length, false);
					} else {
						UE_LOG(LogROS, Error, TEXT("bytes_read is not 4 in bson_state_read_length==true. It's: %d"), bytes_read);
					}
				} else {
					UE_LOG(LogROS, Error, TEXT("Failed to recv(); Closing receiver thread."));
					run_receiver_thread = false;
				}
			} else {
				// Message retrieval mode
				int32 bytes_read = 0;
				if (_sock->Recv(binary_buffer.GetData() + bson_msg_length_read, bson_msg_length - bson_msg_length_read, bytes_read) && bytes_read > 0) {

					bson_msg_length_read += bytes_read;
					if (bson_msg_length_read == bson_msg_length) {
						// Full received message!
						bson_state_read_length = true;
						bson_t b;
						if (!bson_init_static(&b, binary_buffer.GetData(), bson_msg_length_read)) {
							UE_LOG(LogROS, Error, TEXT("Error on BSON parse - Ignoring message"));
							continue;
						}
						if (incoming_message_callback_bson_) {
							incoming_message_callback_bson_(b);
						}
					} else {
						UE_LOG(LogROS, VeryVerbose, TEXT("Binary buffer num is: %d"), binary_buffer.Num());
					}
				} else {
					UE_LOG(LogROS, Error, TEXT("Failed to recv()"));
				}
			}
		}
		else {
			FString result;
			uint32 count = 0;
			while (_sock->HasPendingData(count) && count > 0) {
				FArrayReader data;
				data.SetNumUninitialized(count);

				int32 bytes_read = 0;
				// read pending data into the Data array reader
				if (_sock->Recv(data.GetData(), data.Num(), bytes_read))
				{
					int32 dest_len = TStringConvert<ANSICHAR, TCHAR>::ConvertedLength((char*)(data.GetData()), data.Num());
					UE_LOG(LogROS, VeryVerbose, TEXT("count is %d"), count);
					UE_LOG(LogROS, VeryVerbose, TEXT("bytes_read is %d"), bytes_read);
					UE_LOG(LogROS, VeryVerbose, TEXT("dest_len will be %i"), dest_len);
					TCHAR* dest = new TCHAR[dest_len + 1];
					TStringConvert<ANSICHAR, TCHAR>::Convert(dest, dest_len, (char*)(data.GetData()), data.Num());
					dest[dest_len] = '\0';

					result += dest;

					delete[] dest;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			if (result.Len() == 0) {
				continue;
			}

			// TODO catch parse error properly
			// auto j = json::parse(received_data);
			json j;
			j.Parse(TCHAR_TO_UTF8(*result));

			if (_incoming_message_callback)
				_incoming_message_callback(j);
		}
	}

	return return_value;
}
*/

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
