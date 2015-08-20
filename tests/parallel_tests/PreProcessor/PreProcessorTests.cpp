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
    auto mesh = pre_processor.createFun3DMesh();
}

TEST(PreProcessorTests,ThrowsIfCantOpenFile){
    CHECK_THROWS(std::domain_error,PreProcessor("non_existant.xml"));
}
