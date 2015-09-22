#include <catch.hpp>
#include <MessagePasserStream.h>

TEST_CASE("Stream POD") {
    MessagePasser::Stream stream;
    stream << 5;
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
    friend MessagePasser::Stream& operator<<(MessagePasser::Stream &stream, const MyClass& myClass);
    friend MessagePasser::Stream& operator>>(MessagePasser::Stream &stream, MyClass& myClass);

};

MessagePasser::Stream& operator<<(MessagePasser::Stream &stream, const MyClass& myClass){
    stream << myClass.a;
    stream << myClass.vec;
    stream << myClass.d;
}

MessagePasser::Stream& operator>>(MessagePasser::Stream &stream, MyClass& myClass){
    stream >> myClass.a;
    stream >> myClass.vec;
    stream >> myClass.d;
}

TEST_CASE("Stream user defined class"){

    MyClass sending(9, {1,2,3}, 5.5);
    MessagePasser::Stream stream;
    stream << sending;
    MyClass recving;
    stream >> recving;
    REQUIRE(recving.a == 9);
    REQUIRE((recving.vec == std::vector<int>{1,2,3}));
    REQUIRE(recving.d == 5.5);
}

//TEST_CASE("Stream vector of user defined class"){
//    std::vector<MyClass> myVector(3);
//    for(int i = 0; i < 3; i++){
//        myVector[i].a = i;
//        myVector[i].vec = std::vector<int>{i,i+1,i+2};
//        myVector[i].d = i*1.1;
//    }
//
//    MessagePasser::Stream stream;
//    stream << myVector;
//}

