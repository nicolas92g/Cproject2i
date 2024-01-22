#include "DatabaseReader.h"
#include "DatabaseLogin.h"

#include <stdio.h>

void DatabaseReaderCreate(DatabaseReader* self) {
	
	self->isConnected = 1;

	self->connection = mysql_init(NULL);

	if (!self->connection) {
		printf("failed to init mysql !\n");
		self->isConnected = 0;
		return;
	}

	if (!mysql_real_connect(self->connection, "localhost", DATABASE_USER, DATABASE_PASSWORD, "Cproject2i", 0, NULL, 0)) {
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
	if (!self->isConnected) return 0;
	char buf[100];
	sprintf_s(buf, 100, "INSERT INTO game(score, datetime) VALUES(%u, CURRENT_TIMESTAMP)", score);
	return !mysql_query(self->connection, buf);
}

const GameModel* DatabaseReaderGetGames(DatabaseReader* self, unsigned* count)
{
	if (!self->isConnected) return NULL;

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

unsigned DatabaseReaderGetBestScore(DatabaseReader* self)
{
	if (!self->isConnected) return 0;

	if (mysql_query(self->connection, "SELECT MAX(score) FROM game WHERE score = (SELECT MAX(score) FROM game)")) {
		printf("failed to read database !\n");
		return 0;
	}

	MYSQL_RES* result = mysql_store_result(self->connection);

	if (!result) {
		printf("failed to store result from mysql\n");
		return 0;
	}

	const int num_fields = mysql_num_fields(result);
	if (num_fields < 1) return 0;

	unsigned ret = 0;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		ret = atoi(row[0]);
	}

	mysql_free_result(result);

	return ret;
}
double DatabaseReaderGetMeanScore(DatabaseReader* self)
{
	if (!self->isConnected) return 0.;

	if (mysql_query(self->connection, "SELECT SUM(score) / COUNT(score) FROM game")) {
		printf("failed to read database !\n");
		return 0.;
	}

	MYSQL_RES* result = mysql_store_result(self->connection);

	if (!result) {
		printf("failed to store result from mysql\n");
		return 0.;
	}

	const int num_fields = mysql_num_fields(result);
	if (num_fields < 1) return 0.;

	double ret = 0.;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		sscanf_s(row[0], "%lf", &ret);
	}

	mysql_free_result(result);
	return ret;
}
