// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_HASH_HPP_
#define INCLUDE_HASH_HPP_

#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

    using json = nlohmann::json;

const char ending[] = "0000"; // Окончание хэша
const size_t N = 4; // Количество нулей в конце
void SetUpLogging(); // Логгирование

class JsonFiler {
 public:
  // Создание нового элемента файла
  void NewElement(const std::string& data, const std::string& hash,
                  std::time_t timestamp);
  // Ввод элемента файла
  friend std::ostream& operator<<(std::ostream& out, const JsonFiler& j);

 private:
  // mutable показывает, что член класса является изменяемым,
  // и его можно изменять в функциях, у которых указан модификатор const,
  // а также у константных объектов.

  // mutex - это базовый механизм синхронизации, предназначенный для
  // организации взаимоисключающего доступа к общим данным для
  // нескольких потоков с использованием барьеров памяти
  mutable std::mutex mut; // Обеспечиваем потокобезопасность данных
  json JsonArray; // json-массив
};

#endif // INCLUDE_HASH_HPP_
