/*
	GNU General Public License

	Copyright (C) 2021 David Maisonave (www.axter.com)
	The SQLiteClassBuilder source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "sqlite3pp_ez.h"
#include "cxxopts.hpp"
#include "CommonFunctions.h"

#include <iostream>
#include <string>
#include <direct.h>
#include <sys/stat.h>


bool FileExists(const std::string &path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

const std::string EarlyExitErrMsg = "\
\
**********************************************\
Performing early exit due to error!\
**********************************************\
\
";


#define V_COUT(VB, V)		{if (sqlite3pp::sql_base::GetVerbosityLevel() >= sqlite3pp::VBLV_##VB) {std::cout << V << std::endl;} }

int main(int argc, char* argv[])
{
	cxxopts::Options options("SQLiteClassBuilder.exe", "SQLiteClassBuilder: Creates type safe SQL classes.\nBy David Maisonave (www.axter.com)");
	options
		//.positional_help("[optional args]")
		.allow_unrecognised_options()
		.show_positional_help();

	options
		.set_width(80)
		.set_tab_expansion()
		.add_options()
		// Common
		("d,database", "File name of the SQLite database. Required argument!! Example: SQLiteClassBuilder -d\"C:\\Data\\myDatabase.db\"", cxxopts::value<std::string>()->default_value(""))
		("w,where", "Optional: And-Where-Clause. Can be used to specify a set of tables/views to process.\nExample1:\nSQLiteClassBuilder -d\"my.db\" -w\"AND tbl_name like 'Personnel%'\"\nExample2:\nSQLiteClassBuilder -d\"my.db\" -w\"AND tbl_name NOT like 'zzTest%'\"", cxxopts::value<std::string>()->default_value(""))
		("v,verbosity", "Verbosity level. Default: 3.  0=No-output; 1=Error-Output-Only; 2=Err+Warn; 3=Err+Wrn+Info; 4=Err+Wrn+Info+Debug; 5=Err+Wrn+Info+Debug+Details", cxxopts::value<int>()->default_value("3"))
		("p,pause", "Pause before program exit. Note: If verbosity level 0, program will exit without pause unless error occurs.", cxxopts::value<bool>()->default_value("true"))
		("h,help", "Print usage")
		// ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false")) // ToDo: Delete this argument if it doesn't get used.
		// ("c,clipboard", "Populate bla bla from clipboard", cxxopts::value<bool>()->default_value("false"), "bool")
		;

	options.add_options("String Type")
		//StrOptions
		("t,strtype", "TEXT default type. Can be (string, wstring, tstring, or tstring_t). Default: string (aka std::string).", cxxopts::value<std::string>()->default_value(""))
		;
	options.add_options("Miscellaneous")
		// MiscOptions
		("u,public", "Make data members public. Default: Data members are protected.", cxxopts::value<bool>()->default_value("false"))
		("g,xgetfnc", "Exclude class get functions. Default: class get_* method is created for each data member.", cxxopts::value<bool>()->default_value("false"))
		("s,xsetfnc", "Exclude class set functions. Default: class set_* method is created for each data member.", cxxopts::value<bool>()->default_value("false"))
		("o,xostream", "Exclude operator<< function. Default: An ostream operator<< function is created for each class.", cxxopts::value<bool>()->default_value("false"))
		("e,delimit", "Only used with opereator<<, and can be a desired string value to mark seperation between field output. ie:  \", \", \", \", \" \", \"; \", \"\".", cxxopts::value<std::string>()->default_value(","))
		("c,xcmmnt", "Exclude comments. Default: Usage comments are created for each class.", cxxopts::value<bool>()->default_value("false"))
		("x,xinterf", "Exclude SQLite3pp::Table interface. Default: A set of methods (getTableName, getColumnNames, getSelecColumnNames, and getStreamData) are created for each class in order to interface with SQLite3pp::Table.", cxxopts::value<bool>()->default_value("false"))
		("b,basic_t", "Only use C++ basic types (int, double, std::string, and std::wstring). Default: Use SQLite2 and SQLite3 sub types (Text, Integer, Blob, Clob, Date, Datetime, Boolean, Bigint, Varchar, Nchr, etc...).", cxxopts::value<bool>()->default_value("false"))
		("m,xvalid", "Exclude test validation examples in main header. Default: Test validation examples are created in main header.", cxxopts::value<bool>()->default_value("false"))
		("n,cvalid", "Comment out example in main header. Default: Validation examples in main header are commented out.", cxxopts::value<bool>()->default_value("true"))
		;
	options.add_options("Header")
		//HeaderOpt
		("f,folder", "Distination folder. Path where headers are created.", cxxopts::value<std::string>()->default_value("SQL"))
		("a,prefix", "Specify a desired prefix for created headers. Default: \"sql_\".", cxxopts::value<std::string>()->default_value("sql_"))
		("z,postfix", "Optionally specify a desired postfix for created headers. Default: empty.", cxxopts::value<std::string>()->default_value(""))
		("y,fileext", "File type to create (h, hpp, class, hxx). Note: If empty, default type is used. Default: \"h\".", cxxopts::value<std::string>()->default_value("h"))
		("i,include", "#include to add to all headers. Note: If empty, no additional include is added. Default: \"sqlite3pp_ez.h\".", cxxopts::value<std::string>()->default_value("sqlite3pp_ez.h"))
		("r,rmdir", "Remove destination directory before creating headers", cxxopts::value<bool>()->default_value("false"))
		("q,dhead", "Delete all sql_*.h files before creating headers.", cxxopts::value<bool>()->default_value("true"))
		;
	auto arg_result = options.parse(argc, argv);
	const int VerbosityLevel = arg_result["verbosity"].as<int>();
	sqlite3pp::sql_base::SetVerbosityLevel(static_cast<sqlite3pp::VerbosityLevels>(VerbosityLevel));

	std::string db_file_name = arg_result["database"].as<std::string>();
	if (db_file_name.size())
		V_COUT(DETAIL, "Using argument -d for DB file name. " << db_file_name)
	else if (argc > 1)
	{
		if (argv[1][0] != '-') // Check if it's the first argument
		{
			db_file_name = argv[1];
			V_COUT(DEBUG, "Using first argument for DB file name. " << db_file_name);
		}
		else if (argc > 3 && argv[argc-2][0] != '-') // Check if it's the last argument
		{
			db_file_name = argv[argc-1];
			V_COUT(DEBUG, "Using last argument for DB file name. " << db_file_name);
		}
	}

	if (arg_result.count("help") || !db_file_name.size())
	{
		std::cout << options.help().c_str() << std::endl << std::endl;
		system("pause");
		exit(0);
	}

	if (!FileExists(db_file_name))
	{
		V_COUT(ERROR, "Error: Could not find database file '" << db_file_name << "'." << std::endl);
		V_COUT(ERROR, EarlyExitErrMsg);
		exit(-1);
	}

	std::string TargetPath = arg_result["folder"].as<std::string>();
	TargetPath = Common::rtrim(TargetPath, "\\");
	if (TargetPath.size())
	{
		V_COUT(DETAIL, "Using destination path '" << TargetPath << "'.");
		if (arg_result["rmdir"].as<bool>())
		{
			if (sqlite3pp::SQLiteClassBuilder::dir_exists(TargetPath.c_str()))
			{
				V_COUT(INFO, "Removing directory '" << TargetPath << "' before creating headers.");
				_rmdir(TargetPath.c_str());
			}
		}

		if (!sqlite3pp::SQLiteClassBuilder::dir_exists(TargetPath.c_str()))
		{
			V_COUT(INFO, "Creating target path '" << TargetPath << "'.");
			_mkdir(TargetPath.c_str());
		}
	}

	std::string FileExt = arg_result["fileext"].as<std::string>().size() ? arg_result["fileext"].as<std::string>() : "h";
	V_COUT(DETAIL, "Files created will use file extension '" << FileExt << "'.");
	
	if (arg_result["dhead"].as<bool>())
	{
		V_COUT(INFO, "Deleting exsiting headers.");
		std::string Command = "del /F /Q " + TargetPath + "\\" + arg_result["prefix"].as<std::string>() + "*" + arg_result["postfix"].as<std::string>() + "." + FileExt;
		V_COUT(DEBUG, "Deleting exsiting headers. by using command:" << Command);
		system(Command.c_str()); // ToDo: Create a cleaner method for this.
	}

	//StrOptions
	sqlite3pp::StrOptions	my_StrOptions	= sqlite3pp::SqlBld::strOpt_std_string;
	if (arg_result["strtype"].as<std::string>() == "wstring" || arg_result["strtype"].as<std::string>() == "std::wstring")
	{
		my_StrOptions = sqlite3pp::SqlBld::strOpt_std_wstring;
		V_COUT(DETAIL, "Using string type." << my_StrOptions.str_type);
	}
	else if (arg_result["strtype"].as<std::string>() == "tstring")
	{
		my_StrOptions = sqlite3pp::SqlBld::strOpt_sql_tstring;
		V_COUT(DETAIL, "Using string type." << my_StrOptions.str_type);
	}
	else if (arg_result["strtype"].as<std::string>() == "tstring_t")
	{
		my_StrOptions = sqlite3pp::SqlBld::strOpt_sql_tstring_T;
		V_COUT(DETAIL, "Using string type." << my_StrOptions.str_type);
	}
	else
	{
		V_COUT(DETAIL, "Using default string type std::string.");
	}

	// MiscOptions
	sqlite3pp::MiscOptions	my_MiscOptions	= sqlite3pp::SqlBld::MiscOpt_max;
	my_MiscOptions.delimiter = arg_result["delimit"].as<std::string>();
	my_MiscOptions.is_public_var_members = arg_result["public"].as<bool>();
	my_MiscOptions.exclude_get_functions = arg_result["xgetfnc"].as<bool>();
	my_MiscOptions.exclude_set_functions = arg_result["xsetfnc"].as<bool>();
	my_MiscOptions.exclude_ostream_operator = arg_result["xostream"].as<bool>();
	my_MiscOptions.exclude_comments = arg_result["xcmmnt"].as<bool>();
	my_MiscOptions.exclude_table_interface = arg_result["xinterf"].as<bool>();
	my_MiscOptions.use_basic_types_only = arg_result["basic_t"].as<bool>();
	my_MiscOptions.exclude_main_hdr_example = arg_result["xvalid"].as<bool>();
	my_MiscOptions.exclude_comment_out_exampl = !arg_result["cvalid"].as<bool>();

	//HeaderOpt
	sqlite3pp::HeaderOpt	my_HeaderOpt	= sqlite3pp::SqlBld::HeaderDefaultOpt;
	my_HeaderOpt.dest_folder = TargetPath + "\\";
	my_HeaderOpt.file_type = FileExt;
	my_HeaderOpt.header_postfix = arg_result["postfix"].as<std::string>();
	my_HeaderOpt.header_prefix = arg_result["prefix"].as<std::string>();
	my_HeaderOpt.header_include = arg_result["include"].as<std::string>();

	sqlite3pp::SQLiteClassBuilder
		createsqlitetableclass(
			db_file_name
			, my_StrOptions
			, arg_result["where"].as<std::string>()
			, my_MiscOptions
			, my_HeaderOpt
		);

	V_COUT(INFO, "\n\n***************************************\nProcess Complete.\nCreated " << createsqlitetableclass.GetHeadersCreated().size() << " classes and headers.\n***************************************\n");
	if (arg_result["pause"].as<bool>() && VerbosityLevel)
		system("pause");

	return 0;
}
