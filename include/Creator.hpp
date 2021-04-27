//
// Created by aleksandr on 26.04.2021.
//

#ifndef STORAGE_CREATOR_HPP
#define STORAGE_CREATOR_HPP
#include <string>
#include <iomanip>
#include <iostream>
#include <rocksdb/db.h>
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using rocksdb::ColumnFamilyDescriptor;
using rocksdb::ColumnFamilyOptions;
using rocksdb::ColumnFamilyHandle;

class Creator {
 public:
  ~Creator();

  void create_new_random_db(const std::string& path_to_new_db);

  inline int get_column_families_names_size();

  inline int get_values_size();

  inline void set_values(const std::string& value);
 private:
  rocksdb::DB* db_ptr;
  std::vector<std::string> column_families_names;
  std::vector<ColumnFamilyHandle*> handles;
  std::vector<std::string> values;
};
#endif  // STORAGE_CREATOR_HPP
