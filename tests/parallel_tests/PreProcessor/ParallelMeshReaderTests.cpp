//
// Created by cdruyorj on 8/18/15.
//
#include <string>
#include "PreProcessor.h"
#include "ParallelMeshReader.h"

#include "CppUTest/CommandLineTestRunner.h"

using std::string;

TEST_GROUP(ParallelMeshReaderTests)
{ };

TEST(ParallelMeshReaderTests,Exists)
{
    auto mesh = Parfait::ParallelMeshReader::read("sample.xml");
}

