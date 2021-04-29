// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>
#include <thread>
#include <rocksdb/db.h>
#include "Reader.h"
#include "Writer.h"
#include <queue>
#include "Field.h"
#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace po = boost::program_options;
using string = std::string;
using std::cout;
using std::endl;

const string note = "NOTE:\n  If the database is created ignore this message, "
              "but if the database is not created, use:\n"
              "  ./create <path_to_directory>\n\n";
const string usage = "Usage:\n  ./demo [options] <source_path>\n\n";
const string error_mes = "**********BAD SYNTAX**********\n"
                     "Look to --help or -h";
const string default_output_path = "output_db";
const int default_num_threads =
    static_cast<int>(std::thread::hardware_concurrency());
const boost::log::trivial::severity_level default_sev_lvl =
    boost::log::trivial::severity_level::debug;
const int spases_for_table[4] = {76, 74, 42, 33};

class Storage{
 public:
  explicit Storage(int& num_workers);

  static void create_program_options(po::options_description& desc,
                                     po::variables_map& vm,
                                     const int& argc, const char *argv[]);

  static void start(const string& path_to_db,
                    int num_workers, const string& path_to_out_db);

  static void check_status(const rocksdb::Status& status,
                           const string& message);

  static void create_descriptors(
      const std::vector<string>& names_cf,
      std::vector<rocksdb::ColumnFamilyDescriptor>& descriptors_cf);

  static void create_iterators(
      rocksdb::DB* db,
      const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf,
      std::vector<rocksdb::Iterator*>& iterators_cf);

  static void delete_db(
      rocksdb::DB* db,
      const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf,
      const std::vector<rocksdb::Iterator*>& iterators_cf);

  static void delete_db(
      rocksdb::DB* db,
      const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf);

  static void check_iterators(
      const std::vector<rocksdb::Iterator*>& iterators_cf,
      const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf,
      std::queue<Field>& fields, std::vector<int>& nums_in_columns);

  static std::stringstream print_table(
      const std::vector<Field>& out_fields,
      const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf_out,
      const std::vector<int>& nums_in_columns);

  static void hashing(Storage* storage, std::queue<Field>& fields,
                      std::queue<Field>& hashs, std::vector<Field>& out_fields);

  static void init(const boost::log::trivial::severity_level& sev_lvl);

  static boost::log::trivial::severity_level choose_sev_lvl(
      const string& sev_lvl_str);

  static void str_of_separator(std::stringstream& ss);

  Writer& use_writer();

  Reader& use_reader();
 private:
  Writer writer;
  Reader reader;
};
#endif // INCLUDE_HEADER_HPP_
