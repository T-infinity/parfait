#pragma once

#include <list>
#include <stddef.h>
#include <cstring>
#include <iostream>
#include <vector>

namespace MessagePasser{

  struct Element {
      size_t length;
      std::vector<char> data_copy;
      Element(size_t n) :length(n), data_copy(length){}
      Element(char* data, size_t length)
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
          elements.push_back(Element{(char*)&a, sizeof(a)});
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
          elements.push_back(Element{(char*)vec.data(), (vec.size()*sizeof(T))});
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

      template <typename T> typename std::enable_if<std::is_pod<T>::value, void>::type
      push_front(const T& a) {
          elements.push_front(Element{(char*)&a, sizeof(a)});
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, void>::type
      push_front(const T& a) {
          printf("\npushing non-pod object to the front"); fflush(stdout);
          Stream s;
          s << a;
          for(auto it = s.elements.rbegin(); it != s.elements.rend(); it++ ) {
              auto e = *it;
              elements.push_front(e);
          }
      }

      inline size_t size() const {
          return elements.size();
      }
      inline std::list<Element>& getElements(){
          return elements;
      }
      inline void push_element(const Element& e){
          elements.push_back(e);
      }
  };
}
