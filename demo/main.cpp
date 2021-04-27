#include <kv_storage.hpp>

//default_value("../cmake-build-debug/out.txt")
//default_value(std::thread::hardware_concurrency())
//cout << argv[count_options*2 + 1] << endl;

int main(int argc, const char *argv[])
{
  try
  {
    po::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen\n")

        ("log_lvl,l", po::value<string>(), "Logger severity\n")

        ("thread_count,t", po::value<int>(), "Count worker-thread\n")

        ("output,o", po::value<string>(),"Path to output file");

    po::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    string note = "NOTE:\n  If the database is created ignore this message, "
        "but if the database is not created, use:\n"
                  "  ./create <path_to_directory>\n\n";
    string usage = "Usage:\n  ./demo [options] <sourse_path>\n\n";

    size_t count_options = vm.count("log_lvl") +
                           vm.count("thread_count") +
                           vm.count("output");
    if (vm.count("help"))
      cout << note << usage << desc;
    else if (count_options){
      rocksdb::DB* db;
      std::vector<rocksdb::ColumnFamilyHandle*> handles;
      rocksdb::WriteOptions write_options;
      write_options.sync = true;
      rocksdb::Options options;
      options.create_if_missing = true;
      rocksdb::Status status = rocksdb::DB::Open(options, "creating_db", &db);
      assert(status.ok());
      if(!status.ok())
        std::cerr << "Status1: " << status.ToString() << endl;
      string temp;
      db->Get(rocksdb::ReadOptions(), handles[0], "key", &temp);
      cout << temp << endl;
      status = db->Close();
      delete db;
      cout << "Status2: " << status.ToString() << endl;
    } else if(argc == 2){
      cout << argv[1] << endl;
    } else
      throw po::error("**********BAD SYNTAX**********\n"
                      "Look to --help or -h");
  } catch (const po::error &ex)
  {
    std::cerr << ex.what() << '\n';
  }
}