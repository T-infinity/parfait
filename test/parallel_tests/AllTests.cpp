#include "CppUTest/CommandLineTestRunner.h"
#include "message_passer.h"

using namespace MessagePasser;

int main(int argc,char **argv)
{
	Init();
	auto returnValue = CommandLineTestRunner::RunAllTests(argc,argv);
	Finalize();
	return returnValue;
}
