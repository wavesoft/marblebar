
#include <marblebar.hpp>
#include <iostream>

using namespace mb;
using namespace std;

int main(int argc, char ** argv) {

	KernelPtr kernel = createKernel( defaultConfig() );

	ViewPtr view = kernel->createView( "Primary" );
	PStringPtr p1 = view->addProperty( make_shared<PString>("First Value", "value") );
	PStringPtr p2 = view->addProperty( make_shared<PString>("Second Value", "value") );
	PBoolPtr p3 = view->addProperty( make_shared<PBool>("Toggler", false) );
	PIntPtr p4 = view->addProperty( make_shared<PInt>("Range", 0) );


	*p1 = "test";
	*p1 += "ing";

	string test = *p1;
	cout << "Res = " << test;
	cout << endl;

	kernel->openGUI();
	kernel->start();

	return 0;
};
