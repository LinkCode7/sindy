#include "TableData.h"

#include <format>

#include "../../sindy/SQLite.h"

#pragma warning(disable : 4018)

#define UNKNOWN_INT 0
#define UNKNOWN_DOUBLE 999999999

namespace sindy
{
VirtualItem::VirtualItem()
{
    m_iType = kUnknownField;
}
VirtualItem::VirtualItem(const std::string& strKey, int iType) : m_strKey(strKey), m_iType(iType)
{
}

std::string VirtualItem::GetProperty()
{
    if (m_iType == kTextField)
        return "TEXT";
    else if (m_iType == kIntegerField)
        return "INTEGER";
    else if (m_iType == kDoubleField)
        return "FLOAT";
    else if (m_iType == kBlobField)
        return "BLOB";
    else
        return std::string();
}

DataItem::DataItem()
{
    m_iType = kTextField;
}

DataItem::DataItem(const std::string& strKey, const std::string& strValue, int iType)
{
    m_strKey   = strKey;
    m_strValue = strValue;
    m_iType    = iType;
}

DataItem::DataItem(const std::string& strKey, int iType)
{
    m_strKey = strKey;
    m_iType  = iType;
}

std::string DataItem::GetValue()
{
    return m_strValue;
}

void DataItem::SetValue(const std::string& strValue)
{
    m_strValue = strValue;
}

BlobItem::BlobItem(const std::string& strKey)
{
    m_iType = kBlobField;

    m_strKey = strKey;

    m_lBufferSize = 0;
    m_pBuffer     = NULL;
}

BlobItem::~BlobItem()
{
}

void* BlobItem::GetBlobValue()
{
    return m_pBuffer;
}

ColumnItem::ColumnItem()
{
    m_iType   = kTextField;
    m_isFixed = false;
    m_iWidth  = 50;
    m_isHide  = false;
}

ColumnItem::ColumnItem(const std::string& strName, const std::string& strCaption, bool isFixed, int iWidth, bool isHide)
    : VirtualItem(strName, kTextField), m_strValue(strCaption), m_isFixed(isFixed), m_iWidth(iWidth), m_isHide(isHide)
{
}

std::string ColumnItem::GetValue()
{
    return m_strValue;
}

void ColumnItem::SetValue(const std::string& strValue)
{
    m_strValue = strValue;
}

TableData::TableData() : m_strDbAliasName("main")
{
}

void TableData::AddColumnItem(const std::string& strName, const std::string& strCaption, bool isFixed, int iWidth, bool isHide)
{
    m_items.push_back(std::make_shared<ColumnItem>(strName, strCaption, isFixed, iWidth, isHide));
}

void TableData::AddItemKey(const TableData& other)
{
    for (const auto& item : other.m_items)
    {
        m_items.push_back(std::make_shared<DataItem>(item->m_strKey, item->m_iType));
    }
}

std::string TableData::GetValueString(const std::string& strKey)
{
    for (const auto& item : m_items)
    {
        if (item->m_strKey == strKey)
            return item->GetValue();
    }
    return std::string();
}
double TableData::GetValueDouble(const std::string& strKey)
{
    for (const auto& item : m_items)
    {
        if (item->m_strKey == strKey)
            return std::atof(item->GetValue().data());
    }
    return UNKNOWN_DOUBLE;
}
int TableData::GetValueInt(const std::string& strKey)
{
    for (const auto& item : m_items)
    {
        if (item->m_strKey == strKey)
            return std::atoi(item->GetValue().data());
    }
    return UNKNOWN_INT;
}

bool TableData::SetValue(const std::string& strKey, const std::string& strValue)
{
    for (auto& item : m_items)
    {
        if (item->m_strKey == strKey)
        {
            item->SetValue(strValue);
            return true;
        }
    }
    return false;
}
bool TableData::SetValue(const std::string& strKey, double dValue)
{
    for (auto& item : m_items)
    {
        if (item->m_strKey == strKey)
        {
            item->SetValue(std::to_string(dValue));
            return true;
        }
    }
    return false;
}
bool TableData::SetValue(const std::string& strKey, int iValue)
{
    for (auto& item : m_items)
    {
        if (item->m_strKey == strKey)
        {
            item->SetValue(std::to_string(iValue));
            return true;
        }
    }
    return false;
}

void TableData::SetValue2(const std::string& strKey, const std::string& strValue)
{
    for (auto& item : m_items)
    {
        if (item->m_strKey == strKey)
        {
            item->SetValue(strValue);
            return;
        }
    }
}

// 请先BeginTransaction、Prepare，最后再commit
void TableData::InsertRecord(SQLite& database)
{
    int index = 1;
    for (const auto& item : m_items)
    {
        if (item->m_iType == kTextField)
            database.bindText(index++, item->GetValue());
        else if (item->m_iType == kDoubleField)
            database.bindDouble(index++, std::atof(item->GetValue().data()));
        else if (item->m_iType == kIntegerField)
            database.bindInt(index++, std::atoi(item->GetValue().data()));
        else if (item->m_iType == kBlobField)
            database.bindLargeField(index++, item->GetBlobValue(), ((BlobItem*)item.get())->m_lBufferSize);
        else
        {
            database.bindText(index++, item->GetValue());
        }
    }

    database.step();
    database.resetSyntax();
}

void TableData::InsertRecord(SQLite& database, const std::shared_ptr<DataRecord>& record)
{
    auto iSize = record->m_data.size();
    if (iSize != m_items.size())
        return;

    int index = 1;
    for (int i = 0; i < iSize; ++i)
    {
        if (m_items[i]->m_iType == kTextField)
            database.bindText(index++, record->m_data[i]);
        else if (m_items[i]->m_iType == kDoubleField)
            database.bindDouble(index++, std::atof(record->m_data[i].data()));
        else if (m_items[i]->m_iType == kIntegerField)
            database.bindInt(index++, std::atoi(record->m_data[i].data()));
        else
        {
            database.bindText(index++, record->m_data[i]);
        }
    }

    database.step();
    database.resetSyntax();
}

std::string TableData::GetInsertSql(const std::string& strTableName)
{
    std::string strFront = std::format("INSERT INTO {}.{}(", m_strDbAliasName, strTableName);

    std::string strTail = " VALUES(";

    auto iSize = m_items.size();
    for (int i = 0; i < iSize; ++i)
    {
        const VirtualItemPtr& item = m_items[i];
        if (i == iSize - 1)
        {
            strFront += std::format("{})", item->m_strKey);
            strTail += "?)";
        }
        else
        {
            strFront += std::format("{},", item->m_strKey);
            strTail += "?,";
        }
    }
    return std::string(strFront + strTail);
}

std::string TableData::GetUpdateSql(const std::string& strTableName, const std::string& strKey)
{
    //"update table set show_id=:1 where com_id=:2"
    std::string strFront = std::format("UPDATE {}.'{}' SET ", m_strDbAliasName, strTableName);

    auto iSize = m_items.size();

    // 判断的值在最后绑定
    std::string strTail = std::format(" WHERE {}={}", strKey, iSize);

    for (int i = 0; i < iSize; ++i)
    {
        const VirtualItemPtr& item = m_items[i];
        if (item->m_strKey == strKey)
            continue;

        if (i == iSize - 1)
        {
            strFront += std::format("{}=:{}", item->m_strKey, i);
        }
        else
        {
            strFront += std::format("{}=:{},", item->m_strKey, i);
        }
    }
    return std::string(strFront + strTail);
}

void TableData::UpdateRecord(SQLite& database, const std::string& strKey)
{
    int         index = 0;
    std::string strBind;

    int         iType = kTextField;
    std::string strValue;

    for (const auto& item : m_items)
    {
        if (item->m_strKey == strKey)
        {
            iType    = item->m_iType;
            strValue = item->GetValue(); // 这里不可能是 blob字段
            continue;
        }

        strBind = std::format(":{}", index++);
        if (item->m_iType == kTextField)
            database.bindText(strBind, item->GetValue());
        else if (item->m_iType == kDoubleField)
            database.bindDouble(strBind, std::atof(item->GetValue().data()));
        else if (item->m_iType == kIntegerField)
            database.bindInt(strBind, std::atoi(item->GetValue().data()));
        else if (item->m_iType == kBlobField)
            database.bindLargeField(strBind, item->GetBlobValue(), ((BlobItem*)item.get())->m_lBufferSize);
        else
        {
            database.bindText(strBind, item->GetValue());
        }
    }

    // 绑定最后一个：判定值
    {
        strBind = std::format(":{}", index);

        if (iType == kTextField)
            database.bindText(strBind, strValue);
        else if (iType == kDoubleField)
            database.bindDouble(strBind, std::atof(strValue.data()));
        else if (iType == kIntegerField)
            database.bindInt(strBind, std::atoi(strValue.data()));
        else
            database.bindText(strBind, strValue);
    }

    database.step();
    database.resetSyntax();
}

void TableData::CreateSqlOfText(const std::string& strTableName, std::string& strCreateSql)
{
    strCreateSql = std::format("CREATE TABLE {}.'{}'(", m_strDbAliasName, strTableName);

    auto iSize = m_items.size();
    for (int i = 0; i < iSize; ++i)
    {
        const VirtualItemPtr& item = m_items[i];
        if (i == iSize - 1)
        {
            strCreateSql += std::format("'{}' TEXT)", item->m_strKey);
        }
        else
        {
            strCreateSql += std::format("'{}' TEXT,", item->m_strKey);
        }
    }
}

std::string TableData::_CreateSql(const std::string& strTableName, bool isPrimary, bool isAuto)
{
    std::string strCreateSql = std::format("CREATE TABLE {}.'{}'(", m_strDbAliasName, strTableName);

    auto iSize = m_items.size();
    for (int i = 0; i < iSize; ++i)
    {
        const VirtualItemPtr& item        = m_items[i];
        std::string           strProperty = item->GetProperty();
        if (i == iSize - 1)
        {
            strCreateSql += std::format("'{}' {})", item->m_strKey, strProperty);
        }
        else
        {
            if (i == 0)
            {
                std::string strPrimary;
                if (isPrimary)
                    strPrimary = " PRIMARY KEY";
                if (isAuto)
                    strPrimary += " autoincrement";

                if (!strPrimary.empty())
                {
                    strCreateSql += std::format("'{}' {} {},", item->m_strKey, strProperty, strPrimary);
                    continue;
                }
            }

            strCreateSql += std::format("'{}' {},", item->m_strKey, strProperty);
        }
    }
    return strCreateSql;
}

void TableData::FillData(SQLite& database, const std::string& strTableName, Dataset2& dataset)
{
    return;
    std::string strSelect = "select ";

    auto iSize = m_items.size();
    for (int i = 0; i < iSize; ++i)
    {
        if (i == iSize - 1)
        {
            strSelect += std::format("{}", m_items[i]->m_strKey);
        }
        else
        {
            strSelect += std::format("{},", m_items[i]->m_strKey);
        }
    }

    database.prepare(strSelect);
    while (database.step() == SQLITE_ROW)
    {
        DataRecordPtr record = std::make_shared<DataRecord>();
        for (const auto& field : m_items)
        {
            std::string strValue;
            database.getValueText(field->m_strKey, strValue);
            record->m_data.push_back(strValue);
        }
        dataset.AddRecord(record);
    }
}

TableDataPtr Dataset::GetItem(const std::string& strRowName)
{
    for (const auto& data : m_records)
    {
        if (data->GetValueString("name") == strRowName)
            return data;
    }
    return TableDataPtr();
}

std::string Dataset::GetValue(const std::string& strRowName, const std::string& strFieldName)
{
    for (const auto& data : m_records)
    {
        if (data->GetValueString("name") != strRowName)
            continue;

        return data->GetValueString(strFieldName);
    }
    return std::string();
}

Dataset2::Dataset2(const TableDataPtr& schema) : m_schema(schema)
{
}

void Dataset2::FillData(SQLite& database, const std::string& strTableName)
{
    m_schema->FillData(database, strTableName, *this);
}

void Dataset2::InsertRecords(SQLite& database)
{
    for (const auto record : m_records)
    {
        m_schema->InsertRecord(database, record);
    }
}

} // namespace sindy
