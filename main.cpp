#include <iostream>
#include "Pathfinder.h"

using namespace std;

int main()
{
auto *pathfin = new Pathfinder();
std::cout<<"Your path:"<<endl;
pathfin->getCurrentPath();
std::cout<<pathfin->resultPath<<endl;
pathfin->referenceCountSum();
std::cout<<endl<<"reference counter:"<<pathfin->referenceCounter<<endl;
return 0;
}
