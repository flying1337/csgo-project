#include "../cheat.h"

bool c_tcp::init(const char* ip, const char* port) {
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (result)
		return false;

	hints_.ai_family = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(ip, port, &hints_, &result_);
	if (result) {
		WSACleanup();
		return false;
	}

	addrinfo* ptr = nullptr;
	for (ptr = result_; ptr != nullptr; ptr = ptr->ai_next) {
		socket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (socket_ == INVALID_SOCKET) {
			WSACleanup();
			return false;
		}

		result = connect(socket_, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
		if (result == SOCKET_ERROR) {
			closesocket(socket_);
			WSACleanup();
			return false;
		}

		break;
	}

	freeaddrinfo(result_);
	if (socket_ == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
}

void c_tcp::disconnect() {
	shutdown(socket_, SD_SEND);
	closesocket(socket_);
	WSACleanup();
}

bool c_tcp::tell(const char* data, const size_t size) {
	if (send(socket_, data, size, 0) == SOCKET_ERROR) {
		disconnect();
		return false;
	}
}

int c_tcp::ask(char* buf, const size_t bufSize) {
	int recvSize{};
	if ((recvSize = recv(socket_, buf, bufSize, NULL)) == SOCKET_ERROR) {
		disconnect();
		return -1;
	}

	return recvSize;
}