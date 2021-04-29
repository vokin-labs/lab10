//
// Created by aleksandr on 28.04.2021.
//

#include "Writer.h"

Writer::Writer(const int& num_workers) : writers(num_workers) {}

std::string Writer::hash256(std::string& key, std::string& value) {
  return writers.enqueue([&]
                         (std::string& key_, std::string& value_){
    return picosha2::hash256_hex_string(key_ + value_);
  }, key, value).get();
}
