#include "MainApplication.h"

#include <mysql.h>


int main(){
	MainApplication app;
	MainApplicationCreate(&app);

	MainApplicationRun(&app);

	return EXIT_SUCCESS;

	MYSQL* con = mysql_init(NULL);

	if (!con) {
		printf("failed to init mysql !\n");
		return EXIT_FAILURE;
	}

	if (!mysql_real_connect(con, "localhost", "root", "esgi", "vocabula", 0, NULL, 0)) {
		printf("failed to connect to the database !\n");
		return EXIT_FAILURE;
	}

	if (mysql_query(con, "SELECT * FROM vocabulaire LIMIT 3")){
		printf("failed to read database !\n");
		return EXIT_FAILURE;
	}

	MYSQL_RES* result = mysql_store_result(con);

	if (!result) {
		printf("failed to store result from mysql\n");
		return EXIT_FAILURE;
	}

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD* field;

	while ((row = mysql_fetch_row(result)))
	{
		for (int i = 0; i < num_fields; i++)
		{
			if (i == 0)
			{
				while (field = mysql_fetch_field(result))
				{
					printf("%s\t", field->name);
				}

				printf("\n");
			}

			printf("%s\t", row[i] ? row[i] : "NULL");
		}
	}
	printf("\n");

	mysql_free_result(result);

	mysql_close(con);

	return EXIT_SUCCESS;
}