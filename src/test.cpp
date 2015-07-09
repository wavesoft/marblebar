
#include <marblebar.hpp>

int main(int argc, char ** argv) {

	mb::KernelPtr kernel = mb::createKernel( mb::defaultConfig() );

	kernel->openGUI();
	kernel->start();

	return 0;
};
