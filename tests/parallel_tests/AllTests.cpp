#include <checkpoint.h>
#include "MessagePasser.h"
#include "CppUTest/CommandLineTestRunner.h"

using namespace MessagePasser;

int main(int argc,char **argv)
{
	CHECKPOINT
	Init();
	CHECKPOINT
	auto returnValue = CommandLineTestRunner::RunAllTests(argc,argv);
	Finalize();
	return returnValue;
}
