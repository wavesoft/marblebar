# The MarbleBar GUI
A minimal footprint Web GUI library for C++11 with a tiny embedded web browser. I wrote this library because I wanted a cross-platform GUI without the need to drag along massive SDKs.

The concept is very simple: The library includes a tiny webserver, it's static resources to serve and the core logic required in order to crate simple views. It comes complete, with bootstrap for nice GUIs (offline of course), and a modular appropach on writing custom widgets.

## Example

Until I come with a complete documentation, have a look on this example:

```cpp

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

    // You can access the underlaying property like any other
    // propetty of the same type
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
```

## License

MarbleBar is licensed under GNU GPL Version 2.0, Open-Source license.

```
libMarbleBar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libMarbleBar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libMarbleBar. If not, see <http://www.gnu.org/licenses/>.

Developed by Ioannis Charalampidis 2015
Contact: <ioannis.charalampidis[at]cern.ch>
```
