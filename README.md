# The MarbleBar GUI

Ever wanted a simple, quick, cross-platform GUI with no additional dependencies for your C++11 application? Was that UI mainly for providing some configuration or to see some results in real-time? Then say hello to MarbleBar!

MarbleBar is a minimal footprint Web GUI library for C++11 with a tiny embedded web browser. I wrote this library because I wanted a cross-platform GUI without the need to drag along massive SDKs.

The concept is very simple: The library includes a tiny web server, it's static resources to serve and the core logic required in order to crate simple views. It comes complete, with bootstrap for nice GUIs (offline of course), and a modular appropach on writing custom widgets.

Don't expect to see something like (Wt)[http://www.webtoolkit.eu/]. There are no sessions, no complex widgets nor advanced lay-outing. There is only one session: your actual application.  

## Building

MarbleBar is statically built along with your project. If you are using `CMake`, you can integrate it in your project like this:

```cmake
# Before you define your target:
add_subdirectory( "/path/to/marblebar/sources" marblebar )
include_directories( ${MarbleBar_INCLUDE_DIRS} )

# After you have defined your target:
target_link_libraries( ${PROJECT_NAME} ${MarbleBar_LIBS} )
```

If you are not using CMake, well... currently you are on your own. (But definitely, have a look at CMake, it could make our life way easier!)

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
```

And here is an example `CMakeLists.txt` file for building that sample project:

```cmake
cmake_minimum_required (VERSION 2.8)
project ( marblebar-example )
# Include MarbleBar
add_subdirectory( "/path/to/marblebar/sources" marblebar )
include_directories( ${MarbleBar_INCLUDE_DIRS} )
# Compile Project (With C++11)
add_definitions(-std=c++11)
add_executable( ${PROJECT_NAME} example.cpp )
# Link MarbleBar
target_link_libraries( ${PROJECT_NAME} ${MarbleBar_LIBS} )
```

## Quick Terminology Intro

From the C++ point of view, you are operating on view one or more `Property` objects in a `View`. This property is rendered in the javascript interface using a corresponding `Widget`. You can specify the widget in the property's specifications description. 

Have a look on the `PString::getUISpecs` method:

```cpp
Json::Value PString::getUISpecs()
{
    Json::Value data;
    data["id"] = id;
    data["widget"] = "text";    // You select your widget here
    data["value"] = value;
    data["meta"] = metadata;
    return data;
}
```

On the javascript side, the `MarbleBar` GUI will expect to find the specified view in the `MarbleBar.Widgets` global object. Each widget should be a subclass of the `MarbleBar.Widget` global class.

There is a helper function that automatically does this subclassing and method overloading so you can only focus to the widget development. Have a look on the respective widget for the `PString` property:

```javascript
MarbleBar.Widgets['text'] = MarbleBar.Widget.create(
    // Constructor
    function( hostDOM, specs ) {
        // Initialize widget
        var id = MarbleBar.new_id();
        this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
        this.input = $('<input class="form-control" id="'+id+'"></input>').appendTo(
            $('<div class="col-sm-10"></div>').appendTo(hostDOM)
        );
        // Register updates
        $(this.input).on("click blur keyup", (function() {
            // Trigger value update
            this.trigger("update", { "value": this.input.val() });
        }).bind(this));
    }, {
        // Update widget value
        update: function(value) {
            // Set to text field value
            this.input.attr("value", value);
        }
    }
);
``` 

The `update` function is triggered when the property is changed by the C++ code. Respectively, the widget can trigger arbitrary events to the C++ code. In this case the `update` event updates the value of the property.

You can see how this event is handled in the `PString::handleUIEvent` function:

```cpp
void PString::handleUIEvent( const string & event, const Json::Value & data )
{
    if (event == "update") {
        value = data["value"].asString();
        this->markAsDirty();
    }
}
```

Remember to call the `Property::markAsDirty` function in order to propagate the changes to the other GUI instances.

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
