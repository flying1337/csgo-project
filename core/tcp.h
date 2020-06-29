#pragma once

class c_tcp {
private:
	WSADATA wsaData_{};
	SOCKET socket_ = INVALID_SOCKET;
	addrinfo* result_ = nullptr, hints_{};
public:
	bool init(const char* ip, const char* port);
	void disconnect();

	bool tell(const char* data, const size_t size);
	int ask(char* buf, const size_t size);
};