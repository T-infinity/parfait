#include "message_passer.h"
#include "CppUTest/CommandLineTestRunner.h"

using namespace MessagePasser;

int main(int argc,char **argv)
{
	Init();
	auto returnValue = CommandLineTestRunner::RunAllTests(argc,argv);
	Finalize();
	return returnValue;
}
