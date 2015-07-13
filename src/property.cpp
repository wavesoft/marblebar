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

#include "marblebar/property.hpp"
#include <algorithm>

using namespace mb;
using namespace std;

/**
 * Property constructor
 */
Property::Property()
 : metadata(), attached(false), eventCallbacks()
{ }

/**
 * Set a metadata property
 */
PropertyPtr Property::meta( const string & property, const Json::Value & value )
{
	// Update property
	metadata[property] = value;
	// Return instance for chaining calls
	return shared_from_this();
}

/**
 * Register an event handler
 */
PropertyPtr Property::on( const string & event, EventCallback callback )
{
	// Create a vector of event callbacks
	if (eventCallbacks.find(event) == eventCallbacks.end()) {
		eventCallbacks[event] = vector< EventCallback >();
	}

	// Register event callback
	eventCallbacks[event].push_back( callback );

	// Return this for chain calling
	return shared_from_this();
}

/**
 * Marblebar Property constructor
 */
void Property::attach( const ViewPtr& view, const string & id )
{
	this->view = view;
	this->id = id;
	this->attached = true;
}

/**
 * Mark property as dirty
 */
void Property::markAsDirty() 
{
	// Do not do anything unless attached
	if (!this->attached) return;

	// Notify view that I am dirty
	view->markPropertyAsDirty( shared_from_this() );
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value Property::getUISpecs()
{
	Json::Value data;
	data["id"] = id;
	data["widget"] = "text";
	data["value"] = getUIValue();
	data["meta"] = metadata;
	return data;
}

/**
 * Overridable function to apply a property change to it's contents
 */
void Property::receiveUIEvent( const string & event, const Json::Value & data )
{

	// Forward to the
	handleUIEvent( event, data );

	// Lookup listeners
	if (eventCallbacks.find(event) == eventCallbacks.end())
		return;

	// Trigger them
	for (auto it = eventCallbacks[event].begin(); it != eventCallbacks[event].end(); ++it)
		(*it)( data );

}
