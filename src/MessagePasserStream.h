#ifndef PARFAIT_MESSAGEPASSERSTREAM_H
#define PARFAIT_MESSAGEPASSERSTREAM_H

#include <queue>
#include <stddef.h>
#include <cstring>
#include <iostream>

namespace MessagePasser{

  struct Element {
      size_t length;
      char* data;
      Element(size_t length, char* data)
              :length(length), data(data){}
  };

  class Stream{
  private:
      std::queue<Element> elements;
  public:
      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, Stream&>::type
      operator<<(const T& a){
          elements.push(Element(sizeof(a), (char*)&a));
          return *this;
      }
      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, Stream&>::type
      operator>>(T& a){
          auto e = elements.front();
          elements.pop();
          a = *(T*)e.data;
          return *this;
      }

      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream&>::type
      operator<<(const std::vector<T>& vec){
          elements.push(Element{(vec.size()*sizeof(T)), (char*)vec.data()});
          return *this;
      }
      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream&>::type
              operator>>(std::vector<T>& vec){
          auto e = elements.front();
          elements.pop();
          auto length = e.length / sizeof(T);
          vec.resize(length);
          std::memcpy(vec.data(), e.data, e.length);
          return *this;
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream&>::type
      operator<<(const std::vector<T>& vec){
          elements.push(Element{vec.size(), nullptr});
          for(auto &e :vec)
              *this << e;
          return *this;
      }
      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream&>::type
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

#endif 
