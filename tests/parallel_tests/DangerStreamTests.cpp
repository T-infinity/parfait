#include <catch.hpp>
#include "MessagePasserDangerStream.h"

TEST_CASE("DangerStream POD") {
    MessagePasser::DangerStream stream;
    int b = 5;
    stream << b;
    int a;
    stream >> a;
    REQUIRE(a == 5);
    double c = 1.1;
    stream << c;
    double d;
    stream >> d;
}
TEST_CASE("DangerStream vector of POD"){

    MessagePasser::DangerStream stream;
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
    friend MessagePasser::DangerStream & operator<<(MessagePasser::DangerStream &stream, const MyClass& myClass);
    friend MessagePasser::DangerStream & operator>>(MessagePasser::DangerStream &stream, MyClass& myClass);

};

MessagePasser::DangerStream & operator<<(MessagePasser::DangerStream &stream, const MyClass& myClass){
    return stream << myClass.a << myClass.vec << myClass.d;
}

MessagePasser::DangerStream & operator>>(MessagePasser::DangerStream &stream, MyClass& myClass){
    return stream >> myClass.a >> myClass.vec >> myClass.d;
}

TEST_CASE("DangerStream user defined class"){

    MyClass sending(9, {1,2,3}, 5.5);
    MessagePasser::DangerStream stream;
    stream << sending;
    MyClass recving;
    stream >> recving;
    REQUIRE(recving.a == 9);
    REQUIRE((recving.vec == std::vector<int>{1,2,3}));
    REQUIRE(recving.d == 5.5);
}

TEST_CASE("DangerStream vector of user defined class"){
    std::vector<MyClass> myVector(3);
    for(int i = 0; i < 3; i++){
        myVector[i].a = i;
        myVector[i].vec = std::vector<int>{i,i+1,i+2};
        myVector[i].d = i*1.1;
    }

    MessagePasser::DangerStream stream;
    stream << myVector;
    std::vector<MyClass> myOutputVector;
    stream >> myOutputVector;
    REQUIRE(myOutputVector.size() == 3);
    for(int i = 0; i < 3; i++){
        REQUIRE(myOutputVector[i].a == i);
        REQUIRE((myOutputVector[i].vec == std::vector<int>{i,i+1,i+2}));
        REQUIRE(myVector[i].d == i*1.1);
    }
}

