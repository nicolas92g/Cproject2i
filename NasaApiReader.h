#pragma once
#include <Texture.h>
#include <curl/curl.h>

typedef struct 
{
	CURL* curl;
	int isAbleToRead;

}NasaApiReader;

void NasaApiReaderCreate(NasaApiReader* self);

Texture NasaApiReaderGetTodayImage(NasaApiReader* self);
