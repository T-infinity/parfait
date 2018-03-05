
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once

#include <list>
#include <stddef.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <map>
#include <set>

namespace Stream{

    struct Element {
        Element(size_t n) :length(n), data_copy(length){}
        Element(char* data, size_t length)
            :length(length),
             data_copy(data,data+length)
        { }
        size_t size(){return length;}
        char* data() {return data_copy.data();}
    private:
        size_t length;
        std::vector<char> data_copy;
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
          throwIfEmpty();
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
          throwIfEmpty();
          auto e = elements.front();
          elements.pop_front();
          a = *(T*)e.data();
          return *this;
      }

      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream &>::type
      operator<<(const std::vector<T>& vec){
          elements.push_back(Element{(char*)vec.data(), (vec.size()*sizeof(T))});
          return *this;
      }
      template <typename T> typename std::enable_if<std::is_pod<T>::value, Stream &>::type
              operator>>(std::vector<T>& vec){
          throwIfEmpty();
          auto e = elements.front();
          elements.pop_front();
          auto length = e.size() / sizeof(T);
          vec.resize(length);
          std::memcpy(vec.data(), e.data(), e.size());
          return *this;
      }
      template <typename T, typename U>
      Stream & operator<<(const std::pair<T, U>& p){
          return *this << p.first << p.second;
      }
      template <typename T, typename U>
      Stream & operator>>(std::pair<T, U>& p){
          return *this >> p.first >> p.second;
      }
      Stream& operator<<(const Element& e){
          elements.push_back(e);
          return *this;
      }

      Stream& operator>>(Element& element){
          throwIfEmpty();
          auto e = elements.front();
          elements.pop_front();
          element = e;
          return *this;
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream &>::type
      operator<<(const std::vector<T>& vec) {
          elements.push_back(Element{vec.size()});
          for (auto& e :vec)
              *this << e;
          return *this;
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, Stream &>::type
      operator>>(std::vector<T>& vec){
          throwIfEmpty();
          auto e = elements.front();
          elements.pop_front();
          vec.resize(e.size());
          for(auto & v : vec)
              *this >> v;
          return *this;
      }
      template <typename T>
      Stream& operator<<(const std::set<T>& s){
          size_t length = s.size();
          *this << length;
          for(const auto& p : s){
              *this << p;
          }
          return *this;
      }
      template <typename T>
      Stream& operator>>(std::set<T>& s){
          size_t length;
          *this >> length;
          for(size_t i = 0; i < length; i++){
              T p;
              *this >> p;
              s.insert(p);
          }
          return *this;
      }
      template <typename T, typename U>
      Stream& operator<<(const std::map<T, U> &m){
          size_t length = m.size();
          *this << length;
          for(auto& pair : m){
              *this << pair;
          }
          return *this;
      }
      template <typename T, typename U>
      Stream& operator>>(std::map<T, U> &m){
          size_t length;
          *this >> length;
          for(size_t i = 0; i < length; i++){
              std::pair<T, U> p;
              *this >> p;
              m.insert(p);
          }
          return *this;
      }

      template <typename T> typename std::enable_if<std::is_pod<T>::value, void>::type
      push_front(const T& a) {
          elements.push_front(Element{(char*)&a, sizeof(a)});
      }

      template <typename T> typename std::enable_if<not std::is_pod<T>::value, void>::type
      push_front(const T& a) {
          Stream s;
          s << a;
          for(auto it = s.elements.rbegin(); it != s.elements.rend(); it++ ) {
              auto e = *it;
              elements.push_front(e);
          }
      }

      inline bool empty(){ return 0 == elements.size(); }

      inline size_t size() const { return elements.size(); }

      inline std::list<Element>& getElements(){ return elements; }

      inline void push_element(const Element& e){ elements.push_back(e); }

      inline void throwIfEmpty(){
          if(empty())
            throw std::logic_error("Tried to >> out of an empty stream");
      }
  };
}
