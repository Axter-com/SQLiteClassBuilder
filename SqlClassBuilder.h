/*	SqlClassBuilder.h
	!!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!  !!! Under construction !!!
	GNU General Public License
	Copyright (C) 2025 David Maisonave (www.axter.com)
	The sqlite3pp_ez source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	# Summary
	SqlClassBuilder is used to create source code in multiple programming languages based on the tables and views in a database.
	The main difference between SQLiteClassBuilder and SqlClassBuilder, is that SQLiteClassBuilder only supports creating C++ source code and SqlClassBuilder is designed to support multiple programming languages.
	Also one of SqlClassBuilder goals is to support other database types in the future, like MySQL, ODBC, PostgreSQL, SQL Server, Oracle, etc.

	For usage examples see the following links:
		https://github.com/Axter-com/SQLiteClassBuilder
		https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQLCLASSBUILDER_H
#define SQLCLASSBUILDER_H

#include <string>
#include <map>
#include <set>
#include <vector>

namespace sqlite3pp
{
	enum class ProgLang : std::uint64_t  { // Warning: Only C++ is fully implemented at this time.
		NoLanguageSet = 0,
		C_Lang = 1,				// Work in progress
		CPP_Lang = 2,			//Default Language
		CSharp_Lang = 4,
		FSharp_Lang = 8,
		Go_Lang = 16,
		Java_Lang = 32,
		ObjectiveC_Lang = 64,
		ObjectiveCPP_Lang = 128,
		PHP_Lang = 256,
		PowerShell_Lang = 512,
		Python_Lang = 1024,
		VisualBasic_Lang = 2048,
		ALL_Supported_Langs = CPP_Lang,
		ALL_Beta_Supported_Langs = CPP_Lang,
		ALL_Supported_And_Langs_UnderConstruction = C_Lang | CPP_Lang,

		// Languages not yet ready for consideration
		ClassicVisualBasic_Lang = 4096,
		Cobol_Lang = 8192,
		D_Lang = 16384,
		Dart_Lang = 32768,
		Fortran_Lang = 65536,
		Groovy_Lang = 131072,
		JavaScript_Lang = 262144,
		Kotlin_Lang = 524288,
		Lua_Lang = 1048576,
		Lisp_Lang = 2097152,
		MATLAB_Lang = 4194304,
		Pascal_Lang = 8388608,
		Perl_Lang = 16777216,
		Prolog_Lang = 33554432,
		R_Lang = 67108864,
		Ruby_Lang = 134217728,
		Rust_Lang = 268435456,
		Scala_Lang = 536870912,
		Swift_Lang = 1073741824,
		TypeScript_Lang = 2147483648,
		VBScript_Lang = 4294967296,
		// Custom programming languages can be added here, but will only be supported by SQLiteClassBuilder by calling SetProgLangCommonSettings and SetProgLangSpecialSettings to provide the necessary settings for the custom programming language.
		// If the custom programming language is not similar to one of the supported programming languages, a SqlClassBuilder derived class should be created which overrides virtual functions to provide the necessary logic for the custom programming language.
		// There is bit space for an additional 30 custom programming languages. 
		// If more than 30 custom programming languages are needed, use ID value greater than 4294967296 and less than 9223372036854775808. And use CreateAllFiles which takes AllTheProgrammingLanguagesToProcess.
		// That logic allows for an additional 9,223,372,032,559,808,512 custom programming languages.
		Last_Lang = 9223372036854775808, // Largest single bit value for an std::uint64_t
	};

	enum DatabaseTyoe{
		SQLite_Database = 1, // Default: Only SQLite is supported at this time.
		MySQL_Database = 2,
		ODBC_Database = 4,
		PostgreSQL_Database = 8,
		SQLServer_Database = 16,
		Oracle_Database = 32,
	};

	struct CommonProgLangSettings
	{
		struct TypeOptions
		{
			std::string str_type;			// "std::string", "std::wstring", "sqlite3pp::tstring", "char*", "wchar_t*", "TCHAR*"
			std::string str_prefix;			//  "", "L", "_T("
			std::string str_suffix;			//  "", "", ")"
		};

		struct DestFileOptions // Destination File Options
		{
			std::string filename_prefix;	// Default: ""					Desired file name prefix. If empty, a prefix is created using prefix "SQL_" and the database file name, plus suffix "_". Any periods (.) in database file name are replaced with underscores (_).
											//								Example: If database name is NorthWind.DB, than the file name prefix is "SQL_NorthWind_DB_"
			std::string filename_suffix;	// Default: ""					Desired suffix for destination source file name.
			std::string file_type;			// Default: "hpp"				Other options (h, hxx, class)
			std::string dest_folder;		// Default: "SQL\\"				Destination folder where files are created. If drive letter is not included in the path, folder is treated as relative path.
		};

		struct MiscOptions
		{
			std::string SingleLineComment;				// Program language single line comment character(s). Example ("//", "#", "--")
			std::string SartOfMultiLineComment;			// Program language start of multiple line comments. Example ("/*")
			std::string EndOfMultiLineComment;			// Program language start of multiple line comments. Example ("*/")
			std::string ProgLangClassType;				// Program language class type. Example( "class", "struct")
			std::string ParrentClass;					// Example: ": public sqlite3pp::sql_base"
			std::string delimiter;						// Output delimiter:  ",", ", ", " ", ";", ""
			std::string GetColumnNameDelimiter;			// Delimiter used in getColumnNames function. Example: ", "
			std::string GetSelectColumnNamesDelimiter;	// Delimiter used in getSelectColumnNames function. Example: ", "
			std::string MemberVarProtectionType;		// "public", "protected", "private"
			bool exclude_get_functions = false;			// If true, no get function. If false, a get function is created for each data member variable.
			bool exclude_set_functions = false;			// If true, no set function. If false, a set function is created for each data member variable.
			bool include_comments = false;				// If true, includes comments and additional spaces.
			bool include_table_interface = false;		// If true, excludes sqlite3pp::Table interface functions ( getTableName, getColumnNames, and getStreamData), and excludes Miscellaneous function(s).
			bool exclude_main_hdr_example = false;		// If true, excludes example code added to SQL_MyDB_db__Master_Header
			bool initialize_member_variables = true;	// If true, initialize class member variables in header
			bool initialize_str_member_var = false;		// If true, initialize class member string variables in header
			bool use_basic_types_only = false;			// If true, only int, double, std::string, and std::wstring are used
			bool create_constructor = false;			//
		};

		TypeOptions m_TypeOptions;
		DestFileOptions m_DestFileOptions;
		MiscOptions m_MiscOptions;
		ProgLang m_ProgLang = ProgLang::CPP_Lang;
	};

	enum ChangeDetails
	{
		TableOrViewName = 0,
		WasColumnName = 1, // This bit is set if it was a column name, and not set if it was a table or view name
		InvalidChars = 2,
		ChangedFirstChar = 4,
		UppperCaseKeywordConflict = 8,
		LowerCaseKeywordConflict = 16,
		MixCaseKeywordConflict = 32,
		SpaceCharInName = 64,
		FixedKeywordConflictWithMakeLowerCase = 128, // Fixed keyword conflict by changing name to all lower case
		FixedKeywordConflictWithMakeUpperCase = 256, // Fixed keyword conflict by changing name to all upper case
		FixedKeywordConflictWithMakeMixCase = 512, // Fixed keyword conflict by changing name to mix case (first letter upper case, rest lower case)
		FixedKeywordConflictWithMakeRevMixCase = 1024, // Fixed keyword conflict by changing name to reverse mix case (first letter lower case, rest upper case)
		FixedKeywordConflictWithSpecialMixUpperCase = 2048, // Fixed keyword conflict by changing name to upper case, and then iterate through characters making the lower case and check for keyword conflicts.
		FixedKeywordConflictWithSpecialMixLowerCase = 4096, // Fixed keyword conflict by changing name to lower case, and then iterate through characters making the lower case and check for keyword conflicts.
		FixedKeywordConflictWithAppendUnderscore = 8192, // Fixed keyword conflict by appending multiple underscore charcters to name
		NeedToUseOriginalSqlNameInSqlQuery = 16384, // When this bit is set, it indicates that extra logic is needed to make sure original SQL name is used in SQL queries. 
		//	This bit is set when any of the following bits are set: FixedKeywordConflictWithAppendUnderscore, InvalidChars, ChangedFirstChar, and SpaceCharInName.
	};

	class SqlClassBuilderDbData; // Hide SQLite details so it's not exposed in the header file, and to make it easier to support other database types in the future.
	class NameChangeDetails; // Used to keep track of names which were changed due to conflicts with keywords or invalid characters. This information is mainly used for reporting, documenting, and  debugging purposes.

	class SqlClassBuilder
	{
	public:
		SqlClassBuilder(const std::string& DB_filename, DatabaseTyoe databaseTyoe = DatabaseTyoe::SQLite_Database); // Only SQLite is supported at this time.
		~SqlClassBuilder(); // Deletes m_pData, which closes the database connection.

		// The following function just calls the CreateAllFiles which takes std::uint64_t, and therefore is not virtual. Add any overriding logic in the CreateAllFiles which takes std::uint64_t.
		bool CreateAllFiles(ProgLang BitListOfTargetLanguages = ProgLang::ALL_Supported_Langs);

		virtual bool CreateAllFiles(std::uint64_t BitListOfTargetLanguages); // Using std::uint64_t type instead of ProgLang enum so-as to allow multiple languages to be specified, and to allow future expansion for languages not included in the enum
		virtual bool CreateAllFiles(std::set<std::uint64_t> AllTheProgrammingLanguagesToProcess); // Can be overridden for custom programming languages requirements.

		const CommonProgLangSettings& GetProgLangCommonSettings(ProgLang lang) { return m_ProgLangCommonSettings[lang]; }
		void SetProgLangCommonSettings(ProgLang lang, const CommonProgLangSettings& options) { m_ProgLangCommonSettings[lang] = options; }
		const std::map<std::string, std::string>& GetProgLangSpecialSettings(ProgLang lang) { return m_ProgLangSpecialSettings[lang]; };
		void SetProgLangSpecialSettings(ProgLang lang, const std::map<std::string, std::string>& settings) { m_ProgLangSpecialSettings[lang] = settings; }
		static const CommonProgLangSettings& GetAlternateProgLangSettings(ProgLang lang, const std::string& AlternateName) { return m_AlternateProgLangSettings[lang][AlternateName]; }
		static void AddOtherProgLangKeywordsMixCase(const std::string& keyword) { m_OtherProgLangKeywordsMixCase.insert(keyword); }
		static void AddOtherProgLangKeywordsLowerCase(const std::string& keyword) { m_OtherProgLangKeywordsLowerCase.insert(keyword); }
		static void AddOtherProgLangKeywordsUpperCase(const std::string& keyword) { m_OtherProgLangKeywordsUpperCase.insert(keyword); }
		static void ClearOtherProgLangKeywordsMixCase() { m_OtherProgLangKeywordsMixCase.clear(); }
		static void ClearOtherProgLangKeywordsLowerCase() { m_OtherProgLangKeywordsLowerCase.clear(); }
		static void ClearOtherProgLangKeywordsUpperCase() { m_OtherProgLangKeywordsUpperCase.clear(); }
		static void SetSpaceCharReplacementCharacter(char replacement) { m_SpaceCharReplacementCharacter = replacement; }
	protected:
		// Following functions return false if any failure occurs.
		virtual bool Initialize();	// Called by CreateAllFiles before creating any files. Can be overridden to perform any initialization needed before creating files for custom programming languages.
		virtual bool PopulateLangsToProcess(); // Called by Initialize, and can be overridden for custom programming languages requirements.
		virtual bool AddProgLangSettings(ProgLang progLang); // Called by PopulateLangsToProcess for each programming language in m_BitListOfLangsToProcess or in m_LangsToProcess. Can be overridden for custom programming languages requirements.
		virtual bool CreateFiles(); // Called by both CreateAllFiles, and does the real work in creating files. Can be overridden for custom programming languages requirements.
		virtual bool AddCommentsToFile(std::ofstream &myfile, ProgLang lang, const std::string& ClassName, const std::string& TableName);
		// Returns empty string upon failure.
		virtual std::string GetFileNameAndClassName(const std::string& TableName, std::string& FileNameOnly, std::string& ClassName, ProgLang lang); // Populates ClassName and FileNameOnly. Returns full FileName with directory path. Can be overridden for custom programming languages requirements.
		virtual std::string InitializeValue(std::string TypeName, ProgLang lang); // Determines if the programming language needs a specific initialization value for the specified type. Returns empty string if no initialization is needed. Can be overridden for custom programming languages requirements.
		virtual std::string MakeValidClassOrVarName(const std::string& name, bool ColumnName); // This calls the static MakeValidClassOrVarName, and virtual method can be overridden for custom programming languages requirements.

		//  Note:	When changing column or table names, try to provide as much information as possible in changeDetails about what was changed and why.
		//			This is so that it can be added to class comments for documentation purposes.
		//			It will let the developer know why the original name is not being used, and hopefully guide them to pick a better name if they want to change it.
		//			When possible, only the case of one or more characters are changed, so that it doesn't effect SQL queries, because SQL is case insensitive.	
		static std::string MakeValidClassOrVarName(const std::string& name, ChangeDetails& changeDetails); //
		static std::string ConvertToValidAlphaNum(std::string str, ChangeDetails& changeDetails);// Convert all characters to alpha-numeric, and make sure first character is a letter.

		std::map<std::string, NameChangeDetails> m_NotUsingOriginalName; // key = changed name, value = original name change reason.
		std::map<std::string, std::string> m_NeedToUseOriginalSqlNameInSqlQuery; // key = changed name, value = original name
	private:
		const std::string m_db_filename; // This is used to open the database, and it's used as part of class file default prefix name.
		DatabaseTyoe m_DatabaseTyoe; // Only SQLite is supported at this time.
		SqlClassBuilderDbData* m_pData;
		std::map<ProgLang, CommonProgLangSettings> m_ProgLangCommonSettings;
		std::map<ProgLang, std::map<std::string, std::string> > m_ProgLangSpecialSettings;
		std::set<std::uint64_t> m_LangsToProcess;
		std::uint64_t m_BitListOfLangsToProcess;
		std::map<std::string, std::string> m_TableNamesAndTypes; // m_TableNamesAndTypes = Key is table/view name, and value is type (table or view); Warning: Table and view names must be unique across both tables and views.
		std::map<std::string, std::map<std::string, std::string> > m_DataNamesAndTypes; // m_DataNamesAndTypes = First key is table/view name. Second key is member name, and value is member type (TEXT, CHAR, NCHAR, INTEGER, BOOLEAN, BLOB, etc...)
		static std::map<ProgLang, std::map<std::string, std::string> > m_DefaultProgLangSpecialSettings;
		static std::map<ProgLang, CommonProgLangSettings> m_DefaultProgLangSettings;
		static std::map<ProgLang, std::map<std::string, CommonProgLangSettings> > m_AlternateProgLangSettings;
		static const std::vector<std::string> m_SqliteKeywordsMixCase; // List of SQLite3.h and SQLite3pp.h keywords which should not be used as class or variable names
		static const std::vector<std::string> m_CppKeywordsLowerCase; // List of C++ keywords which should not be used as class or variable names
		static std::set<std::string> m_OtherProgLangKeywordsMixCase; // List of other programming languages mix case keywords which should not be used as class or variable names
		static std::set<std::string> m_OtherProgLangKeywordsLowerCase; // List of other programming languages lower case keywords which should not be used as class or variable names
		static std::set<std::string> m_OtherProgLangKeywordsUpperCase; // List of other programming languages upper case keywords which should not be used as class or variable names
		static char m_SpaceCharReplacementCharacter; // Character used to replace any space characters in the names. Default is underscore (_). Can be changed via SetSpaceCharReplacementCharacter.
	};
};

#endif //SQLCLASSBUILDER_H