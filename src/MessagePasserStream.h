#ifndef PARFAIT_MESSAGEPASSERSTREAM_H
#define PARFAIT_MESSAGEPASSERSTREAM_H

#include <queue>
#include <stddef.h>
#include <cstring>

namespace MessagePasser{

  struct Element {
      size_t length;
      char* data;
      Element(size_t length, char* data)
              :length(length), data(data){}
  };

  class Stream{
  public:
      template <typename T> Stream& operator<<(const T& a);
      template <typename T> Stream& operator<<(const std::vector<T>& vec);

      template <typename T> Stream& operator>>(T& a);
      template <typename T> Stream& operator>>(std::vector<T>& vec);
  private:
      std::queue<Element> elements;
  };

  template <typename T>
  Stream& Stream::operator<<(const T& a){
      static_assert(std::is_pod<T>::value, "T must be pod");
      elements.push(Element(sizeof(a), (char*)&a));
      return *this;
  }
  template <typename T>
  Stream& Stream::operator>>(T& a){
      static_assert(std::is_pod<T>::value, "T must be pod");
      auto e = elements.front();
      elements.pop();
      a = *(T*)e.data;
      return *this;
  }
  template <typename T>
  Stream &Stream::operator<<(const std::vector<T> &vec) {
      static_assert(std::is_pod<T>::value, "T must be pod");
      elements.push(Element{(vec.size()*sizeof(T)), (char*)vec.data()});
      return *this;
  }
  template <typename T>
  Stream &Stream::operator>>(std::vector<T> &vec) {
      auto e = elements.front();
      elements.pop();
      auto length = e.length / sizeof(T);
      vec.resize(length);
      std::memcpy(vec.data(), e.data, e.length);
      return *this;
  }
}

#endif 
