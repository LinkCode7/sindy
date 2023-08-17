#include "CFunctions.h"

#include <format>

#include "../../../sindy/RunTime.h"
#include "../../../sindy/SQLite.h"
#include "../ConstDefine.h"

#define INSERT_ROW_COUNT 1000
// #define PRINT_C_FUNCTION_CALLBACK

using namespace std;

namespace SQLiteCFunction
{
void entry()
{
    sindy::RunTime time;

    int iRowCount = testInsert();
    time.addTimePoint(std::format("Insert {} row", iRowCount));
#if 1
    char pSql[100];

    std::sprintf(pSql, "select * from %s", COMPONENT_TABLE_NAME);
    testSelect(pSql);
    time.addTimePoint(pSql);

    std::sprintf(pSql, "select com_id from %s", COMPONENT_TABLE_NAME);
    testSelect(pSql);
    time.addTimePoint(pSql);
#endif

    testExpression();

    time.write(TEST_LOG_PATH);

    {
        createDatabase();
        createTable();
        insertRow();
        select();

        printf("\n------------------------------------------------------\n");
        update();

        printf("\n------------------------------------------------------\n");
        deleteRow();
    }
}

int DropTable(sqlite3* db, const std::string& strTableName)
{
    char pSql[100];
    std::sprintf(pSql, "DROP TABLE %s", strTableName.c_str());

    char* zErrMsg = 0;
    return sqlite3_exec(db, pSql, _callback, 0, &zErrMsg);
}

int ClearTable(sqlite3* db, const std::string& strTableName)
{
    char pSql[100];
    std::sprintf(pSql, "DELETE TABLE %s", strTableName.c_str());

    char* zErrMsg = 0;
    return sqlite3_exec(db, pSql, _callback, 0, &zErrMsg);
}

int testInsert()
{
    sqlite3* db = nullptr;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = DropTable(db, COMPONENT_TABLE_NAME);

    /* Create SQL statement */
    const char* sql =
        "CREATE TABLE %s("
        "com_id INT PRIMARY KEY NOT NULL,"
        "com_type		INT     NOT NULL,"
        "floor_id		INT     NOT NULL,"
        "name           TEXT    NOT NULL,"
        "show_id        INT     NOT NULL,"
        "mode			INT,"
        //"time           TEXT,"
        "note           TEXT)";

    char pCreate[1000];
    std::sprintf(pCreate, sql, COMPONENT_TABLE_NAME);

    char* zErrMsg = 0;
    rc            = sqlite3_exec(db, pCreate, _callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    {
        rc = sqlite3_exec(db, "BEGIN", _callback, 0, &zErrMsg);

        for (int i = 1; i <= INSERT_ROW_COUNT; ++i)
        {
            char pSql[1000];
            // std::sprintf(pSql, "INSERT INTO %s (com_id,com_type,floor_id,name,show_id,time) VALUES (%d, %d, 1, 'Paul', 100,
            // datetime('now','localtime'))", COMPONENT_TABLE_NAME, i, i*1000);
            std::sprintf(pSql, "INSERT INTO %s (com_id,com_type,floor_id,name,show_id) VALUES (%d, %d, 1, 'Paul', 100)",
                         COMPONENT_TABLE_NAME, i, i * 1000);

            rc = sqlite3_exec(db, pSql, _callback, 0, &zErrMsg);
            if (rc != SQLITE_OK)
            {
                sqlite3_free(zErrMsg);
                continue;
            }
        }

        rc = sqlite3_exec(db, "COMMIT", _callback, 0, &zErrMsg);
    }

    sqlite3_close(db);

    return INSERT_ROW_COUNT;
}

void testExpression()
{
    // SELECT COUNT(*) AS "RECORDS" FROM COMPANY
    sqlite3* db = nullptr;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // 数值表达式
    // 计数："select count(*) AS 'RECORDS' from component"
    // 最大数："select max(com_id) AS 'RECORDS' from component"
    // 求和："select sum(com_id) AS 'RECORDS' from component"
    // 平均值："select avg(com_id) AS 'RECORDS' from component"
    char pSql[100];
    std::sprintf(pSql, "select max(com_id) AS 'RECORDS' from %s", COMPONENT_TABLE_NAME);

    char* zErrMsg = 0;
    rc            = sqlite3_exec(db, pSql, _callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
}

void testSelect(const char* sql)
{
    sqlite3* db = nullptr;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    char* zErrMsg = 0;
    rc            = sqlite3_exec(db, sql, _callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
}

int _callback(void* data, int argc, char** argv, char** azColName)
{
#ifdef PRINT_C_FUNCTION_CALLBACK
    printf("\n");
    fprintf(stderr, "%s: ", (const char*)data);
    for (int i = 0; i < argc; ++i)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
#endif
    return 0;
}

void createDatabase()
{
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
}

void createTable()
{
    /* Open database */
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char* sql =
        "CREATE TABLE company("
        "id INT PRIMARY KEY     NOT NULL,"
        "name           TEXT    NOT NULL,"
        "age            INT     NOT NULL,"
        "address        CHAR(100),"
        "salary         REAL);";

    /* Execute SQL statement */
    char* zErrMsg = 0;
    rc            = sqlite3_exec(db, sql, _callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
}

void insertRow()
{
    /* Open database */
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char* sql =
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    char* zErrMsg = 0;
    rc            = sqlite3_exec(db, sql, _callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
}

void select()
{
    /* Open database */
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char* sql = "SELECT * from COMPANY";

    char*       zErrMsg = 0;
    const char* data    = "Callback function called";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, _callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(db);
}

void update()
{
    /* Open database */
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    const char* sql =
        "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
        "SELECT * from COMPANY";

    char*       zErrMsg = 0;
    const char* data    = "Callback function called";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, _callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

void deleteRow()
{
    /* Open database */
    sqlite3* db = NULL;
    int      rc = sqlite3_open(TEST_CUR_DB_PATH, &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    char*       zErrMsg = 0;
    const char* data    = "Callback function called";

    /* Create merged SQL statement */
    const char* sql = "DELETE from COMPANY where ID=2; SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, _callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

} // namespace SQLiteCFunction