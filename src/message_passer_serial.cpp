#ifndef PARFAIT_WITH_MPI
#include "message_passer.h"

//Send a single value
void MessagePasser::Send(double value,int destination){}
void MessagePasser::Send(float value,int destination){}
void MessagePasser::Send(int value,int destination){}
	
// recv a single value
void MessagePasser::Recv(double &value,int source){}
void MessagePasser::Recv(float &value,int source){}
void MessagePasser::Recv(int &value,int source){}


#endif
