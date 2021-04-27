// Copyright 2020 Your Name <your_email>

#include <kv_storage.hpp>

void Storage::check_status(const rocksdb::Status& status,
                           const string& message) {
  if(!status.ok())
    throw std::runtime_error(message + status.ToString());
}
void Storage::delete_db(rocksdb::DB* db,
    const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf,
    const std::vector<rocksdb::Iterator*>& iterators_cf) {
  rocksdb::Status status;
  for (auto& iterator : iterators_cf) {
    delete iterator;
  }
  for (auto& handle : handles_cf) {
    status = db->DestroyColumnFamilyHandle(handle);
    Storage::check_status(status, "BD::Destroy_cf fail fail ");
  }
  status = db->Close();
  Storage::check_status(status, "BD::Close fail fail ");
  delete db;
}
void Storage::create_iterators(
    rocksdb::DB* db,
    const std::vector<rocksdb::ColumnFamilyHandle*>& handles_cf,
    std::vector<rocksdb::Iterator*>& iterators_cf) {
  iterators_cf.reserve(handles_cf.size());
  for(auto & handle : handles_cf){
    iterators_cf.emplace_back(db->NewIterator(rocksdb::ReadOptions(),
                                              handle));
  }
}
void Storage::create_descriptors(
    const std::vector<string>& names_cf,
    std::vector<rocksdb::ColumnFamilyDescriptor>& descriptors_cf) {
  descriptors_cf.reserve(names_cf.size());
  for (auto& name : names_cf) {
    descriptors_cf.emplace_back(name, rocksdb::ColumnFamilyOptions());
  }
}
void Storage::create_program_options(po::options_description& desc,
                                     po::variables_map& vm, const int& argc,
                                     const char** argv) {
  desc.add_options()
      ("help,h", "Help screen\n")

      ("log_lvl,l", po::value<string>(), "Logger severity\n")

      ("thread_count,t", po::value<int>(), "Count worker-thread\n")

      ("output,o", po::value<string>(),"Path to output file");
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
}
void Storage::start(const string& path_to_db) {
  std::vector<string> names_cf;
  rocksdb::Status status = rocksdb::DB::ListColumnFamilies(
      rocksdb::DBOptions(),
      path_to_db,
      &names_cf);
  Storage::check_status(status, "BD::List_cf fail ");

  std::vector<rocksdb::ColumnFamilyDescriptor> descriptors_cf;
  Storage::create_descriptors(names_cf, descriptors_cf);

  rocksdb::DB* db;
  std::vector<rocksdb::ColumnFamilyHandle*> handles_cf;
  status = rocksdb::DB::Open(rocksdb::DBOptions(), path_to_db,
                             descriptors_cf, &handles_cf, &db);
  Storage::check_status(status, "BD::Open fail ");

  std::vector<rocksdb::Iterator*> iterators_cf;
  Storage::create_iterators(db, handles_cf, iterators_cf);

  std::vector<int> nums_in_columns;
  for(size_t i = 1; i < iterators_cf.size(); ++i){
    for(iterators_cf[i]->SeekToFirst(); iterators_cf[i]->Valid(); iterators_cf[i]->Next()){
      cout << iterators_cf[i]->key().ToString() << " : " << iterators_cf[i]->value().ToString() << endl;
    }
  }

  Storage::delete_db(db, handles_cf, iterators_cf);
}

string Storage::hash256(const string& key, const string& value) {
  return picosha2::hash256_hex_string(key + value);
}

void Storage::init(const boost::log::trivial::severity_level& sev_lvl) {
  boost::log::add_common_attributes();

  boost::log::core::get()->set_filter(boost::log::trivial::severity >= sev_lvl);

  boost::log::add_console_log(std::clog, boost::log::keywords::format = "[%Severity%] %TimeStamp%: %Message%");

  boost::log::add_file_log
      (
          boost::log::keywords::file_name = "sample_%N.log",
          boost::log::keywords::rotation_size = 30 * 1024 * 1024,
          boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
          boost::log::keywords::format = "[%Severity%][%TimeStamp%]: %Message%"
      );
}
boost::log::trivial::severity_level Storage::chose_sev_lvl(
    const string& sev_lvl_str) {
  if (sev_lvl_str == "trace")
    return boost::log::trivial::severity_level::trace;
  else if (sev_lvl_str == "debug")
    return boost::log::trivial::severity_level::debug;
  else if (sev_lvl_str == "info")
    return boost::log::trivial::severity_level::info;
  else if (sev_lvl_str == "warning")
    return boost::log::trivial::severity_level::warning;
  else
    return boost::log::trivial::severity_level::error;
}
