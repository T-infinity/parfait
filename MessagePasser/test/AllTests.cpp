#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../MessagePasser.h"
int main( int argc, char* argv[] )
{
    MessagePasser mp;
    mp.Init();
    int result = Catch::Session().run( argc, argv );
    mp.Finalize();
    return ( result < 0xff ? result : 0xff );
}

