#pragma once
#include "../TableData.h"

void test_case_when();

class StatisticsTable : public sindy::TableData
{
public:
    StatisticsTable();
    std::string GetCreateSql();

    void fill(sindy::SQLite& sqlite, const std::string& strCountry, int population);
};