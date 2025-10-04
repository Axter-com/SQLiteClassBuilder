/* 
	!!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  
	GNU General Public License
	Copyright (C) 2025 David Maisonave (www.axter.com)
	The sqlite3pp_ez source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	See SqlClassBuilder.h for summary and usage details.
*/

#include "SqlClassBuilder.h"
#include <windows.h>
#include <stringapiset.h>

#ifdef SQLITE3PP_LOADABLE_EXTENSION
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT3
#endif

#include "sqlite3pp_ez.h"
#include <fstream>
#include <iostream>
#include <direct.h>
#include <cassert>
#include "sqlite3pp.h"
#include "SQL_ProgamLanguageDetails_db__Master_Header.hpp"

#define V_COUT(VB, V)		{if (sqlite3pp::sql_base::GetVerbosityLevel() >= sqlite3pp::VBLV_##VB) {std::cout << __FUNCTION__ << ":" << #VB << ": " << V << std::endl;} }

namespace sqlite3pp
{
	class SqlClassBuilderDbData
	{
	public:
		virtual ~SqlClassBuilderDbData()
		{
		}
		virtual bool GetTableNamesAndTypes(
			std::map<std::string, std::string>& TableNamesAndTypes, // TableNamesAndTypes = Key is table/view name, and value is type (table or view)
			std::map<std::string, std::map<std::string, std::string> >& DataNamesAndTypes, // DataNamesAndTypes = First key is table/view name. Second key is member name, and value is member type (TEXT, CHAR, NCHAR, INTEGER, BOOLEAN, BLOB, etc...)
			const CommonProgLangSettings& commonProgLangSettings,
			const std::map<std::string, std::string>& ProgLangSpecialSettings) = 0;
			// Warning: Table and view names must be unique across both tables and views. This is required for SQLite databases, but not sure about other database types.
	protected:
		virtual std::string GetType(const std::string& tblVw, const std::string& colName, const char* str_org, const CommonProgLangSettings& commonProgLangSettings, const std::map<std::string, std::string>& ProgLangSpecialSettings)
		{
			std::string strtype = GetType_Sub(tblVw, colName, str_org, commonProgLangSettings, ProgLangSpecialSettings);
			V_COUT(DETAIL, "Using type '" << strtype << "' for column '" << colName << "' in table/view '" << tblVw << "'.");
			return strtype;
		}

		virtual std::string GetType_Sub(const std::string& tblVw, const std::string& colName, const char* str_org, const CommonProgLangSettings& commonProgLangSettings, const std::map<std::string, std::string>& ProgLangSpecialSettings)
		{
			const std::string DefaultType = "Text";
			if (!str_org)
			{
				// Logging out as DEBUG because some views do return NULL value for the column type.
				V_COUT(DEBUG, "Entered with DB type NULL value for column '" << colName << "' in table/view '" << tblVw << "'.\nGracefully continuing by returning default type '" << DefaultType << "'.");
				return DefaultType;
			}
			char str[99] = { 0 };
			strcpy_s(str, str_org);
			_strupr_s(str);
			const std::string str_type = str;
			// There's no practical method for handling blob or clob other than the Blob and Clob type, so don't even include them  in an option to declare them any other way.
			if (strcmp("BLOB", str) == 0)
				return "Blob";
			if (strcmp("CLOB", str) == 0)
				return "Clob";

#ifdef SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY // If defined convert types to Resulting Affinity (int, double, or StrType)
			bool UseBaseTypes = true;
#else  // Use SQLite3 sub types and use type names
			bool UseBaseTypes = false;
#endif //SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY

			if (UseBaseTypes || commonProgLangSettings.m_MiscOptions.use_basic_types_only)
			{
				// Only output detail once
				static bool HasLoggedDetails = false;
				if (!HasLoggedDetails)
				{
					HasLoggedDetails = true;
					V_COUT(DETAIL, "Only setting DB types to basic types due to compiler #define SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY(" << UseBaseTypes << ") or input option use_basic_types_only(" << commonProgLangSettings.m_MiscOptions.use_basic_types_only << ")");
				}

				if (strcmp("INTEGER", str) == 0 || strcmp("INT", str) == 0 || strcmp("TINYINT", str) == 0 || strcmp("SMALLINT", str) == 0 || strcmp("MEDIUMINT", str) == 0 || strcmp("BIGINT", str) == 0 || strcmp("UNSIGNED BIG INT", str) == 0 || strcmp("UBIGINT", str) == 0 || strcmp("INT2", str) == 0 || strcmp("INT8", str) == 0)
					return "int";
				if (strcmp("REAL", str) == 0 || strcmp("DOUBLE", str) == 0 || strcmp("DOUBLE PRECISION", str) == 0 || strcmp("DOUBLEPRCSN", str) == 0 || strcmp("FLOAT", str) == 0 || strncmp("DECIMAL", str, 7) == 0 || strcmp("BOOLEANL", str) == 0 || strcmp("BOOLEAN", str) == 0 || strcmp("BOOL", str) == 0 || strcmp("DATE", str) == 0 || strcmp("DATETIME", str) == 0 || strcmp("TIMESTAMP", str) == 0 || strcmp("NUMERIC", str) == 0 || strcmp("NUMBER", str) == 0)
					return "double";

				if (commonProgLangSettings.m_MiscOptions.use_basic_types_only)
				{
					// String types
					if (strcmp("TEXT", str) == 0 || strncmp("CHARACTER", str, 9) == 0 || strncmp("VARCHAR", str, 7) == 0 || strncmp("VARYING CHARACTER", str, 17) == 0 || strcmp("STRING", str) == 0 || strcmp("TSTRING", str) == 0)
						return "std::string";
					if (strncmp("NVARCHAR", str, 8) == 0 || strncmp("NATIVE CHARACTER", str, 16) == 0 || strncmp("NCHAR", str, 5) == 0 || strcmp("WSTRING", str) == 0)
						return "std::wstring";
				}
			}
			else
			{
				if (strcmp("INTEGER", str) == 0)
					return "Integer";
				if (strcmp("INT", str) == 0)
					return "Int";
				if (strcmp("INT2", str) == 0)
					return "Int2";
				if (strcmp("INT8", str) == 0)
					return "Int8";
				if (strcmp("TINYINT", str) == 0)
					return "Tinyint";
				if (strcmp("SMALLINT", str) == 0)
					return "Smallint";
				if (strcmp("MEDIUMINT", str) == 0)
					return "Mediumint";
				if (strcmp("BOOLEAN", str) == 0 || strcmp("BOOLEANL", str) == 0 || strcmp("BOOL", str) == 0)
					return "Boolean";
				if (strcmp("BIGINT", str) == 0)
					return "Bigint";
				if (strcmp("UNSIGNED BIG INT", str) == 0 || strcmp("UBIGINT", str) == 0)
					return "UBigint";
				if (strcmp("DATE", str) == 0)
					return "Date";
				if (strcmp("DATETIME", str) == 0)
					return "Datetime";
				if (str_type.find("NUMERIC") == 0 || str_type.find("NUMBER") == 0)
					return "Numeric";
				if (str_type.find("DECIMAL") == 0)
					return "Decimal";
				if (strcmp("REAL", str) == 0)
					return "Real";
				if (strcmp("DOUBLE PRECISION", str) == 0)
					return "DoublePrcsn";
				if (strcmp("DOUBLEPRCSN", str) == 0)
					return "DoublePrcsn";
				if (strcmp("DOUBLE", str) == 0)
					return "Double";
				if (strcmp("FLOAT", str) == 0)
					return "Float";

				// String types
				if (strcmp("TEXT", str) == 0 || str_type.find(" SUB_TYPE TEXT") != std::string::npos || strcmp("TSTRING", str) == 0)
					return "Text";
				if (str_type.find("CHAR") == 0)
					return "Character";
				if (str_type.find("VARYING CHARACTER") == 0	|| str_type.find("VARCHAR") == 0 || strcmp("STRING", str) == 0)
					return "Varchar";
				if (str_type.find("NATIVE CHARACTER") == 0 || str_type.find("NCHAR") == 0)
					return "Nchar";
				if (str_type.find("NVARCHAR") == 0 || strcmp("WSTRING", str) == 0)
					return "Nvarchar";

				if (strcmp("TIMESTAMP", str) == 0)
					return "Bigint";
			}
			V_COUT(ERROR, "Could not find valid type for table/view '" << tblVw << "' column '" << colName << "' having type '" << str << "'");
			assert(0);
			V_COUT(WARN, "Using '" << DefaultType << "' type for column '" << tblVw << "'.'" << colName << "'instead of type '" << str << "'");
			return DefaultType; // Handle it gracefully for release mode.
		}
	};

	class SqliteClassBuilderData : public SqlClassBuilderDbData
	{
		sqlite3pp::database m_db;
		class sqlite_master
		{
		public:
			using StrType = std::string;
			static StrType getTableName() { return  "sqlite_master"; }
			static StrType getColumnNames() { return  "type, name, tbl_name, rootpage, sql"; }
			static StrType getSelectColumnNames() { return  "type, name, tbl_name, rootpage, sql"; }
			template<class T> void getStreamData(T q) { q.getter() >> type >> name >> tbl_name >> rootpage >> sql; }
			static int getColumnCount() { return 5; }
		public:
			StrType type;
			StrType name;
			StrType tbl_name;
			StrType rootpage;
			StrType sql;
		};
	public:
		SqliteClassBuilderData(const std::string& DB_filename):m_db(DB_filename.c_str())
		{
		}

		~SqliteClassBuilderData()
		{
			m_db.disconnect();
		}

		virtual bool GetTableNamesAndTypes(
			std::map<std::string, std::string>& TableNamesAndTypes,
			std::map<std::string, std::map<std::string, std::string> >& DataNamesAndTypes,
			const CommonProgLangSettings& commonProgLangSettings,
			const std::map<std::string, std::string>& ProgLangSpecialSettings)
		{
			try
			{
				using SQLiteMaster = Table<sqlite_master>;
				SQLiteMaster tbl(m_db, WhereClauseArg(T_("where (type = 'table' or type = 'view') ") ));
				for (auto& t : tbl)
				{
					TableNamesAndTypes[t.tbl_name] = t.type;
					std::string QueryStr = "SELECT * FROM \"" + t.tbl_name + "\"";
					std::shared_ptr < sqlite3pp::query> qry(sql_base::CreateQuery(m_db, QueryStr));
					if (!qry)
						return false;
					for (int i = 0; i < qry->column_count(); ++i)
					{
						if (strstr(qry->column_name(i), ":") != NULL) 
							continue;
						DataNamesAndTypes[t.tbl_name][qry->column_name(i)] = GetType(t.tbl_name, qry->column_name(i), qry->column_decltype(i), commonProgLangSettings, ProgLangSpecialSettings);
					}
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: Exception in SqliteClassBuilderData::GetTableNamesAndTypes: " << e.what() << std::endl;
				return false;
			}
			return true;
		}
	};

	SqlClassBuilder::SqlClassBuilder(const std::string& DB_filename, DatabaseTyoe databaseTyoe) :m_db_filename(DB_filename), m_DatabaseTyoe(databaseTyoe), m_BitListOfLangsToProcess(0)
	{
		switch (m_DatabaseTyoe)
		{
		default:
		case DatabaseTyoe::SQLite_Database:
			m_pData = new SqliteClassBuilderData(DB_filename);
			break;
		}
	}

	SqlClassBuilder::~SqlClassBuilder()
	{
		delete m_pData;
	}

	std::map<ProgLang, std::map<std::string, std::string> > SqlClassBuilder::m_DefaultProgLangSpecialSettings
	{
		{ProgLang::C_Lang, {{"requires_header_guards", "true"},{"header_include", "#include \"..\\sqlite3pp_ez.h\""}}},
		{ProgLang::CPP_Lang, {{"requires_header_guards", "true"},{"str_tostr", "sqlite3pp::to_string"},{"header_include", "#include <string>\n#include \"..\\sqlite3pp_ez.h\""},{"exclude_ostream_operator", "false"},{"function_protection", "public:"},}},
		{ProgLang::CSharp_Lang, {{}}},
		{ProgLang::FSharp_Lang, {{}}},
		{ProgLang::Go_Lang, {{}}},
		{ProgLang::Java_Lang, {{}}},
		{ProgLang::ObjectiveC_Lang, {{}}},
		{ProgLang::ObjectiveCPP_Lang, {{}}},
		{ProgLang::PHP_Lang, {{}}},
		{ProgLang::PowerShell_Lang, {{}}},
		{ProgLang::Python_Lang, {{}}},
		{ProgLang::VisualBasic_Lang, {{}}},
	};

	std::map<ProgLang, CommonProgLangSettings> SqlClassBuilder::m_DefaultProgLangSettings
	{ 
		//{ProgLang::NoLanguageSet, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::NoLanguageSet}},
		//{ProgLang::C_Lang, {{"", "", ""},{"", "", "h", "SQL\\"},{"//", "/*", "*/", "struct", ", ", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::C_Lang}},
		//{ProgLang::CPP_Lang, {{"", "", ""},{"", "", "hpp", "SQL\\"},{"//", "/*", "*/", "class", ": public sqlite3pp::sql_base", ", ", ", ", ", ", "protected:", false, false, true, true, false, true, false, false, true}, ProgLang::CPP_Lang}},
		//{ProgLang::CSharp_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"//", "/*", "*/", "class", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::CSharp_Lang}},
		//{ProgLang::FSharp_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::FSharp_Lang}},
		//{ProgLang::Go_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::Go_Lang}},
		//{ProgLang::Java_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::Java_Lang}},
		//{ProgLang::ObjectiveC_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::ObjectiveC_Lang}},
		//{ProgLang::ObjectiveCPP_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::ObjectiveCPP_Lang}},
		//{ProgLang::PHP_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::PHP_Lang}},
		//{ProgLang::PowerShell_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::PowerShell_Lang}},
		//{ProgLang::Python_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::Python_Lang}},
		//{ProgLang::VisualBasic_Lang, {{"", "", ""},{"", "", "", "SQL\\"},{"", "", "", "", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::VisualBasic_Lang}},
	};

	std::map<ProgLang, std::map<std::string, CommonProgLangSettings> > SqlClassBuilder::m_AlternateProgLangSettings
	{ 
		//{ProgLang::C_Lang, {{"char", {{"char*", "", ""},{"", "", "h", "SQL\\"},{"//", "/*", "*/", "struct", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::C_Lang}}}},
		//{ProgLang::C_Lang, {{"wchar_t", {{"wchar_t*", "L", ""},{"", "", "h", "SQL\\"},{"//", "/*", "*/", "struct", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::C_Lang}}}},
		//{ProgLang::C_Lang, {{"TCHAR", {{"TCHAR*", "_T(", ")"},{"", "", "h", "SQL\\"},{"//", "/*", "*/", "struct", "", ", ", ", ", "", false, false, true, false, false, true, false, false}, ProgLang::C_Lang}}}},
		//{ProgLang::CPP_Lang, {{"string", {{"std::string", "", ""},{"", "", "hpp", "SQL\\"},{"//", "/*", "*/", "class", ": public sqlite3pp::sql_base", ", ", ", ", "protected:", false, false, true, true, false, true, false, false, true}, ProgLang::CPP_Lang}}}},
		//{ProgLang::CPP_Lang, {{"wstring", {{"std::wstring", "L", ""},{"", "", "hpp", "SQL\\"},{"//", "/*", "*/", "class", ": public sqlite3pp::sql_base", ", ", ", ", "protected:", false, false, true, true, false, true, false, false, true}, ProgLang::CPP_Lang}}}},
		//{ProgLang::CPP_Lang, {{"tstring", {{"sqlite3pp::tstring", "_T(", ")"},{"", "", "hpp", "SQL\\"},{"//", "/*", "*/", "class", ": public sqlite3pp::sql_base", ", ", ", ", "protected:", false, false, true, true, false, true, false, false, true}, ProgLang::CPP_Lang}}}},
	};

	const std::vector<std::string> SqlClassBuilder::m_SqliteKeywordsMixCase // List of SQLite3.h and SQLite3pp.h keywords which should not be used as class or variable names
	{
		"Bigint",
		"Bool",
		"Boolean",
		"Character",
		"Date",
		"Datetime",
		"Decimal",
		"Double",
		"DoublePrcsn",
		"Float",
		"Int",
		"Int2",
		"Int8",
		"Integer",
		"Mediumint",
		"Nchar",
		"Numeric",
		"Nvarchar",
		"Real",
		"Smallint",
		"StrType",
		"Text",
		"Tinyint",
		"UBigint",
		"Varchar" 
	};

	const std::vector<std::string> SqlClassBuilder::m_CppKeywordsLowerCase // List of C++ keywords which should not be used as class or variable names
	{
		"alignas",
		"alignof",
		"and",
		"and_eq",
		"asm",
		"atomic_cancel",
		"atomic_commit",
		"atomic_noexcept",
		"auto",
		"bitand",
		"bitor",
		"bool",
		"break",
		"case",
		"catch",
		"char",
		"char16_t",
		"char32_t",
		"char8_t",
		"class",
		"co_await",
		"co_return",
		"co_yield",
		"compl",
		"concept",
		"const",
		"const_cast",
		"consteval",
		"constexpr",
		"constinit",
		"continue",
		"contract_assert",
		"decltype",
		"default",
		"delete",
		"do",
		"double",
		"dynamic_cast",
		"else",
		"enum",
		"explicit",
		"export",
		"extern",
		"false",
		"float",
		"for",
		"friend",
		"goto",
		"if",
		"inline",
		"int",
		"long",
		"mutable",
		"namespace",
		"new",
		"noexcept",
		"not",
		"not_eq",
		"nullptr",
		"operator",
		"or",
		"or_eq",
		"private",
		"protected",
		"public",
		"reflexpr",
		"register",
		"reinterpret_cast",
		"requires",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"static_assert",
		"static_cast",
		"struct",
		"switch",
		"synchronized",
		"template",
		"this",
		"thread_local",
		"throw",
		"true",
		"try",
		"typedef",
		"typeid",
		"typename",
		"union",
		"unsigned",
		"using",
		"virtual",
		"void",
		"volatile",
		"wchar_t",
		"while",
		"xor",
		"xor_eq"
	};

	class NameChangeDetails
	{
	public:
		std::string originalName;
		std::string OptionalAdditionalChangeDetails; // If populated, this will be added as a comment in the generated code. It's main usage is for custom programming languages and/or derived classes to add additional information about the name change.
		int changeDetails = 0; // Bit field of ChangeDetails values
	};
	char SqlClassBuilder::m_SpaceCharReplacementCharacter = '_';

	std::string SqlClassBuilder::ConvertToValidAlphaNum(std::string str, ChangeDetails& changeDetails) // Convert all characters to alpha-numeric, and make sure first character is a letter.
	{
		std::string strAfterSpaceCheck = ReplaceAll(str, " ", std::string(1, m_SpaceCharReplacementCharacter));
		if (str != strAfterSpaceCheck)
		{
			changeDetails = static_cast<ChangeDetails>(changeDetails | SpaceCharInName);
			changeDetails = static_cast<ChangeDetails>(changeDetails | NeedToUseOriginalSqlNameInSqlQuery);
			str = strAfterSpaceCheck;
		}

		// Replace all non alpha-numeric characters with replacement character
		std::replace_if(str.begin(), str.end(), [](unsigned char c) {return !std::isalnum(c);}, m_SpaceCharReplacementCharacter);
		if (str != strAfterSpaceCheck)
		{
			changeDetails = static_cast<ChangeDetails>(changeDetails | InvalidChars);
			changeDetails = static_cast<ChangeDetails>(changeDetails | NeedToUseOriginalSqlNameInSqlQuery);
		}

		if (!isalpha(str[0]))
		{
			str[0] = m_SpaceCharReplacementCharacter;
			changeDetails = static_cast<ChangeDetails>(changeDetails | ChangedFirstChar);
			changeDetails = static_cast<ChangeDetails>(changeDetails | NeedToUseOriginalSqlNameInSqlQuery);
		}
		return str;
	}


	std::string SqlClassBuilder::MakeValidClassOrVarName(const std::string& name, bool ColumnName)
	{
		ChangeDetails changeDetails = ColumnName? WasColumnName : TableOrViewName;
		std::string validName = MakeValidClassOrVarName(name, changeDetails);
		if (name != validName)
		{
			NameChangeDetails nameChangeDetails = { name, "", changeDetails};
			m_NotUsingOriginalName[validName] = nameChangeDetails; // key = changed name, value = original name and change details.
			V_COUT(DETAIL, "Converted name '" << name << "' to name '" << validName << "' due to either invalid naming convention, or keyword conflict.");
			if (changeDetails & NeedToUseOriginalSqlNameInSqlQuery)
			{
				m_NeedToUseOriginalSqlNameInSqlQuery[validName] = name; // key = changed name, value = original name.
				V_COUT(DETAIL, "Will need to use original name '" << name << "' in SQL query, instead of converted valid name '" << validName << "'");
			}
		}
		return validName;
	}

	std::string SqlClassBuilder::MakeValidClassOrVarName(const std::string& name, ChangeDetails& changeDetails)
	{
		std::string newname = ConvertToValidAlphaNum(name, changeDetails);
		std::string name_lower = sqlite3pp::str_tolower(newname);
		std::string name_upper = sqlite3pp::str_toupper(newname);
		bool match_all_lower = (std::find(m_CppKeywordsLowerCase.begin(), m_CppKeywordsLowerCase.end(), name_lower) != m_CppKeywordsLowerCase.end() ||
			std::find(m_OtherProgLangKeywordsLowerCase.begin(), m_OtherProgLangKeywordsLowerCase.end(), name_lower) != m_OtherProgLangKeywordsLowerCase.end());
		bool match_all_upper = std::find(m_OtherProgLangKeywordsUpperCase.begin(), m_OtherProgLangKeywordsUpperCase.end(), name_upper) != m_OtherProgLangKeywordsUpperCase.end();
		bool originally_all_upper = newname == sqlite3pp::str_toupper(newname);
		bool originally_all_lower = newname == sqlite3pp::str_tolower(newname);

		if (originally_all_upper && match_all_upper)
		{
			changeDetails = static_cast<ChangeDetails>(changeDetails | UppperCaseKeywordConflict);
			if (match_all_lower)
			{
				changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeMixCase);
				newname[0] = std::toupper(newname[0]); // Make it mix case
			}
			else
			{
				changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeLowerCase);
				return str_tolower(newname);
			}
		}
		else if (originally_all_lower && match_all_lower)
		{
			changeDetails = static_cast<ChangeDetails>(changeDetails | LowerCaseKeywordConflict);
			if (match_all_upper)
			{
				changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeRevMixCase);
				newname[0] = std::tolower(newname[0]); // Make it reverse mix case
			}
			else
			{
				changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeUpperCase);
				return str_toupper(newname);
			}
		}
		
		if (std::find(m_SqliteKeywordsMixCase.begin(), m_SqliteKeywordsMixCase.end(), newname) != m_SqliteKeywordsMixCase.end() ||
			std::find(m_OtherProgLangKeywordsMixCase.begin(), m_OtherProgLangKeywordsMixCase.end(), newname) != m_OtherProgLangKeywordsMixCase.end())
		{
			changeDetails = static_cast<ChangeDetails>(changeDetails | MixCaseKeywordConflict);
			if (match_all_lower)
			{
				if (match_all_upper)
				{
					bool append_underscore = true;
					if (newname.size() > 1)
					{
						for(size_t i = 0; i < newname.size();++i)
						{
							newname = str_toupper(newname);
							newname[i] = std::tolower(newname[i]); // Special upper mix case
							if (std::find(m_SqliteKeywordsMixCase.begin(), m_SqliteKeywordsMixCase.end(), newname) == m_SqliteKeywordsMixCase.end() &&
								std::find(m_OtherProgLangKeywordsMixCase.begin(), m_OtherProgLangKeywordsMixCase.end(), newname) == m_OtherProgLangKeywordsMixCase.end())
							{
								changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithSpecialMixUpperCase);
								append_underscore = false;
								break;
							}
						}
						if (append_underscore) 
						{
							for(size_t i = 0; i < newname.size();++i)
							{
								newname = str_tolower(newname);
								newname[i] = std::toupper(newname[i]); // Special lower mix case
								if (std::find(m_SqliteKeywordsMixCase.begin(), m_SqliteKeywordsMixCase.end(), newname) == m_SqliteKeywordsMixCase.end() &&
									std::find(m_OtherProgLangKeywordsMixCase.begin(), m_OtherProgLangKeywordsMixCase.end(), newname) == m_OtherProgLangKeywordsMixCase.end())
								{
									changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithSpecialMixLowerCase);
									append_underscore = false;
									break;
								}
							}
						}
					}
					
					if (append_underscore)
					{ // Do this as last resort, because we'll have to make sure the SQL query uses the original name in the query.
						changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithAppendUnderscore);
						changeDetails = static_cast<ChangeDetails>(changeDetails | NeedToUseOriginalSqlNameInSqlQuery);
						newname = newname + "____";
					}
				}
				else
				{
					changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeUpperCase);
					newname = str_toupper(newname);
				}
			}
			else
			{
				changeDetails = static_cast<ChangeDetails>(changeDetails | FixedKeywordConflictWithMakeLowerCase);
				newname = str_tolower(newname);
			}
		}

		return newname;
	}

	std::set<std::string> SqlClassBuilder::m_OtherProgLangKeywordsMixCase // List of other programming languages mix case keywords which should not be used as class or variable names
	{
	};

	std::set<std::string> SqlClassBuilder::m_OtherProgLangKeywordsLowerCase // List of other programming languages lower case keywords which should not be used as class or variable names
	{
	};

	std::set<std::string> SqlClassBuilder::m_OtherProgLangKeywordsUpperCase // List of other programming languages upper case keywords which should not be used as class or variable names
	{
	};

	static std::string GetValidFuncName(std::string name)
	{
		//ToDo: Use regex to make sure all values are AlphaNum
		replace_all(name, " ", "__");
		return name;
	}
	bool SqlClassBuilder::CreateAllFiles(ProgLang BitListOfTargetLanguages)
	{
		return CreateAllFiles(static_cast<std::uint64_t>(BitListOfTargetLanguages));
	}

	bool SqlClassBuilder::CreateAllFiles(std::uint64_t BitListOfTargetLanguages)
	{
		if (BitListOfTargetLanguages == static_cast<std::uint64_t>(ProgLang::NoLanguageSet) && m_LangsToProcess.empty())
			return false;
		m_BitListOfLangsToProcess = BitListOfTargetLanguages;
		if (!Initialize())
			return false;
		return CreateFiles();
	}

	bool SqlClassBuilder::CreateAllFiles(std::set<std::uint64_t> AllTheProgrammingLanguagesToProcess) // Can be overridden for custom programming languages requirements.
	{
		if (AllTheProgrammingLanguagesToProcess.empty())
			return false;
		m_LangsToProcess = AllTheProgrammingLanguagesToProcess;
		if (!Initialize())
			return false;
		return CreateFiles();
	}

	bool SqlClassBuilder::CreateFiles()	// Called by both CreateAllFiles, and does the real work in creating files. Can be overridden for custom programming languages requirements.
	{
		bool ReturnValue = true;
		for (auto& lang : m_LangsToProcess)
		{
			if (m_pData->GetTableNamesAndTypes(m_TableNamesAndTypes, m_DataNamesAndTypes, m_ProgLangCommonSettings[static_cast<ProgLang>(lang)], m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]))
			{
				std::vector<std::string> FileList;
				for (auto& tb : m_TableNamesAndTypes)
				{
					std::string ClassName, FileNameOnly;
					std::string FileName = GetFileNameAndClassName(tb.first, FileNameOnly, ClassName, static_cast<ProgLang>(lang));
					FileList.push_back(FileNameOnly);
					std::ofstream myfile;
					myfile.open(FileName.c_str(), std::ios_base::out);
					if (!myfile.is_open())
					{
						V_COUT(ERROR, "Failed to open file '" << FileName << "'");
						return false;
					}
					const std::string HeaderGuards = GetValidFuncName(sqlite3pp::str_toupper(ClassName + "_H"));
					if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.include_comments)
					{
						if (!AddCommentsToFile(myfile, static_cast<ProgLang>(lang), ClassName, tb.first))
							V_COUT(WARN, "Failed to add comments to file '" << FileName << "'");
					}
					if (m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].find("requires_header_guards") != m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].end() &&
						m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]["requires_header_guards"] == "true")
					{
						myfile << "#ifndef " << HeaderGuards << std::endl;
						myfile << "#define " << HeaderGuards << std::endl;
					}
					if (m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].find("header_include") != m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].end() &&
						!m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]["header_include"].empty())
						myfile << m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]["header_include"] << std::endl;

					if (!m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.ProgLangClassType.empty())
					{
						myfile << "\n" << m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.ProgLangClassType << " " << GetValidFuncName(ClassName) << m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.ParrentClass << "\n{" << std::endl;
						if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.include_table_interface)
						{
							if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.include_comments && !m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.SingleLineComment.empty())
								myfile << "\t" << m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.SingleLineComment << " A member variable for each field in the table" << std::endl;
							if (!m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.MemberVarProtectionType.empty())
								myfile << m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.MemberVarProtectionType << std::endl;

							const std::map<std::string, std::string>& columns = m_DataNamesAndTypes[tb.first];
							// Define data member variables associated with the table/view
							for (auto& c : columns)
								myfile << "\t" << c.second << " " << GetValidFuncName(c.first) << InitializeValue(c.second, static_cast<ProgLang>(lang)) << ";" << std::endl;
							if (m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].find("function_protection") != m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)].end() &&
								!m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]["function_protection"].empty())
								myfile << "\n" << m_ProgLangSpecialSettings[static_cast<ProgLang>(lang)]["function_protection"] << std::endl;
							if (!m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_TypeOptions.str_type.empty())
							{
								// Create a define type for strings
								myfile << "\tusing StrType = " << m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_TypeOptions.str_type << ";" << std::endl;
							}
							if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.create_constructor)
							{
								if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.include_comments)
									myfile << "\t// Constructors" << std::endl;
								// These constructors are only useful if method setData is created.
								myfile << "\t" << GetValidFuncName(ClassName) << "() {}";  // Allow default constructor to still work
								if (m_ProgLangCommonSettings[static_cast<ProgLang>(lang)].m_MiscOptions.include_comments)
									myfile << " // Default constructor";
								myfile << std::endl;
							}

						}
					}
				}
			}
			else
				ReturnValue = false;
		}
		return ReturnValue;
	}

	std::string SqlClassBuilder::InitializeValue(std::string TypeName, ProgLang lang)
	{
		TypeName = str_toupper(TypeName);
		if (!m_ProgLangCommonSettings[lang].m_MiscOptions.initialize_member_variables)
			return "";
		if (TypeName == "BOOLEAN")
			return " = false";
		if (TypeName == "INTEGER" || TypeName == "INT" || TypeName == "INT2" || TypeName == "INT8" || TypeName == "TINYINT" ||
			TypeName == "SMALLINT" || TypeName == "MEDIUMINT" || TypeName == "BIGINT" || TypeName == "UNSIGNED BIG INT" || TypeName == "UBIGINT")
			return " = 0";
		if (TypeName == "REAL" || TypeName == "DOUBLE PRECISION" || TypeName == "DOUBLEPRCSN" || TypeName == "NUMERIC" || TypeName == "DECIMAL" || TypeName == "DOUBLE" || TypeName == "FLOAT")
			return " = 0.0f";
		if (m_ProgLangCommonSettings[lang].m_MiscOptions.initialize_str_member_var)
		{
			if (TypeName == "TEXT")
				return " = " + m_ProgLangCommonSettings[lang].m_TypeOptions.str_prefix + "\"\"" + m_ProgLangCommonSettings[lang].m_TypeOptions.str_suffix;
			if (TypeName == "TEXT" || TypeName == "CHARACTER" || TypeName == "VARCHAR")
				return " = \"\"";
			if (TypeName == "NCHAR" || TypeName == "NVARCHAR")
				return " = " + m_ProgLangCommonSettings[lang].m_TypeOptions.str_prefix + "\"\"" + m_ProgLangCommonSettings[lang].m_TypeOptions.str_suffix;
		}
		return "";
	}

	bool SqlClassBuilder::AddCommentsToFile(std::ofstream& myfile, ProgLang lang, const std::string& ClassName, const std::string& TableName)
	{
		if (m_ProgLangCommonSettings[lang].m_MiscOptions.SartOfMultiLineComment.empty() || m_ProgLangCommonSettings[lang].m_MiscOptions.EndOfMultiLineComment.empty())
		{
			V_COUT(WARN, "Failed to add comments because program language '" << static_cast<std::uint64_t>(lang) << "' is missing SartOfMultiLineComment and/or EndOfMultiLineComment.");
			return false;
		}
		static const char TopHeaderCommnetsPrt1[] = " This file was automatically generated using [SqlClassBuilder and Sqlite3pp_EZ].\nSqlClassBuilder and Sqlite3pp_EZ Copyright (C) 2025 David Maisonave (http::\\www.axter.com)";
		static const char TopHeaderCommnetsPrt2[] = "\nFor more details see  https://github.com/David-Maisonave/sqlite3pp_EZ\n";
		myfile << m_ProgLangCommonSettings[lang].m_MiscOptions.SartOfMultiLineComment + TopHeaderCommnetsPrt1 << std::endl;
		if (lang == ProgLang::CPP_Lang)
		{
			myfile << "Example Usage:\t\t(Using sqlite3pp::Table container)" << std::endl;
			myfile << "\t// Example #1\n\t\tsqlite3pp::setGlobalDB(\"myDatabase.db\");" << std::endl;
			myfile << "\t\tsqlite3pp::Table<" << ClassName << "> my_tbl;\n\t\tfor (auto& row : my_tbl)\n\t\t\tstd::cout << row << std::endl;\n" << std::endl;
			const std::map<std::string, std::string>& columns = m_DataNamesAndTypes[TableName];
			if (columns.size())
			{
				std::string outType = "std::cout";
				std::string ColumnSep = " << \" | \"";
				myfile << "\t// Example #2\n\t\tsqlite3pp::setGlobalDB(\"myDatabase.db\");" << std::endl;
				myfile << "\t\tsqlite3pp::Table<" << ClassName << "> my_tbl;";

				myfile << "\n\t\t// Example#2a -- (C++11) Range-based loop";
				myfile << "\n\t\tfor(auto& row : my_tbl)\n";
				myfile << "\t\t\t" << outType;
				for (auto& c : columns)
					myfile << " << row.get_" << GetValidFuncName(c.first) << "()" << ColumnSep;
				myfile << " << std::endl;" << std::endl;

				myfile << "\n\t\t// Example#2b -- C++ style iteration";
				myfile << "\n\t\tfor (auto row = my_tbl.begin(); row != my_tbl.end(); ++row) \n";
				myfile << "\t\t\t" << outType;
				for (auto& c : columns)
					myfile << " << row->get_" << GetValidFuncName(c.first) << "()" << ColumnSep;
				myfile << " << std::endl;" << std::endl;

				myfile << "\n\t\t// Example#2c -- C style iteration";
				myfile << "\n\t\tfor (int row = 0; row < my_tbl.size(); ++row) \n";
				myfile << "\t\t\t" << outType;
				for (auto& c : columns)
					myfile << " << my_tbl[row].get_" << GetValidFuncName(c.first) << "()" << ColumnSep;
				myfile << " << std::endl;" << std::endl;
			}
		}
		myfile << TopHeaderCommnetsPrt2 << m_ProgLangCommonSettings[lang].m_MiscOptions.EndOfMultiLineComment << std::endl;
		return false;
	}

	std::string SqlClassBuilder::GetFileNameAndClassName(const std::string& TableName, std::string& FileNameOnly, std::string& ClassName, ProgLang lang) // Populates ClassName and FileNameOnly. Returns full FileName with directory path. Can be overridden for custom programming languages requirements.
	{
		if (m_ProgLangCommonSettings[lang].m_DestFileOptions.filename_prefix.empty())
			m_ProgLangCommonSettings[lang].m_DestFileOptions.filename_prefix = "SQL_" + sqlite3pp::ConvertToAlphaNum(sqlite3pp::getFileName(m_db_filename)) + "_";
		if (m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder.size())
		{
			if (m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder[m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder.size() - 1] == '/')
				m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder[m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder.size() - 1] = '\\';
			else if (m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder[m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder.size() - 1] != '\\')
				m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder += "\\";
			V_COUT(DEBUG, "Using destination path '" << m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder << "'.");
		}
		if (!sqlite3pp::DirExists(m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder))
		{ // ToDo: Add logic to attempt making parrent folder of dest_folder if parrent doesn't exist.
			if (_mkdir(m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder.c_str()) != 0)
			{
				V_COUT(ERROR, "Failed to create folder '" << m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder << "'");
				return "";
			}
		}

		ClassName = m_ProgLangCommonSettings[lang].m_DestFileOptions.filename_prefix + sqlite3pp::ConvertToAlphaNum(TableName) + m_ProgLangCommonSettings[lang].m_DestFileOptions.filename_suffix;
		FileNameOnly = ClassName + "." + m_ProgLangCommonSettings[lang].m_DestFileOptions.file_type;
		std::string FileName = m_ProgLangCommonSettings[lang].m_DestFileOptions.dest_folder + FileNameOnly;
		return FileName;
	}

	bool SqlClassBuilder::Initialize()	// Called by CreateAllFiles before creating any files. Can be overridden to perform any initialization needed before creating files for custom programming languages.
	{
		PopulateLangsToProcess();
		return true;
	}

	bool SqlClassBuilder::PopulateLangsToProcess() // Called by Initialize, and can be overridden for custom programming languages requirements.
	{
		bool ReturnValue = true;
		if (m_LangsToProcess.empty())
		{
			for (std::uint64_t i = 1; i < static_cast<std::uint64_t>(ProgLang::Last_Lang); i = i << 1)
			{
				if ((m_BitListOfLangsToProcess & i) == i)
				{
					m_LangsToProcess.insert(i);
					if (!AddProgLangSettings(static_cast<ProgLang>(i)))
						ReturnValue = false;
				}
			}
		}
		else
		{
			for(auto& lang : m_LangsToProcess)
			{
				if (!AddProgLangSettings(static_cast<ProgLang>(lang)))
					ReturnValue = false;
			}
		}
		return ReturnValue;
	}

	bool SqlClassBuilder::AddProgLangSettings(ProgLang progLang) // Called by PopulateLangsToProcess for each programming language in m_BitListOfLangsToProcess or in m_LangsToProcess. Can be overridden for custom programming languages requirements.
	{
		if (m_ProgLangCommonSettings.find(progLang) == m_ProgLangCommonSettings.end())
		{
			if (m_DefaultProgLangSettings.find(progLang) == m_DefaultProgLangSettings.end())
			{
				m_ProgLangCommonSettings[progLang] = m_DefaultProgLangSettings[ProgLang::NoLanguageSet]; // NoLanguageSet has default settings
			}
			else
				m_ProgLangCommonSettings[progLang] = m_DefaultProgLangSettings[progLang];
		}

		if (m_ProgLangSpecialSettings.find(progLang) == m_ProgLangSpecialSettings.end())
		{
			if (m_DefaultProgLangSpecialSettings.find(progLang) == m_DefaultProgLangSpecialSettings.end())
			{
				m_ProgLangSpecialSettings[progLang] = m_DefaultProgLangSpecialSettings[ProgLang::NoLanguageSet]; // NoLanguageSet has default settings
			}
			else
				m_ProgLangSpecialSettings[progLang] = m_DefaultProgLangSpecialSettings[progLang];
		}
		return true;
	}
};