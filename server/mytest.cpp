#include <iostream>
#include <string>
#include "../include/C_R.h"
using namespace std;
int main()
{
	string a="$~fdsfafddsf~$";
	C_R c;
std::cout<<c.detect_STX(a);
std::cout<<c.detect_DEX(a);
	c.remove_header_ender(a);
	
	std::cout<<a;
return 0;
}

$~fdsfafddsf~$