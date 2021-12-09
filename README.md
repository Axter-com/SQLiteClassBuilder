# SQLiteClassBuilder
A command line tool for creating type safe SQL classes. The tool uses the database schema to create a class for each table and view. It has many command line options to allow developers to customize the class creation.
Each class is created in it's own header (*.h).  A master header (sql_Master_Header.h) is created which includes all the headers.

## Usage Example:
````
SQLiteClassBuilder.exe "my_database.db"
````

## SQLite3pp_EZ containers
By default, classes are created so they're compatible with sqlite3pp::Table and sqlite3pp::TableOStream container classes.

See https://github.com/David-Maisonave/sqlite3pp_EZ

These container classes can be populated using a single line of code: ``Table<sql_table_FileExt> tbl(DbFileNameArg("mydatabase.db"));``

In this example, **sql_table_FileExt** is a class created by SQLiteClassBuilder, and "mydatabase.db" is the file name of the SQLite database.

Example#1
````
sqlite3pp::Table<sql_table_FileExt> tbl(DbFileNameArg("mydatabase.db"));
for (auto row : my_tbl)
  std::wcout << row << std::endl;

````


Example#2
````
sqlite3pp::TableOStream<sql_table_FileExt> tbl(DbFileNameArg("mydatabase.db"));
std::cout << tbl; // Send data to screen with the changed delimiter

````


Example#3
````
sqlite3pp::TableOStream<sql_table_FileExt> tbl(DbFileNameArg("mydatabase.db"));
std::ofstream ofs ("data.csv", std::ofstream::out); // Open a file
ofs << tbl; // Write data to the csv file

````

