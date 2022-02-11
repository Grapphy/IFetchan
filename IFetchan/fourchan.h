#pragma once
#include <iostream>
#include <vector>

class FourchanResult
{
public:
	int status = 0;
	std::vector<std::vector<std::string>> images;
};

class FourchanREST
{
private:
	std::string m_threads_endpoint = "https://a.4cdn.org";
	std::string m_cdn_endpoint = "https://i.4cdn.org";

public:
	FourchanResult GetThreadImages(std::string board, std::string thread);
	std::string GetImageBytes(std::string url);
};