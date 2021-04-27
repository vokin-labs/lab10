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
#include "../third-party/PicoSHA2/picosha2.h"

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

class Storage{
 public:
  static void create_program_options(po::options_description& desc,
                                     po::variables_map& vm,
                                     const int& argc, const char *argv[]);

  static void start(const string& path_to_db);

  static void check_status(const rocksdb::Status& status,
                           const string& message);

  static string hash256(const string& key, const string& value);

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

  static void init(const boost::log::trivial::severity_level& sev_lvl);

  static boost::log::trivial::severity_level chose_sev_lvl(
      const string& sev_lvl_str);

 private:
};

#endif // INCLUDE_HEADER_HPP_
