#include "Global.h"

unsigned int getState()
{
	return (int) leftFrontL.currentColor*10000 + leftFrontM.currentColor*1000 + middleFron.currentColor*100 + rightFrontM.currentColor*10 + rightFrontR.currentColor;
}

void lineTrace()
{

}
