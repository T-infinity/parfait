#pragma once

#include <queue>
#include <stddef.h>
#include <cstring>
#include <iostream>

namespace MessagePasser{

  struct DangerElement {
      size_t length;
      char* data;
      DangerElement(size_t length, char* data)
              :length(length), data(data){}
  };

  class DangerStream {
  private:
      std::queue<DangerElement> elements;
  public:
      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, DangerStream &>::type
      operator<<(const T& a){
          elements.push(DangerElement(sizeof(a), (char*)&a));
          return *this;
      }
      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, DangerStream &>::type
      operator>>(T& a){
          auto e = elements.front();
          elements.pop();
          a = *(T*)e.data;
          return *this;
      }

      template <typename T> typename std::enable_if<std::is_pod<T>::value, DangerStream &>::type
      operator<<(const std::vector<T>& vec){
          elements.push(DangerElement{(vec.size()*sizeof(T)), (char*)vec.data()});
          return *this;
      }
      template <typename T> typename std::enable_if<std::is_pod<T>::value, DangerStream &>::type
              operator>>(std::vector<T>& vec){
          auto e = elements.front();
          elements.pop();
          auto length = e.length / sizeof(T);
          vec.resize(length);
          std::memcpy(vec.data(), e.data, e.length);
          return *this;
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, DangerStream &>::type
      operator<<(const std::vector<T>& vec){
          elements.push(DangerElement{vec.size(), nullptr});
          for(auto &e :vec)
              *this << e;
          return *this;
      }
      template <typename T> typename std::enable_if<not std::is_pod<T>::value, DangerStream &>::type
      operator>>(std::vector<T>& vec){
          auto e = elements.front();
          elements.pop();
          vec.resize(e.length);
          for(auto & v : vec)
              *this >> v;
          return *this;
      }
  };
}