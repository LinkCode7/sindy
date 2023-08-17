#ifndef CONST_DEFINE_H
#define CONST_DEFINE_H

#include <string>

#define COMPONENT_TABLE_NAME "component"

#define TEST_CUR_DB_PATH "../unittest/data/sqlite.db3"
#define TEST_LOG_PATH "run.log"

#define SINDY_API

namespace sindy
{
namespace common
{
SINDY_API extern const std::string id;
SINDY_API extern const std::string dwg; // 图纸ID
SINDY_API extern const std::string zy;  // 专业
SINDY_API extern const std::string item;
SINDY_API extern const std::string com_id;
SINDY_API extern const std::string com_type;
SINDY_API extern const std::string unit;
SINDY_API extern const std::string handle;
SINDY_API extern const std::string row_type;
SINDY_API extern const std::string is_private;

SINDY_API extern const std::string main;
SINDY_API extern const std::string name;       // 逻辑名称
SINDY_API extern const std::string alias_name; // 别名
SINDY_API extern const std::string code_name;  // 代号，编号
SINDY_API extern const std::string caption;    // 显示名称
SINDY_API extern const std::string type;
SINDY_API extern const std::string width;
SINDY_API extern const std::string align;
SINDY_API extern const std::string level; // index/序号
SINDY_API extern const std::string cj;    // 层级

SINDY_API extern const std::string layer;
SINDY_API extern const std::string color;

SINDY_API extern const std::string schema;
SINDY_API extern const std::string key;
SINDY_API extern const std::string value;
SINDY_API extern const std::string note;
SINDY_API extern const std::string params;
SINDY_API extern const std::string param_id;
SINDY_API extern const std::string property;
SINDY_API extern const std::string state;
SINDY_API extern const std::string section;
SINDY_API extern const std::string select;
SINDY_API extern const std::string count;

SINDY_API extern const std::string show2d;
SINDY_API extern const std::string show3d;

SINDY_API extern const std::string subtotal;    // 小计
SINDY_API extern const std::string total;       // 合计
SINDY_API extern const std::string formula;     // 计算公式
SINDY_API extern const std::string formula_des; // 计算公式-描述
SINDY_API extern const std::string calc_item;   // 计算部位
SINDY_API extern const std::string adjust;      // 调整值

SINDY_API extern const std::string regex;
SINDY_API extern const std::string hide;
SINDY_API extern const std::string list_data;
SINDY_API extern const std::string mode;
SINDY_API extern const std::string input_mode;
SINDY_API extern const std::string is_default;

SINDY_API extern const std::string data;
} // namespace common

namespace group_by
{
SINDY_API extern const std::string table_name;
using common::id;
SINDY_API extern const std::string province; // 省
SINDY_API extern const std::string city;     // 市
SINDY_API extern const std::string county;   // 县
SINDY_API extern const std::string town;     // 镇
SINDY_API extern const std::string village;  // 村

} // namespace group_by

namespace figure
{
SINDY_API extern const std::string table_name;
SINDY_API extern const std::string letter;
SINDY_API extern const std::string field1;
SINDY_API extern const std::string field2;
SINDY_API extern const std::string field3;
SINDY_API extern const std::string field4;
SINDY_API extern const std::string field5;
} // namespace figure

namespace add_sum_record
{
SINDY_API extern const std::string table_name;
using sindy::common::id;
using sindy::common::name;
using sindy::common::type;
using sindy::common::width;
} // namespace add_sum_record

namespace statistics
{
SINDY_API extern const std::string table_name;

SINDY_API extern const std::string country;    // 国家
SINDY_API extern const std::string population; // 人口
} // namespace statistics

} // namespace sindy

#endif // !CONST_DEFINE_H