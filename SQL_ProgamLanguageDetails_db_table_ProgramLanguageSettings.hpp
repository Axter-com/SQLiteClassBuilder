/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2025 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Example #1
		sqlite3pp::setGlobalDB("myDatabase.db");
		sqlite3pp::Table<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings> my_tbl;
		for (auto& row : my_tbl)
			std::cout << row << std::endl;

	// Example #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::cout << my_tbl[i].get_ProgramLanguageName() << std::endl;

	// Example #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::cout << r->get_Create_constructor() << std::endl;

	// Example #4
		sqlite3pp::setGlobalDB("myDatabase.db");
		sqlite3pp::Table<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings> my_tbl;
		// Example#4a -- (C++11) Range-based loop
		for(auto& row : my_tbl)
			std::cout << row.get_ProgramLanguageName() << " | " << row.get_ProgLangID() << " | " << row.get_SettingsName() << " | " << row.get_strtype() << " | " << row.get_FileExt() << " | " << row.get_SingleLineComment() << " | " << row.get_StartOfMultiLineComment() << " | " << row.get_EndOfMultiLineComment() << " | " << row.get_StrPrefix() << " | " << row.get_StrSuffix() << " | " << row.get_FileNamePrefix() << " | " << row.get_FileNameSuffix() << " | " << row.get_FileDestFolder() << " | " << row.get_ProgLangClassType() << " | " << row.get_ParrentClass() << " | " << row.get_DefaultDelimiter() << " | " << row.get_GetColumnNameDelimiter() << " | " << row.get_GetSelectColumnNamesDelimiter() << " | " << row.get_MemberVarProtectionType() << " | " << row.get_Include_get_functions() << " | " << row.get_Include_set_functions() << " | " << row.get_Include_comments() << " | " << row.get_Include_table_interface() << " | " << row.get_Include_main_hdr_example() << " | " << row.get_Initialize_member_variables() << " | " << row.get_Initialize_str_member_var() << " | " << row.get_Use_basic_types_only() << " | " << row.get_Create_constructor() << " | " << std::endl;

		// Example#4b -- C++ style iteration
		for (auto row = my_tbl.begin(); row != my_tbl.end(); ++row) 
			std::cout << row->get_ProgramLanguageName() << " | " << row->get_ProgLangID() << " | " << row->get_SettingsName() << " | " << row->get_strtype() << " | " << row->get_FileExt() << " | " << row->get_SingleLineComment() << " | " << row->get_StartOfMultiLineComment() << " | " << row->get_EndOfMultiLineComment() << " | " << row->get_StrPrefix() << " | " << row->get_StrSuffix() << " | " << row->get_FileNamePrefix() << " | " << row->get_FileNameSuffix() << " | " << row->get_FileDestFolder() << " | " << row->get_ProgLangClassType() << " | " << row->get_ParrentClass() << " | " << row->get_DefaultDelimiter() << " | " << row->get_GetColumnNameDelimiter() << " | " << row->get_GetSelectColumnNamesDelimiter() << " | " << row->get_MemberVarProtectionType() << " | " << row->get_Include_get_functions() << " | " << row->get_Include_set_functions() << " | " << row->get_Include_comments() << " | " << row->get_Include_table_interface() << " | " << row->get_Include_main_hdr_example() << " | " << row->get_Initialize_member_variables() << " | " << row->get_Initialize_str_member_var() << " | " << row->get_Use_basic_types_only() << " | " << row->get_Create_constructor() << " | " << std::endl;

		// Example#4c -- C style iteration
		for (int row = 0; row < my_tbl.size(); ++row) 
			std::cout << my_tbl[row].get_ProgramLanguageName() << " | " << my_tbl[row].get_ProgLangID() << " | " << my_tbl[row].get_SettingsName() << " | " << my_tbl[row].get_strtype() << " | " << my_tbl[row].get_FileExt() << " | " << my_tbl[row].get_SingleLineComment() << " | " << my_tbl[row].get_StartOfMultiLineComment() << " | " << my_tbl[row].get_EndOfMultiLineComment() << " | " << my_tbl[row].get_StrPrefix() << " | " << my_tbl[row].get_StrSuffix() << " | " << my_tbl[row].get_FileNamePrefix() << " | " << my_tbl[row].get_FileNameSuffix() << " | " << my_tbl[row].get_FileDestFolder() << " | " << my_tbl[row].get_ProgLangClassType() << " | " << my_tbl[row].get_ParrentClass() << " | " << my_tbl[row].get_DefaultDelimiter() << " | " << my_tbl[row].get_GetColumnNameDelimiter() << " | " << my_tbl[row].get_GetSelectColumnNamesDelimiter() << " | " << my_tbl[row].get_MemberVarProtectionType() << " | " << my_tbl[row].get_Include_get_functions() << " | " << my_tbl[row].get_Include_set_functions() << " | " << my_tbl[row].get_Include_comments() << " | " << my_tbl[row].get_Include_table_interface() << " | " << my_tbl[row].get_Include_main_hdr_example() << " | " << my_tbl[row].get_Initialize_member_variables() << " | " << my_tbl[row].get_Initialize_str_member_var() << " | " << my_tbl[row].get_Use_basic_types_only() << " | " << my_tbl[row].get_Create_constructor() << " | " << std::endl;
For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESETTINGS_H
#define SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESETTINGS_H
#include <string>
#include "sqlite3pp_ez.h"

class SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings: public sqlite3pp::sql_base
{
	// A member variable for each field in the table
protected:
	Text ProgramLanguageName;
	Integer ProgLangID = 0;
	Text SettingsName;
	Text strtype; // Original column name: "StrType", change due to keyword conflict or invalid naming convention.
	Text FileExt;
	Text SingleLineComment;
	Text StartOfMultiLineComment;
	Text EndOfMultiLineComment;
	Text StrPrefix;
	Text StrSuffix;
	Text FileNamePrefix;
	Text FileNameSuffix;
	Text FileDestFolder;
	Text ProgLangClassType;
	Text ParrentClass;
	Text DefaultDelimiter;
	Text GetColumnNameDelimiter;
	Text GetSelectColumnNamesDelimiter;
	Text MemberVarProtectionType;
	Boolean Include_get_functions = false;
	Boolean Include_set_functions = false;
	Boolean Include_comments = false;
	Boolean Include_table_interface = false;
	Boolean Include_main_hdr_example = false;
	Boolean Initialize_member_variables = false;
	Boolean Initialize_str_member_var = false;
	Boolean Use_basic_types_only = false;
	Boolean Create_constructor = false;

public:
	using StrType = std::string;
	// Constructors
	SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings() {} // Default constructor
	template <class T> SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelectColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return "ProgramLanguageSettings" ; }
	static StrType getColumnNames() { return "\"ProgramLanguageName\", \"ProgLangID\", \"SettingsName\", \"strtype\", \"FileExt\", \"SingleLineComment\", \"StartOfMultiLineComment\", \"EndOfMultiLineComment\", \"StrPrefix\", \"StrSuffix\", \"FileNamePrefix\", \"FileNameSuffix\", \"FileDestFolder\", \"ProgLangClassType\", \"ParrentClass\", \"DefaultDelimiter\", \"GetColumnNameDelimiter\", \"GetSelectColumnNamesDelimiter\", \"MemberVarProtectionType\", \"Include_get_functions\", \"Include_set_functions\", \"Include_comments\", \"Include_table_interface\", \"Include_main_hdr_example\", \"Initialize_member_variables\", \"Initialize_str_member_var\", \"Use_basic_types_only\", \"Create_constructor\""; }
	static StrType getSelectColumnNames() { return "\"ProgramLanguageName\", \"ProgLangID\", \"SettingsName\", \"strtype\", \"FileExt\", \"SingleLineComment\", \"StartOfMultiLineComment\", \"EndOfMultiLineComment\", \"StrPrefix\", \"StrSuffix\", \"FileNamePrefix\", \"FileNameSuffix\", \"FileDestFolder\", \"ProgLangClassType\", \"ParrentClass\", \"DefaultDelimiter\", \"GetColumnNameDelimiter\", \"GetSelectColumnNamesDelimiter\", \"MemberVarProtectionType\", \"Include_get_functions\", \"Include_set_functions\", \"Include_comments\", \"Include_table_interface\", \"Include_main_hdr_example\", \"Initialize_member_variables\", \"Initialize_str_member_var\", \"Use_basic_types_only\", \"Create_constructor\""; }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  "'" + sqlite3pp::to_string( ProgramLanguageName) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( ProgLangID) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( SettingsName) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( strtype) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( FileExt) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( SingleLineComment) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( StartOfMultiLineComment) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( EndOfMultiLineComment) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( StrPrefix) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( StrSuffix) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( FileNamePrefix) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( FileNameSuffix) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( FileDestFolder) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( ProgLangClassType) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( ParrentClass) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( DefaultDelimiter) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( GetColumnNameDelimiter) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( GetSelectColumnNamesDelimiter) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( MemberVarProtectionType) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Include_get_functions) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Include_set_functions) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Include_comments) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Include_table_interface) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Include_main_hdr_example) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Initialize_member_variables) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Initialize_str_member_var) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Use_basic_types_only) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( Create_constructor) + "'";
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> ProgramLanguageName >> ProgLangID >> SettingsName >> strtype >> FileExt >> SingleLineComment >> StartOfMultiLineComment >> EndOfMultiLineComment >> StrPrefix >> StrSuffix >> FileNamePrefix >> FileNameSuffix >> FileDestFolder >> ProgLangClassType >> ParrentClass >> DefaultDelimiter >> GetColumnNameDelimiter >> GetSelectColumnNamesDelimiter >> MemberVarProtectionType >> Include_get_functions >> Include_set_functions >> Include_comments >> Include_table_interface >> Include_main_hdr_example >> Initialize_member_variables >> Initialize_str_member_var >> Use_basic_types_only >> Create_constructor;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		ProgramLanguageName = t.get_ProgramLanguageName();
		ProgLangID = t.get_ProgLangID();
		SettingsName = t.get_SettingsName();
		strtype = t.get_strtype();
		FileExt = t.get_FileExt();
		SingleLineComment = t.get_SingleLineComment();
		StartOfMultiLineComment = t.get_StartOfMultiLineComment();
		EndOfMultiLineComment = t.get_EndOfMultiLineComment();
		StrPrefix = t.get_StrPrefix();
		StrSuffix = t.get_StrSuffix();
		FileNamePrefix = t.get_FileNamePrefix();
		FileNameSuffix = t.get_FileNameSuffix();
		FileDestFolder = t.get_FileDestFolder();
		ProgLangClassType = t.get_ProgLangClassType();
		ParrentClass = t.get_ParrentClass();
		DefaultDelimiter = t.get_DefaultDelimiter();
		GetColumnNameDelimiter = t.get_GetColumnNameDelimiter();
		GetSelectColumnNamesDelimiter = t.get_GetSelectColumnNamesDelimiter();
		MemberVarProtectionType = t.get_MemberVarProtectionType();
		Include_get_functions = t.get_Include_get_functions();
		Include_set_functions = t.get_Include_set_functions();
		Include_comments = t.get_Include_comments();
		Include_table_interface = t.get_Include_table_interface();
		Include_main_hdr_example = t.get_Include_main_hdr_example();
		Initialize_member_variables = t.get_Initialize_member_variables();
		Initialize_str_member_var = t.get_Initialize_str_member_var();
		Use_basic_types_only = t.get_Use_basic_types_only();
		Create_constructor = t.get_Create_constructor();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 28; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_ProgramLanguageName() const {return ProgramLanguageName;}
	const Integer& get_ProgLangID() const {return ProgLangID;}
	const Text& get_SettingsName() const {return SettingsName;}
	const Text& get_strtype() const {return strtype;}
	const Text& get_FileExt() const {return FileExt;}
	const Text& get_SingleLineComment() const {return SingleLineComment;}
	const Text& get_StartOfMultiLineComment() const {return StartOfMultiLineComment;}
	const Text& get_EndOfMultiLineComment() const {return EndOfMultiLineComment;}
	const Text& get_StrPrefix() const {return StrPrefix;}
	const Text& get_StrSuffix() const {return StrSuffix;}
	const Text& get_FileNamePrefix() const {return FileNamePrefix;}
	const Text& get_FileNameSuffix() const {return FileNameSuffix;}
	const Text& get_FileDestFolder() const {return FileDestFolder;}
	const Text& get_ProgLangClassType() const {return ProgLangClassType;}
	const Text& get_ParrentClass() const {return ParrentClass;}
	const Text& get_DefaultDelimiter() const {return DefaultDelimiter;}
	const Text& get_GetColumnNameDelimiter() const {return GetColumnNameDelimiter;}
	const Text& get_GetSelectColumnNamesDelimiter() const {return GetSelectColumnNamesDelimiter;}
	const Text& get_MemberVarProtectionType() const {return MemberVarProtectionType;}
	const Boolean& get_Include_get_functions() const {return Include_get_functions;}
	const Boolean& get_Include_set_functions() const {return Include_set_functions;}
	const Boolean& get_Include_comments() const {return Include_comments;}
	const Boolean& get_Include_table_interface() const {return Include_table_interface;}
	const Boolean& get_Include_main_hdr_example() const {return Include_main_hdr_example;}
	const Boolean& get_Initialize_member_variables() const {return Initialize_member_variables;}
	const Boolean& get_Initialize_str_member_var() const {return Initialize_str_member_var;}
	const Boolean& get_Use_basic_types_only() const {return Use_basic_types_only;}
	const Boolean& get_Create_constructor() const {return Create_constructor;}

	// A set_ function for each field in the table.
	void set_ProgramLanguageName(const Text& data__) {ProgramLanguageName = data__;}
	void set_ProgLangID(const Integer& data__) {ProgLangID = data__;}
	void set_SettingsName(const Text& data__) {SettingsName = data__;}
	void set_strtype(const Text& data__) {strtype = data__;}
	void set_FileExt(const Text& data__) {FileExt = data__;}
	void set_SingleLineComment(const Text& data__) {SingleLineComment = data__;}
	void set_StartOfMultiLineComment(const Text& data__) {StartOfMultiLineComment = data__;}
	void set_EndOfMultiLineComment(const Text& data__) {EndOfMultiLineComment = data__;}
	void set_StrPrefix(const Text& data__) {StrPrefix = data__;}
	void set_StrSuffix(const Text& data__) {StrSuffix = data__;}
	void set_FileNamePrefix(const Text& data__) {FileNamePrefix = data__;}
	void set_FileNameSuffix(const Text& data__) {FileNameSuffix = data__;}
	void set_FileDestFolder(const Text& data__) {FileDestFolder = data__;}
	void set_ProgLangClassType(const Text& data__) {ProgLangClassType = data__;}
	void set_ParrentClass(const Text& data__) {ParrentClass = data__;}
	void set_DefaultDelimiter(const Text& data__) {DefaultDelimiter = data__;}
	void set_GetColumnNameDelimiter(const Text& data__) {GetColumnNameDelimiter = data__;}
	void set_GetSelectColumnNamesDelimiter(const Text& data__) {GetSelectColumnNamesDelimiter = data__;}
	void set_MemberVarProtectionType(const Text& data__) {MemberVarProtectionType = data__;}
	void set_Include_get_functions(const Boolean& data__) {Include_get_functions = data__;}
	void set_Include_set_functions(const Boolean& data__) {Include_set_functions = data__;}
	void set_Include_comments(const Boolean& data__) {Include_comments = data__;}
	void set_Include_table_interface(const Boolean& data__) {Include_table_interface = data__;}
	void set_Include_main_hdr_example(const Boolean& data__) {Include_main_hdr_example = data__;}
	void set_Initialize_member_variables(const Boolean& data__) {Initialize_member_variables = data__;}
	void set_Initialize_str_member_var(const Boolean& data__) {Initialize_str_member_var = data__;}
	void set_Use_basic_types_only(const Boolean& data__) {Use_basic_types_only = data__;}
	void set_Create_constructor(const Boolean& data__) {Create_constructor = data__;}

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings> tbl(DbFileNameArg("myDatabase.db"));
			tbl.setDelimit("|"); // Change delimiter
			std::cout << tbl; // Send data to screen with the changed delimiter

			std::ofstream ofs ("data.csv", std::ofstream::out);
			tbl.setDelimit(","); // Change delimiter
			ofs << tbl; // Write data to a CSV file using the changed "," delimiter.

			tbl.out(std::cout); // Send data to screen using out() member function.
	To exclude TableOStream interface, set exclude_ostream_operator to true when creating this class using SQLiteClassBuilder.
	*/
	template<class T> T& OStream(T& t) const
	{
		t.os << t.str(ProgramLanguageName) << t.d << ProgLangID << t.d << t.str(SettingsName) << t.d << t.str(strtype) << t.d << t.str(FileExt) << t.d << t.str(SingleLineComment) << t.d << t.str(StartOfMultiLineComment) << t.d << t.str(EndOfMultiLineComment) << t.d << t.str(StrPrefix) << t.d << t.str(StrSuffix) << t.d << t.str(FileNamePrefix) << t.d << t.str(FileNameSuffix) << t.d << t.str(FileDestFolder) << t.d << t.str(ProgLangClassType) << t.d << t.str(ParrentClass) << t.d << t.str(DefaultDelimiter) << t.d << t.str(GetColumnNameDelimiter) << t.d << t.str(GetSelectColumnNamesDelimiter) << t.d << t.str(MemberVarProtectionType) << t.d << Include_get_functions << t.d << Include_set_functions << t.d << Include_comments << t.d << Include_table_interface << t.d << Include_main_hdr_example << t.d << Initialize_member_variables << t.d << Initialize_str_member_var << t.d << Use_basic_types_only << t.d << Create_constructor;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings& t);
	friend std::wostream& operator<<(std::wostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings& t);
	static StrType Delimiter() { return "," ; }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSettings& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESETTINGS_H
