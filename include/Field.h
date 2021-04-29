//
// Created by aleksandr on 28.04.2021.
//

#ifndef STORAGE_FIELD_H
#define STORAGE_FIELD_H
#include <string>
#include <utility>
using string = std::string;
struct Field{
  Field(string  value_, string  key_) : value(std::move(value_)),
                                      key(std::move(key_)){}
  string value;
  string key;
};
#endif  // STORAGE_FIELD_H
