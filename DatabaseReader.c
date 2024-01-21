#include "DatabaseReader.h"

#include <stdio.h>

void DatabaseReaderCreate(DatabaseReader* self) {
	
	self->isConnected = 1;

	self->connection = mysql_init(NULL);

	if (!self->connection) {
		printf("failed to init mysql !\n");
		self->isConnected = 0;
		return;
	}

	if (!mysql_real_connect(self->connection, "localhost", "root", "esgi", "Cproject2i", 0, NULL, 0)) {
		printf("failed to connect to the database !\n");
		self->isConnected = 0;
		return;
	}
}

void DatabaseReaderDestroy(DatabaseReader* self)
{
	mysql_close(self->connection);
}

int DatabaseReaderAddGame(DatabaseReader* self, unsigned score)
{
	char buf[100];
	sprintf_s(buf, 100, "INSERT INTO game(score, datetime) VALUES(%u, CURRENT_TIMESTAMP)", score);
	return !mysql_query(self->connection, buf);
}

const GameModel* DatabaseReaderGetGames(DatabaseReader* self, unsigned* count)
{
	if (mysql_query(self->connection, "SELECT * FROM game LIMIT 1000")) {
		printf("failed to read database !\n");
		return NULL;
	}

	MYSQL_RES* result = mysql_store_result(self->connection);

	if (!result) {
		printf("failed to store result from mysql\n");
		return NULL;
	}

	GameModel* buf = malloc(sizeof(GameModel) * 1000);
	if (!buf) return NULL;

	const int num_fields = mysql_num_fields(result);
	if (num_fields < 3) return NULL;

	*count = 0;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		buf[*count].id = atoi(row[0]);
		buf[*count].score = atoi(row[1]);
		strcpy_s(buf[*count].datetime, 50, row[2]);
		(*count)++;
	}

	mysql_free_result(result);

	return buf;
}
