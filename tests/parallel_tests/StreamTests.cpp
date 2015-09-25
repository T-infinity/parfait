#include <catch.hpp>
#include "MessagePasserStream.h"

TEST_CASE("Stream POD") {
    MessagePasser::Stream stream;
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
TEST_CASE("Stream vector of POD"){

    MessagePasser::Stream stream;
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
    friend MessagePasser::Stream & operator<<(MessagePasser::Stream &stream, const MyClass& myClass);
    friend MessagePasser::Stream & operator>>(MessagePasser::Stream &stream, MyClass& myClass);

};

MessagePasser::Stream & operator<<(MessagePasser::Stream &stream, const MyClass& myClass){
    stream << myClass.a << myClass.vec << myClass.d;
}

MessagePasser::Stream & operator>>(MessagePasser::Stream &stream, MyClass& myClass){
    stream >> myClass.a >> myClass.vec >> myClass.d;
}

TEST_CASE("Stream user defined class"){

    MyClass sending(9, {1,2,3}, 5.5);
    MessagePasser::Stream stream;
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
    MessagePasser::Stream stream;
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
    MessagePasser::Stream s1;
    int a = 5;
    s1 << a;
    int b = 6;
    MessagePasser::Stream s2;
    s2 << b;
    s1 << s2;

    MessagePasser::Stream s3;
    s1 >> s3;
    int c;
    s3 >> c;
    REQUIRE(c == 5);
}

TEST_CASE("Chained Stream POD") {
    MessagePasser::Stream stream;
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

TEST_CASE("Chained Stream Dawg") {
    MessagePasser::Stream s1;

    int a = 5;
    int b = 6;
    int c = 7;

    s1 << a << b << c;

    MessagePasser::Stream s2;

    int d = 8;
    int e = 9;

    s2 << d << e << s1;

    std::vector<int> vec(5);

    for(auto &it : vec) s2 >> it;

    REQUIRE(vec == (std::vector<int> {8,9,5,6,7}));
}