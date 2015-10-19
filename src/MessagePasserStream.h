#pragma once

#include <list>
#include <stddef.h>
#include <cstring>
#include <iostream>

namespace MessagePasser{

  struct Element {
      size_t length;
      std::vector<char> data_copy;
      Element(size_t n) :length(n){}
      Element(size_t length, char* data)
              :length(length),
               data_copy(data,data+length)
      {
      }
  };

  class Stream {
  private:
      std::list<Element> elements;
  public:

      Stream& operator<<(Stream &s){
          for(auto &e : s.elements)
              elements.emplace_back(e);
          return *this;
      }

      Stream& operator>>(Stream &s){
          s << *this;
          return *this;
      }

      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, Stream &>::type
      operator<<(const T& a){
          elements.push_back(Element(sizeof(a), (char*)&a));
          return *this;
      }
      template <typename T>
      typename std::enable_if<std::is_pod<T>::value, Stream &>::type
      operator>>(T& a){
          auto e = elements.front();
          elements.pop_front();
          a = *(T*)e.data_copy.data();
          return *this;
      }

      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream &>::type
      operator<<(const std::vector<T>& vec){
          elements.push_back(Element{(vec.size()*sizeof(T)), (char*)vec.data()});
          return *this;
      }
      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream &>::type
              operator>>(std::vector<T>& vec){
          auto e = elements.front();
          elements.pop_front();
          auto length = e.length / sizeof(T);
          vec.resize(length);
          std::memcpy(vec.data(), e.data_copy.data(), e.length);
          return *this;
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream &>::type
      operator<<(const std::vector<T>& vec){
          elements.push_back(Element{vec.size()});
          for(auto &e :vec)
              *this << e;
          return *this;
      }
      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream &>::type
      operator>>(std::vector<T>& vec){
          auto e = elements.front();
          elements.pop_front();
          vec.resize(e.length);
          for(auto & v : vec)
              *this >> v;
          return *this;
      }
      std::list<Element> getElements();
  };

  inline std::list<Element> Stream::getElements() {
      return elements;
  }
}