//
// Created by cdruyorj on 8/18/15.
//
#include <string>
#include "PreProcessor.h"

#include "CppUTest/CommandLineTestRunner.h"

using std::string;

TEST_GROUP(PreProcessorTests)
{

};

TEST(PreProcessorTests,Exists)
{
    Parfait::PreProcessor pre_processor("sample.xml");

}
