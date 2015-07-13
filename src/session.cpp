/**
 * This file is part of the MarbleBar Library.
 *
 * libMarbleBar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libMarbleBar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libMarbleBar. If not, see <http://www.gnu.org/licenses/>.
 *
 * Developed by Ioannis Charalampidis 2015
 * Contact: <ioannis.charalampidis[at]cern.ch>
 */

#include "marblebar/session.hpp"
#include <iostream>

using namespace mb;

/**
 * Marblebar Session constructor
 */
Session::Session( KernelPtr kernel, const string& domain, const string uri ) : 
	kernel(kernel), WebserverConnection( domain, uri )
{ }

/**
 * Notify to session the fact that a view is added
 */
void Session::notifyViewAdded( ViewPtr view )
{
	// Trigger view add
	sendAction( "view/add", view->getUISpecs() );
}

/**
 * Notify to session the fact that a view is removed
 */
void Session::notifyViewRemoved( ViewPtr view )
{
	Json::Value data;
	data["id"] = view->id;
	// Trigger view remove
	sendAction( "view/remove", data );
}

/**
 * Notify to session the fact that a view is updated
 */
void Session::notifyViewUpdated( ViewPtr view )
{
	// Trigger view update
	sendAction( "view/update", view->getUISpecs() );
}

/**
 * Notify to session the fact that a view property is changed
 */
void Session::notifyViewPropertyUpdate( ViewPtr view, PropertyPtr property )
{
	Json::Value data;
	data["id"] = view->id;
	data["prop"] = property->id;
	data["value"] = property->getUIValue();

	// Trigger view property change
	sendAction( "view/propchange", data );
}

/**
 * Javascript event handler
 */
void Session::handleEvent( const string& id, const string& event, const Json::Value& data )
{
	
	if (event == "ui/init") {

		// Initialize the UI by sending all the view specifications
		for (auto it = kernel->views.begin(); it != kernel->views.end(); ++it) {
			this->notifyViewAdded( *it );
		}

	} else if (event == "property/event") {

	    // Ensure we have an action defined
	    if (!data.isMember("view")) {
	        sendError("Missing 'view' parameter in the incoming request", id);
	        return;
	    }
	    if (!data.isMember("prop")) {
	        sendError("Missing 'prop' parameter in the incoming request", id);
	        return;
	    }
	    if (!data.isMember("name")) {
	        sendError("Missing 'name' parameter in the incoming request", id);
	        return;
	    }
	    if (!data.isMember("data")) {
	        sendError("Missing 'data' parameter in the incoming request", id);
	        return;
	    }
	    // Fetch view and property id
	    string viewName = data["view"].asString();
	    string propName = data["prop"].asString();
	    string event = data["name"].asString();

		// Locate a view with the specified ID
		for (auto it = kernel->views.begin(); it != kernel->views.end(); ++it) {
			ViewPtr view = *it;

			// Find view
			if (view->id == viewName) {

				// Locate property with specified ID
				for (auto jt = view->properties.begin(); jt != view->properties.end(); jt++) {
					PropertyPtr prop = *jt;

					// Find property
					if (prop->id == propName) {

						// Handle event by the property
						prop->receiveUIEvent( event, data["data"] );

						// Do not continue
						return;

					}

				}

				// Property not found
				sendError("Specified property was not found", id);
				return;
			}

		}

		// View not found
		sendError("Specified view was not found", id);

	} else {

		// Send error
		sendError("Unknown event received", id);

	}

}
