#include <iostream>
#include <curl/curl.h>
#include "httpclient.h"

std::string m_user_agent = "Mozilla/5.0 (Windows NT 10.0; rv:91.0) Gecko/20100101 Firefox/91.0";

size_t HttpClient::m_writebody(void* ptr, size_t size, size_t nb, std::string* s)
{
	s->append(static_cast<char*>(ptr), size * nb);
	return size * nb;
}

void HttpClient::SetUserAgent(std::string user_agent)
{
	m_user_agent = user_agent;
}

Response HttpClient::MakeRequest(std::string method, std::string url, std::string data, bool use_json)
{
	Response response;

	CURLcode rescode;
	CURL* httpc = curl_easy_init();
	curl_easy_setopt(httpc, CURLOPT_URL, url.c_str());
	curl_easy_setopt(httpc, CURLOPT_USERAGENT, m_user_agent.c_str());
	curl_easy_setopt(httpc, CURLOPT_CUSTOMREQUEST, method.c_str());
	curl_easy_setopt(httpc, CURLOPT_WRITEFUNCTION, m_writebody);
	curl_easy_setopt(httpc, CURLOPT_WRITEDATA, &response.body);

	if (data != std::string{}) {
		if (method == "POST")
			curl_easy_setopt(httpc, CURLOPT_POST, 1L);

		curl_easy_setopt(httpc, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(httpc, CURLOPT_POSTFIELDS, data.c_str());

		if (use_json) {
			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			curl_easy_setopt(httpc, CURLOPT_HTTPHEADER, headers);
		}
	}

	rescode = curl_easy_perform(httpc);
	curl_easy_cleanup(httpc);

	response.status = rescode;
	response.errmsg = curl_easy_strerror(rescode);

	return response;
}