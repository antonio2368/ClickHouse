#pragma once

#include <Parsers/IAST.h>

#include <map>


namespace DB
{

class ASTColumns;
class ASTCreateQuery;
class ASTIdentifier;
class ASTStorage;

/// Storage and column overrides for a single table, for example:
///
///   TABLE OVERRIDE `foo` PARTITION BY toYYYYMM(`createtime`)
///
class ASTTableOverride : public IAST
{
public:
    String table_name;
    ASTColumns * columns = nullptr;
    ASTStorage * storage = nullptr;
    String getID(char) const override { return "TableOverride " + table_name; }
    ASTPtr clone() const override;
    void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;
    void applyToCreateTableQuery(ASTCreateQuery * create_query) const;
};

/// List of table overrides, for example:
///
///   TABLE OVERRIDE `foo` (PARTITION BY toYYYYMM(`createtime`)),
///   TABLE OVERRIDE `bar` (SAMPLE BY `id`)
///
class ASTTableOverrideList : public IAST
{
public:
    String getID(char) const override { return "TableOverrideList"; }
    ASTPtr clone() const override;
    void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;
    void setTableOverride(const String & name, const ASTPtr ast);
    void removeTableOverride(const String & name);
    ASTPtr tryGetTableOverride(const String & name) const;
    bool hasOverride(const String & name) const;

private:
    std::map<String, size_t> positions;
};

}
