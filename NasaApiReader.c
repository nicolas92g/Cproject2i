#pragma warning(disable : 4996)
#include "NasaApiReader.h"
#include "NasaApodApiKey.h"

void NasaApiReaderCreate(NasaApiReader* self)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	self->curl = curl_easy_init();

	if (!self->curl) {
		printf("Failed to load curl, will not be able to load background images from apod api.\n");
		self->isAbleToRead = 0;
		return;
	}

	self->isAbleToRead = 1;
}

size_t extractImgUrl(void* ptr, size_t size, size_t nmemb, void* stringPtr) {
	char* response = (char*)ptr;

	char* url = strstr(response, "hdurl");
	if (!url) return 0;
	url = strchr(url, ':');
	if (!url) return 0;
	url = strchr(url, '"') + 1;//get the begin of the url
	if (!url) return 0;
	char* endUrl = strchr(url, '"');//get the end of the url
	if (!endUrl) return 0;

	unsigned urlLength = endUrl - url;

	strncpy((char*)stringPtr, url, min(urlLength, 200));

	return size * nmemb; //just to avoid error 23
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	auto buf =  fwrite(ptr, size, nmemb, stream);
	return buf;
}

Texture NasaApiReaderGetTodayImage(NasaApiReader* self)
{

	if (!self->isAbleToRead) {
		return (Texture) { 0, 0, 0, 0 };
	}

	char imgUrl[201] = { 0 };

	//request the apod picture url
	curl_easy_setopt(self->curl, CURLOPT_URL, "https://api.nasa.gov/planetary/apod?api_key="NASA_APOD_API_KEY);
	curl_easy_setopt(self->curl, CURLOPT_WRITEFUNCTION, extractImgUrl);
	curl_easy_setopt(self->curl, CURLOPT_WRITEDATA, imgUrl);

	CURLcode res = curl_easy_perform(self->curl);

	//printf("|%s|\n", imgUrl);

	if (res != CURLE_OK) {
		return (Texture) { 0, 0, 0, 0 };
	}

	//get the picture data
	FILE* f = fopen("apod.jpg", "wb");
	curl_easy_setopt(self->curl, CURLOPT_URL, imgUrl);
	curl_easy_setopt(self->curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(self->curl, CURLOPT_WRITEDATA, f);

	res = curl_easy_perform(self->curl);
	curl_easy_cleanup(self->curl);

	fclose(f);

	if (res != CURLE_OK) {
		return (Texture) { 0, 0, 0, 0 };
	}

	Texture ret;
	stbi_set_flip_vertically_on_load(0);
	TextureCreate(&ret, "apod.jpg");
	stbi_set_flip_vertically_on_load(1);
	return ret;
}
