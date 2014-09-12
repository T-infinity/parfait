#include "CppUTest/CommandLineTestRunner.h"
#include "message_passer.h"

int main(int argc,char **argv)
{
	MessagePasser::Init();
	auto results = CommandLineTestRunner::RunAllTests(argc,argv);
	MessagePasser::Finalize();
	return results;
}
