// Copyright 2020 MIX-1 danilonil1@yandex.ru

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
