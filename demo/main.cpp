// Copyright 2021 Your Name <your_email>

#include "picosha2.h"
#include "hash.hpp"
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <thread>

// Флаги
std::atomic<bool> ContinueProcess = true;
std::atomic<bool> AddToJson = false;

// Сигнал - нажатие Ctri+C
void StopProcess(int param) {
  if (param == SIGINT) {
    ContinueProcess = false;
  }
}

// Генератор хэш-функции + уровень логгирования
void HashGenerator(JsonFiler& j) {
  while (ContinueProcess) {
    std::string genstring = std::to_string(std::rand());
    // Генерация шестнадцатеричной строки хэша SHA256 из std::string
    std::string hash = picosha2::hash256_hex_string(genstring);
    // Фиксируем время
    std::time_t timestamp(std::time(nullptr));
    // Возвращаем последние 4 символа хэша
    std::string lastChar = hash.substr(hash.size() - N);

    // Проверяем, это хэш с 4-мя нулями в конце?
    if (lastChar == ending) {
      // Уровень логирования для данного события info - нашли нужный хэш
      BOOST_LOG_TRIVIAL(info)
          << "Necessary ending found in hash |" << hash
          << "| generarted from string |" << genstring << "|";
      // Создаём элемент json-файла (сохраним хэш-функцию)
      if (AddToJson) {
        j.NewElement(genstring, hash, timestamp);
      }
    } else {
      // Уровень логирования для данного события trace - хэш не подходит
      BOOST_LOG_TRIVIAL(trace)
          << "Hash |" << hash << "| generated from string|"
          << genstring << "|";
    }
  }
}
void Process(const int& argc, char **argv){
  // Количество потоков
  unsigned int numberOfThreads;
  std::string json_path;
  std::srand(time(nullptr));
  switch (argc) {
    case 1:
      numberOfThreads = std::thread::hardware_concurrency();
      break;
    case 2:
      numberOfThreads = std::atoi(argv[1]);
      if (numberOfThreads == 0 ||
          numberOfThreads > std::thread::hardware_concurrency()) {
        throw std::out_of_range(" Invalid number of threads!!!");
      }
      break;
    case 3:
      numberOfThreads = std::atoi(argv[1]);
      if (numberOfThreads == 0 ||
          numberOfThreads > std::thread::hardware_concurrency()) {
        throw std::out_of_range(" Invalid number of threads!!!");
      }
      json_path = argv[2];
      AddToJson = true;
      break;
    default:
      throw std::out_of_range("Invalid number of arguments!!!");
  }
  SetUpLogging();
  // Ведение журнала логгирования
  boost::log::add_common_attributes();
  // Инициализация потоков
  std::vector<std::thread> threads;
  // Json-объект
  JsonFiler json_obj;
  threads.reserve(numberOfThreads);
  // Обработка Ctrl+C
  std::signal(SIGINT, StopProcess);

  for (size_t i = 0; i < numberOfThreads; i++) {
    // Передаём в потоки хэши
    threads.emplace_back(HashGenerator, std::ref(json_obj));
  }
  for (auto& thread : threads) {
    // Ожидаем завершения каждого потока
    thread.join();
  }
  // Добавляем найденные хэши в json-файл
  if (AddToJson) {
    std::ofstream fout{json_path};
    fout << json_obj;
  }
}

int main(int argc, char* argv[]) {
  Process(argc,argv);
  return 0;
}
