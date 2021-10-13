#pragma once
#define CURL_STATICLIB

#include <stdio.h>
#include <curl/curl.h>



#pragma comment( lib, "wldap32.lib")
#pragma comment( lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment( lib, "libcurl_a_debug.lib")
#else
#pragma comment( lib, "libcurl_a.lib")
#endif


class CurlHttp
{
public:
	CURL* curl;
	CURLcode res;
	struct curl_slist* headers = NULL; // init to NULL is important

	CurlHttp()
	{

	}

	void init(int sensor_flag = 0)
	{
		// sensor_flag
		// 0: RGB
		// 1: DLT
		// 2: AHAT

		curl = curl_easy_init();
		headers = curl_slist_append(headers, "Content-Type: application/json");

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			switch (sensor_flag)
			{
			case 0:
				curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.3:5000/Store");
				break;
			case 1:
				curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.3:5000/DLT");
				break;
			case 2:
				curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.3:5000/AHAT");
				break;
			case 3:
				curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.3:5000/AB");
				break;
			default:
				fprintf(stderr, "unknown sensor flag: %d\n", sensor_flag);
				break;
			}
		}
	}

	~CurlHttp()
	{
		if (curl) {
			curl_easy_cleanup(curl);
		}
	}

	void SendFrame(uint8_t* postData, const size_t fileSize)
	{
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileSize);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
	}
};
