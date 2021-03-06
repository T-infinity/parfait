
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
#include <catch.hpp>
#include <map>
#include "../MessagePasserStream.h"
#include "parfait/Extent.h"

TEST_CASE("Stream POD") {
    Stream::Stream stream;
    int b = 5;
    stream << b;
    b = 7; // invalidate b to prove that the data was copied
    int a;
    stream >> a;
    REQUIRE(a == 5);
    double c = 1.1;
    stream << c;
    double d;
    stream >> d;
}

TEST_CASE("Empty stream"){
    Stream::Stream stream;
    REQUIRE(stream.empty());
    int a;
    REQUIRE_THROWS_AS(stream >> a, std::logic_error);
}

TEST_CASE("Stream an Element"){
    Stream::Stream stream;
    double x = 3.5;
    Stream::Element element((char*)&x,sizeof(x));
    stream << element;
    x = 0;
    Stream::Element element2(sizeof(x));
    stream >> element2;
    memcpy(&x,element2.data(),sizeof(x));
    REQUIRE(3.5 == x);
}

TEST_CASE("Stream an extent"){
    Stream::Stream stream;
    Parfait::Extent<double> extent {{0,0,0},{1,1,1}};
    stream << extent;
    extent.lo = {9,9,9};
    extent.hi = {9,9,9};
    stream >> extent;
    REQUIRE(0 == extent.lo[0]);
    REQUIRE(1 == extent.hi[0]);
}

TEST_CASE("Stream vector of POD"){

    Stream::Stream stream;
    std::vector<int> input = {1,2,3};
    std::vector<int> output;
    stream << input;
    input.assign(3,9);
    stream >> output;
    REQUIRE(( output == std::vector<int>{1,2,3} ));
}

class MyClass {
public:
    MyClass() = default;
    MyClass(int a, const std::vector<int>& vec, double d)
            : a(a), vec(vec), d(d){}
    int a;
    std::vector<int> vec;
    double d;
    friend Stream::Stream & operator<<(Stream::Stream &stream, const MyClass& myClass);
    friend Stream::Stream & operator>>(Stream::Stream &stream, MyClass& myClass);
};

Stream::Stream & operator<<(Stream::Stream &stream, const MyClass& myClass){
    return stream << myClass.a << myClass.vec << myClass.d;
}

Stream::Stream & operator>>(Stream::Stream &stream, MyClass& myClass){
    return stream >> myClass.a >> myClass.vec >> myClass.d;
}

TEST_CASE("Stream user defined class"){

    MyClass sending(9, {1,2,3}, 5.5);
    Stream::Stream stream;
    stream << sending;
    MyClass recving;
    sending.a = 9001;
    sending.vec[0] = 9001;
    stream >> recving;
    REQUIRE(recving.a == 9);
    REQUIRE((recving.vec == std::vector<int>{1,2,3}));
    REQUIRE(recving.d == 5.5);
}

TEST_CASE("Stream vector of user defined class"){
    std::vector<MyClass> myVector(3);
    for(int i = 0; i < 3; i++){
        myVector[i].a = i;
        myVector[i].vec = std::vector<int>{i,i+1,i+2};
        myVector[i].d = i*1.1;
    }
    Stream::Stream stream;
    stream << myVector;
    std::vector<MyClass> myOutputVector;
    myVector[0].vec[0] = 9001;
    stream >> myOutputVector;
    REQUIRE(myOutputVector.size() == 3);
    for(int i = 0; i < 3; i++){
        REQUIRE(myOutputVector[i].a == i);
        REQUIRE((myOutputVector[i].vec == std::vector<int>{i,i+1,i+2}));
        REQUIRE(myVector[i].d == i*1.1);
    }
}

TEST_CASE("Stream your stream"){
    Stream::Stream s1;
    int a = 5;
    s1 << a;
    int b = 6;
    Stream::Stream s2;
    s2 << b;
    s1 << s2;

    Stream::Stream s3;
    s1 >> s3;
    int c;
    s3 >> c;
    REQUIRE(c == 5);
}

TEST_CASE("Chained Stream POD") {
    Stream::Stream stream;
    int a = 5;
    int b = 6;
    int c = 7;

    stream << a << b << c;

    a = b = c = 9;

    int d;
    int e;
    int f;

    stream >> d >> e >> f;

    REQUIRE(d == 5);
    REQUIRE(e == 6);
    REQUIRE(f == 7);
}

TEST_CASE("Chained Stream") {
    Stream::Stream s1;

    int a = 5;
    int b = 6;
    int c = 7;

    s1 << a << b << c;

    Stream::Stream s2;

    int d = 8;
    int e = 9;

    s2 << d << e << s1;

    std::vector<int> vec(5);

    for(auto &it : vec) s2 >> it;

    REQUIRE(vec == (std::vector<int> {8,9,5,6,7}));
}

TEST_CASE("Get elements of a stream for communications"){
    Stream::Stream s1;

    int a = 5;
    int b = 6;
    int c = 7;
    s1 << a << b << c;

    auto elements = s1.getElements();
    REQUIRE(elements.size() == 3);
}

class Header {
public:
    int source;
    int destination;
    int ticket;
};

TEST_CASE("Push non-pod front") {
    Stream::Stream s;
    Header h;
    h.source = 1;
    h.destination = 2;
    h.ticket = 3;

    s.push_front(h);
    auto elements = s.getElements();
    REQUIRE(elements.size() == 1);
    Header h2;
    s >> h2;
    REQUIRE(h2.source == 1);
    REQUIRE(h2.destination == 2);
    REQUIRE(h2.ticket == 3);
    elements = s.getElements();
    REQUIRE(elements.size() == 0);

    MyClass myclass(1, {2,3,4}, 5.0);
    s.push_front(myclass);
    REQUIRE(s.getElements().size() == 3);
    MyClass myClass2;
    s >> myClass2;
    REQUIRE(myClass2.a == 1);
    REQUIRE((myClass2.vec == std::vector<int>{2,3,4}));
    REQUIRE(myClass2.d == 5.0);
}

TEST_CASE("Push Front"){
    Stream::Stream s;

    int a = 5;
    int b = 6;
    int c = 7;
    s << a << b << c;


    Header h;
    h.source = 1;
    h.destination = 2;
    h.ticket = 3;

    s.push_front(h);
    Header h_out;
    s >> h_out;
    REQUIRE(h_out.source == 1);
    REQUIRE(h_out.destination == 2);
    REQUIRE(h_out.ticket == 3);

    int aa;
    s >> aa;
    REQUIRE(aa == 5);
    s.push_front(aa);

    int aaa;
    s >> aaa;
    REQUIRE(aaa == 5);

    int bb;
    s >> bb;
    REQUIRE(bb == 6);
    int cc;
    s >> cc;
    REQUIRE(cc == 7);
}