#include <kv_storage.hpp>

//default_value("../cmake-build-debug/out.txt")
//default_value(std::thread::hardware_concurrency())
//cout << argv[count_options*2 + 1] << endl;

//      string temp;
//      status = db->Get(rocksdb::ReadOptions(), handles_cf[1], "key1", &temp);
//      Storage::check_status(status, "BD::Get fail ");
//      cout << temp << endl;

int main(int argc, const char *argv[])
{
  try
  {
    po::options_description desc{"Options"};
    po::variables_map vm;
    Storage::create_program_options(desc, vm, argc, argv);

    size_t count_options = vm.count("log_lvl") + vm.count("thread_count")
                           + vm.count("output");
    if (vm.count("help"))
      cout << note << usage << desc;
    else if (argc > 1){
      if (vm.count("log_lvl"))
        Storage::init(Storage::chose_sev_lvl(vm["log_lvl"].as<string>()));
      Storage::start(argv[count_options * 2 + 1]);
      BOOST_LOG_TRIVIAL(trace) << "first step\n";
    } else
      throw po::error("**********BAD SYNTAX**********\n"
                      "Look to --help or -h");
  } catch (const po::error &ex)
  {
    std::cerr << ex.what() << '\n';
  }
}