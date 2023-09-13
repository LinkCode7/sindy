#include <gtest/gtest.h>

#include <format>
#include <iostream>
#include <string>

#include "./effective/CFunctions.h"
#include "./effective/HighPerformance.h"
#include "./sample/ExpresstionTest.h"
#include "./sql/case_when.h"
#include "./sql/group_by.h"
#include "./sql/petal.h"
#include "ConstDefine.h"

TEST(SindySqlite, entry)
{
    return;

    test_case_when();
    test_group_by();
    test_petal();

    SQLiteCFunction::entry();
    TestSQLitePerformance::entry();

    TestMathExpresstion::entry();
}
