#pragma once
#include <mysql.h>
#include "GameModel.h"

typedef struct DatabaseReader
{
	int isConnected;
	MYSQL* connection;
}DatabaseReader;

void DatabaseReaderCreate(DatabaseReader* self);
void DatabaseReaderDestroy(DatabaseReader* self);

int DatabaseReaderAddGame(DatabaseReader* self, unsigned score);
const GameModel* DatabaseReaderGetGames(DatabaseReader* self, unsigned* count);
