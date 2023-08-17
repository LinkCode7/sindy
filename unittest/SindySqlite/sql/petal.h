#pragma once
#include "../../../sindy/SQLite.h"
#include "../TableData.h"

void test_petal();

class AddSumRecordTable : public sindy::TableData
{
public:
    AddSumRecordTable();
    std::string GetCreateSql();

    void fill(sindy::SQLite& db, int id, const std::string& strName, int type, double width);
};
