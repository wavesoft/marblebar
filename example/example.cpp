#include <marblebar.hpp>
#include <iostream>

using namespace mb;
using namespace std;

int main(int argc, char ** argv) {

    // Create a MarbleBar Kernel
    KernelPtr kernel = createKernel( defaultConfig() );

    // Create our first view, titled 'Primary'
    ViewPtr view = kernel->createView( "Primary" );

    // Create a couple of properties
    PStringPtr p1 = view->addProperty( make_shared<PString>("Iterations", "value") );
    PStringPtr p2 = view->addProperty( make_shared<PString>("Second Value", "value") );
    PBoolPtr p3 = view->addProperty( make_shared<PBool>("Toggler", false) );
    PIntPtr p4 = view->addProperty( make_shared<PInt>("Range", 0) );
    PImagePtr p5 = view->addProperty( make_shared<PImage>("Preview", 128, -1, "about:blank") );

    // Most of the properies offer overloads that behave like
    // a regular underlaying variable (ex. int, bool, string).
    //
    // Changes to these properties are instantly reflected in the UI
    // and likewise, changes in the UI instantly affect the value of
    // the properties.
    //
    *p1 = "test";
    *p1 += "ing";

    // Ask kernel to open the GUI (open system's web browser)
    kernel->openGUI();

    // Start kerne's invinite loop
    while (true) {

        // .. Do your single-threaded work here ..

        // Just remember to let kernel process it's I/O
        kernel->poll();
    }

    // Optionally you might wish to start the kernel thread
    // in a different thread, and to run it's start() method
    kernel->start();

    return 0;
};