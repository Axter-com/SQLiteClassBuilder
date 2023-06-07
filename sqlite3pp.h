// sqlite3pp.h
//
// The MIT License
//
// Copyright (c) 2015 Wongoo Lee (iwongu at gmail dot com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// David Maisonave -- Nov-2021 Added UNICODE API's


#ifndef SQLITE3PP_H
#define SQLITE3PP_H

#define SQLITE3PP_VERSION "1.0.8"
#define SQLITE3PP_VERSION_MAJOR 1
#define SQLITE3PP_VERSION_MINOR 0
#define SQLITE3PP_VERSION_PATCH 8

#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <ctime>
#include <memory>

#include "sqlite3.h"

namespace sqlite3pp
{
#if defined(SQLITE3PP_NO_UNICODE) || !defined(_UNICODE)
	using tstring = std::string;
#else
	using tstring = std::wstring;
#endif // SQLITE3PP_NO_UNICODE || !_UNICODE

#ifndef SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY
	// SQLite3 types (Excluding string types)
	using Blob = std::shared_ptr<std::vector<unsigned char> >;// Stores binary data
	using Clob = std::shared_ptr< std::vector<char> >;	// Stores strings that can have multiple NULL terminators
	struct Date	{std::time_t t;};
	struct Datetime	{std::tm tm_struct;};
	using TEXT = tstring;
#endif //!SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY	

  class database;

  namespace ext
  {
	class function;
	class aggregate;
	database borrow(sqlite3* pdb);
  }

  template <class T>
  struct convert {
	using to_int = int;
  };

  class null_type {};
  extern null_type ignore;

  class noncopyable
  {
   protected:
	noncopyable() = default;
	~noncopyable() = default;

	noncopyable(noncopyable&&) = default;
	noncopyable& operator=(noncopyable&&) = default;

	noncopyable(noncopyable const&) = delete;
	noncopyable& operator=(noncopyable const&) = delete;
  };

  //struct sqlite3_api_routines;
  class db_api_root
  {
  public:
	  db_api_root();
  };

  class database : public db_api_root, private noncopyable
  {
	friend class statement;
	friend class database_error;
	friend class ext::function;
	friend class ext::aggregate;
	friend database ext::borrow(sqlite3* pdb);

   public:
	using busy_handler = std::function<int (int)>;
	using commit_handler = std::function<int ()>;
	using rollback_handler = std::function<void ()>;
	using update_handler = std::function<void (int, char const*, char const*, long long int)>;
	using authorize_handler = std::function<int (int, char const*, char const*, char const*, char const*)>;
	using backup_handler = std::function<void (int, int, int)>;

	explicit database( char const* dbname = nullptr, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, const char* vfs = nullptr );

	database(database&& db);
	database& operator=(database&& db);

	~database();
	
#ifndef SQLITE3PP_NO_UNICODE	
	// Unicode support
	explicit database( const  wchar_t* dbname, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, const wchar_t* vfs = nullptr );
	int connect( const  wchar_t* dbname, int flags, const wchar_t* vfs = nullptr );
	int execute( const std::wstring& sql );
	int execute( const std::string& sql );
	int attach(const  wchar_t* dbname, const  wchar_t* name);
	int detach(const  wchar_t* name);
	int backup(const  wchar_t* dbname, database& destdb, const  wchar_t* destdbname, backup_handler h, int step_page = 5);
#endif //!SQLITE3PP_NO_UNICODE

	int connect( char const* dbname, int flags, const char* vfs = nullptr );
	int disconnect();

	int attach(char const* dbname, char const* name);
	int detach(char const* name);

	int backup(database& destdb, backup_handler h = {});
	int backup(char const* dbname, database& destdb, char const* destdbname, backup_handler h, int step_page = 5);

	long long int last_insert_rowid() const;

	int enable_foreign_keys(bool enable = true);
	int enable_triggers(bool enable = true);
	int enable_extended_result_codes(bool enable = true);

	int changes() const;

	int error_code() const;
	int extended_error_code() const;
	char const* error_msg() const;

	int execute( char const* sql );
	int executef(char const* sql, ...);

	int set_busy_timeout(int ms);

	void set_busy_handler(busy_handler h);
	void set_commit_handler(commit_handler h);
	void set_rollback_handler(rollback_handler h);
	void set_update_handler(update_handler h);
	void set_authorize_handler(authorize_handler h);

   private:
	database(sqlite3* pdb);

   private:
	sqlite3* db_;
	bool borrowing_;

	busy_handler bh_;
	commit_handler ch_;
	rollback_handler rh_;
	update_handler uh_;
	authorize_handler ah_;
  };

  class database_error : public std::runtime_error
  {
   public:
	explicit database_error(char const* msg);
	explicit database_error(const std::string& msg);
	explicit database_error(database& db);
  };

  enum copy_semantic { copy, nocopy };

  class statement : public db_api_root, private noncopyable
  {
   public:
	int prepare(char const* stmt);
	int finish();

	int bind(int idx, int value);
	int bind(int idx, double value);
	int bind(int idx, long long int value);
	int bind(int idx, char const* value, copy_semantic fcopy);
	int bind(int idx, void const* value, int n, copy_semantic fcopy);
	int bind(int idx, std::string const& value, copy_semantic fcopy);
	int bind(int idx);
	int bind(int idx, null_type);

	int bind(char const* name, int value);
	int bind(char const* name, double value);
	int bind(char const* name, long long int value);
	int bind(char const* name, char const* value, copy_semantic fcopy);
	int bind(char const* name, void const* value, int n, copy_semantic fcopy);
	int bind(char const* name, std::string const& value, copy_semantic fcopy);
	int bind(char const* name);
	int bind(char const* name, null_type);

	int step();
	int reset();

   protected:
	explicit statement(database& db, char const* stmt = nullptr);
	~statement();

	int prepare_impl(char const* stmt);
	int finish_impl(sqlite3_stmt* stmt);

   protected:
	database& db_;
	sqlite3_stmt* stmt_;
	char const* tail_;
#ifndef SQLITE3PP_NO_UNICODE
	explicit statement( database& db, wchar_t const* stmt = nullptr );
	int prepare_impl( wchar_t const* stmt);
   public:
	int prepare( wchar_t const* stmt);
#endif // !SQLITE3PP_NO_UNICODE
  };

  class command : public statement
  {
   public:
	class bindstream
	{
	 public:
	  bindstream(command& cmd, int idx);

	  template <class T>
	  bindstream& operator << (T value) {
		auto rc = cmd_.bind(idx_, value);
		if (rc != SQLITE_OK) {
		  throw database_error(cmd_.db_);
		}
		++idx_;
		return *this;
	  }
	  bindstream& operator << (char const* value) {
		auto rc = cmd_.bind(idx_, value, copy);
		if (rc != SQLITE_OK) {
		  throw database_error(cmd_.db_);
		}
		++idx_;
		return *this;
	  }
	  bindstream& operator << (std::string const& value) {
		auto rc = cmd_.bind(idx_, value, copy);
		if (rc != SQLITE_OK) {
		  throw database_error(cmd_.db_);
		}
		++idx_;
		return *this;
	  }

	 private:
	  command& cmd_;
	  int idx_;
	};

	explicit command(database& db, char const* stmt = nullptr);

	bindstream binder(int idx = 1);

	int execute();
	int execute_all();
  };

  class query : public statement
  {
   public:
	class rows
	{
	 public:
	  class getstream
	  {
	   public:
		getstream(rows* rws, int idx);

		template <class T>
		getstream& operator >> (T& value) {
		  value = rws_->get(idx_, T());
		  ++idx_;
		  return *this;
		}

	   private:
		rows* rws_;
		int idx_;
	  };

	  explicit rows(sqlite3_stmt* stmt);

	  int data_count() const;
	  int column_type(int idx) const;

	  int column_bytes(int idx) const;

	  template <class T> T get(int idx) const {
		return get(idx, T());
	  }

	  template <class... Ts>
	  std::tuple<Ts...> get_columns(typename convert<Ts>::to_int... idxs) const {
		return std::make_tuple(get(idxs, Ts())...);
	  }

	  getstream getter(int idx = 0);

	 private:
	  int get(int idx, int) const;
	  double get(int idx, double) const;
	  long long int get(int idx, long long int) const;
	  char const* get(int idx, char const*) const;

	  //////////////////////////////////////////////////////////////////////////////////
	  // Start SQLite3pp_EZ changes
	  // To minimize changes in original SQLite3pp.cpp code, the implementation for this section is in SQLite3pp_EZ.cpp
#ifndef SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY
#ifndef SQLITE3PP_NO_UNICODE
	  wchar_t const* get(int idx, wchar_t const*) const;
	  std::wstring get(int idx, const std::wstring&value) const;
#endif// !SQLITE3PP_NO_UNICODE
	  unsigned char get(int idx, const unsigned char&) const;
	  short int get(int idx, const short int&) const;
	  bool get(int idx, const bool&) const;
	  unsigned long long int get(int idx, const unsigned long long int&) const;
	  Blob get(int idx, const Blob&) const;
	  Clob get(int idx, const Clob&) const;
	  Date get(int idx, const Date&) const;
	  Datetime get(int idx, const Datetime&) const;
#endif// !SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY
	  // End SQLite3pp_EZ changes
	  //////////////////////////////////////////////////////////////////////////////////
	  std::string get(int idx, std::string) const;
	  void const* get(int idx, void const*) const;
	  null_type get(int idx, null_type) const;

	 private:
	  sqlite3_stmt* stmt_;
	};

	class query_iterator
	  : public std::iterator<std::input_iterator_tag, rows>
	{
	 public:
	  query_iterator();
	  explicit query_iterator(query* cmd);

	  bool operator==(query_iterator const&) const;
	  bool operator!=(query_iterator const&) const;

	  query_iterator& operator++();

//#pragma warning(disable : 4996)
	  query::rows operator*() const;

	 private:
	  query* cmd_;
	  int rc_;
	};

	explicit query( database& db, char const* stmt = nullptr );
#ifndef SQLITE3PP_NO_UNICODE
	explicit query( database& db, wchar_t const* stmt );
#endif //!SQLITE3PP_NO_UNICODE
	int column_count() const;

	char const* column_name(int idx) const;
	char const* column_decltype(int idx) const;

	using iterator = query_iterator;

	iterator begin();
	iterator end();
  };

  class transaction : public db_api_root, private noncopyable
  {
   public:
	explicit transaction(database& db, bool fcommit = false, bool freserve = false);
	~transaction();

	int commit();
	int rollback();

   private:
	database* db_;
	bool fcommit_;
  };

} // namespace sqlite3pp

#endif
