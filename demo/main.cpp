#include <kv_storage.hpp>

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
        Storage::init(Storage::choose_sev_lvl(vm["log_lvl"].as<string>()));
      else
        Storage::init(default_sev_lvl);
      BOOST_LOG_TRIVIAL(info) << "Start program\n";
      if (vm.count("thread_count") && vm.count("output"))
        Storage::start(
            argv[count_options * 2 + 1],
            const_cast<int &>(vm["thread_count"].as<int>()),
            vm["output"].as<string>());
      else if (vm.count("thread_count") && !vm.count("output"))
        Storage::start(
            argv[count_options * 2 + 1],
            const_cast<int &>(vm["thread_count"].as<int>()),
            default_output_path);
      else if (!vm.count("thread_count") && vm.count("output"))
        Storage::start(
            argv[count_options * 2 + 1],
            default_num_threads,
            vm["output"].as<string>());
      else
        Storage::start(
            argv[count_options * 2 + 1],
            default_num_threads,
            default_output_path);
    } else
      throw po::error(error_mes);
  } catch (const po::error &ex)
  {
    BOOST_LOG_TRIVIAL(error) << ex.what() << '\n';
  }
}