#include <iostream>
#include <vector>
#include <format>
#include <nlohmann/json.hpp>
#include "httpclient.h"
#include "fourchan.h"

FourchanResult FourchanREST::GetThreadImages(std::string board, std::string thread)
{
	HttpClient httpclient;

	std::string url = std::format("{}/{}/thread/{}.json", m_threads_endpoint, board, thread);
	Response response = httpclient.MakeRequest("GET", url);

	if (response.status != 0 || response.body == std::string{})
		return FourchanResult{ 0, {} };

	std::vector<std::vector<std::string>> thread_images = {};
	auto rjson = nlohmann::json::parse(response.body);

	for (auto& el : rjson["posts"])
	{
		if (el.contains("ext"))
		{
			std::string fname = std::to_string(el["tim"].get<unsigned long long int>());
			std::string ext = el["ext"];
			std::string image_url = std::format("{}/{}/{}{}", m_cdn_endpoint, board, fname, ext);
			std::string thumbnail_url = std::format("{}/{}/{}s.jpg", m_cdn_endpoint, board, fname);
			std::vector<std::string> image_files = { fname, ext, image_url, thumbnail_url };
			thread_images.push_back(image_files);
		}
	}

	return FourchanResult{ 1, thread_images };
}

std::string FourchanREST::GetImageBytes(std::string url) {
	HttpClient httpclient;
	Response response = httpclient.MakeRequest("GET", url);
	return response.body;
}