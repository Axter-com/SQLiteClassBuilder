/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2025 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Example #1
		sqlite3pp::setGlobalDB("myDatabase.db");
		sqlite3pp::Table<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings> my_tbl;
		for (auto& row : my_tbl)
			std::cout << row << std::endl;

	// Example #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::cout << my_tbl[i].get_ProgLangID() << std::endl;

	// Example #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::cout << r->get_VariableValueInt() << std::endl;

	// Example #4
		sqlite3pp::setGlobalDB("myDatabase.db");
		sqlite3pp::Table<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings> my_tbl;
		// Example#4a -- (C++11) Range-based loop
		for(auto& row : my_tbl)
			std::cout << row.get_ProgLangID() << " | " << row.get_VariableName() << " | " << row.get_VariableValueStr() << " | " << row.get_VariableValueBool() << " | " << row.get_VariableValueInt() << " | " << std::endl;

		// Example#4b -- C++ style iteration
		for (auto row = my_tbl.begin(); row != my_tbl.end(); ++row) 
			std::cout << row->get_ProgLangID() << " | " << row->get_VariableName() << " | " << row->get_VariableValueStr() << " | " << row->get_VariableValueBool() << " | " << row->get_VariableValueInt() << " | " << std::endl;

		// Example#4c -- C style iteration
		for (int row = 0; row < my_tbl.size(); ++row) 
			std::cout << my_tbl[row].get_ProgLangID() << " | " << my_tbl[row].get_VariableName() << " | " << my_tbl[row].get_VariableValueStr() << " | " << my_tbl[row].get_VariableValueBool() << " | " << my_tbl[row].get_VariableValueInt() << " | " << std::endl;
For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESPECIALSETTINGS_H
#define SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESPECIALSETTINGS_H
#include <string>
#include "sqlite3pp_ez.h"

class SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings: public sqlite3pp::sql_base
{
	// A member variable for each field in the table
protected:
	Integer ProgLangID = 0;
	Text VariableName;
	Text VariableValueStr;
	Boolean VariableValueBool = false;
	Integer VariableValueInt = 0;

public:
	using StrType = std::string;
	// Constructors
	SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings() {} // Default constructor
	template <class T> SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelectColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return "ProgramLanguageSpecialSettings" ; }
	static StrType getColumnNames() { return "\"ProgLangID\", \"VariableName\", \"VariableValueStr\", \"VariableValueBool\", \"VariableValueInt\""; }
	static StrType getSelectColumnNames() { return "\"ProgLangID\", \"VariableName\", \"VariableValueStr\", \"VariableValueBool\", \"VariableValueInt\""; }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  "'" + sqlite3pp::to_string( ProgLangID) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( VariableName) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( VariableValueStr) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( VariableValueBool) + "'";
		strtype +=  ",'" + sqlite3pp::to_string( VariableValueInt) + "'";
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> ProgLangID >> VariableName >> VariableValueStr >> VariableValueBool >> VariableValueInt;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		ProgLangID = t.get_ProgLangID();
		VariableName = t.get_VariableName();
		VariableValueStr = t.get_VariableValueStr();
		VariableValueBool = t.get_VariableValueBool();
		VariableValueInt = t.get_VariableValueInt();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 5; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Integer& get_ProgLangID() const {return ProgLangID;}
	const Text& get_VariableName() const {return VariableName;}
	const Text& get_VariableValueStr() const {return VariableValueStr;}
	const Boolean& get_VariableValueBool() const {return VariableValueBool;}
	const Integer& get_VariableValueInt() const {return VariableValueInt;}

	// A set_ function for each field in the table.
	void set_ProgLangID(const Integer& data__) {ProgLangID = data__;}
	void set_VariableName(const Text& data__) {VariableName = data__;}
	void set_VariableValueStr(const Text& data__) {VariableValueStr = data__;}
	void set_VariableValueBool(const Boolean& data__) {VariableValueBool = data__;}
	void set_VariableValueInt(const Integer& data__) {VariableValueInt = data__;}

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings> tbl(DbFileNameArg("myDatabase.db"));
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
		t.os << ProgLangID << t.d << t.str(VariableName) << t.d << t.str(VariableValueStr) << t.d << VariableValueBool << t.d << VariableValueInt;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings& t);
	friend std::wostream& operator<<(std::wostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings& t);
	static StrType Delimiter() { return "," ; }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const SQL_ProgamLanguageDetails_db_table_ProgramLanguageSpecialSettings& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_PROGAMLANGUAGEDETAILS_DB_TABLE_PROGRAMLANGUAGESPECIALSETTINGS_H
