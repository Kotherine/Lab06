// Copyright 2021 Your Name <your_email>

#include "hash.hpp"

// Логгирование в файл
void SetUpLogging() {
  // Событие с уровнем логирования - trace
  auto TraceFileLogger = boost::log::add_file_log(
      boost::log::keywords::file_name =
          "/home/kotherine/lab06_c++/Lab6/logs/"
          "TraceLog_%N.log",
      boost::log::keywords::rotation_size = 10 * 1024 * 1024,
      boost::log::keywords::format =
          "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");
  // Событие с уровнем логирования - info
  auto InfoFileLogger = boost::log::add_file_log(
      boost::log::keywords::file_name =
          "/home/kotherine/lab06_c++/Lab6/logs/"
          "TraceLog_%N.log",
      boost::log::keywords::format =
          "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");
  // Событие вывода в консоль
  auto consoleLogger = boost::log::add_console_log(
      std::cout, boost::log::keywords::format =
                     "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

  TraceFileLogger->set_filter(boost::log::trivial::severity >=
                              boost::log::trivial::trace);
  InfoFileLogger->set_filter(boost::log::trivial::severity ==
                             boost::log::trivial::info);
  consoleLogger->set_filter(boost::log::trivial::severity ==
                            boost::log::trivial::info);
}
void JsonFiler::NewElement(const std::string& data,
                           const std::string& hash,
                           std::time_t timestamp) {
  // Поток запрашивает монопольное использование данных, защищаемых мьютексом
  std::scoped_lock<std::mutex> lock(mut);
  // Hex-представление прообраза хеш-функции
  std::stringstream HexString;
  HexString << std::hex << std::uppercase << std::stoi(data);
  json JsonObj;
  JsonObj["data"] = HexString.str();
  JsonObj["hash"] = hash;
  JsonObj["timestamp"] = timestamp;
  JsonArray.push_back(JsonObj);
}
std::ostream& operator<<(std::ostream& out, const JsonFiler& j) {
  std::scoped_lock<std::mutex> lock(j.mut);
  out << j.JsonArray.dump(4);
  return out;
}

