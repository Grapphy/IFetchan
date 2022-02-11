#pragma once
#include <iostream>

class Response
{
public:
	int status = 0;
	std::string body;
	std::string errmsg;
};

class HttpClient
{
private:
	std::string m_user_agent = "Mozilla/5.0 (Windows NT 10.0; rv:91.0) Gecko/20100101 Firefox/91.0";
	static size_t m_writebody(void* ptr, size_t size, size_t nb, std::string* s);

public:
	void SetUserAgent(std::string user_agent);
	Response MakeRequest(std::string method, std::string url, std::string data = std::string{}, bool use_json = false);
};