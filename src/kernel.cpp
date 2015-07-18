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

#include "marblebar/kernel.hpp"
#include "marblebar/session.hpp"
#include "marblebar/platform.hpp"
#include <sstream>

using namespace mb;

/**
 * Marblebar kernel constructor
 */
Kernel::Kernel( ConfigPtr config ) : Webserver(config), config(config), lastViewID(0)
{ }

/**
 * Marblebar kernel destructor
 */
Kernel::~Kernel()
{ }

/**
 * Add a view in the marblebar kernel
 */
KernelPtr Kernel::addView( ViewPtr view )
{
	// Keep view
	views.push_back( view );
	// Attach to the kernel
	view->attach( shared_from_this(), getNextViewID() );
	// Broadcast the fact that a view is added
	this->broadcastViewAdded( view );

	// Return instance for chain-calling
	return shared_from_this();
}

/**
 * Return view ptr
 */
ViewPtr	Kernel::getViewByID( const string & id )
{
	// Get view by ID
	for (auto it = views.begin(); it != views.end(); ++it)
		if ((*it)->id == id)
			return (*it);
	// Return empty view pointer
	return ViewPtr();
}

/**
 * Create and store a new view with the specified ID
 */
ViewPtr Kernel::createView( const string & title )
{
	// Create a shared view
	ViewPtr view = make_shared<View>( title );

	// Keep view
	views.push_back( view );
	// Attach to the kernel
	view->attach( shared_from_this(), getNextViewID() );
	// Broadcast the fact that a view is added
	this->broadcastViewAdded( view );

	// Return instance
	return view;
}

/**
 * Open browser and point to the GUI
 */
void Kernel::openGUI( )
{
	// Open GUI
	openGUIURL( config );
}

/**
 * Broadcast to all session the fact that a view is added
 */
void Kernel::broadcastViewAdded( ViewPtr view )
{
	// Ignore broadcasts originating from the current session
	SessionPtr ignore;
	if (activeConnection)
		ignore = dynamic_pointer_cast<Session>(activeConnection);

	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		if ((*it).second != ignore)
			(dynamic_pointer_cast<Session>((*it).second))->notifyViewAdded( view );
}

/**
 * Broadcast to all session the fact that a view is removed
 */
void Kernel::broadcastViewRemoved( ViewPtr view )
{
	// Ignore broadcasts originating from the current session
	SessionPtr ignore;
	if (activeConnection)
		ignore = dynamic_pointer_cast<Session>(activeConnection);

	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		if ((*it).second != ignore)
			(dynamic_pointer_cast<Session>((*it).second))->notifyViewRemoved( view );
}

/**
 * Broadcast to all session the fact that a view is updated
 */
void Kernel::broadcastViewUpdated( ViewPtr view )
{
	// Ignore broadcasts originating from the current session
	SessionPtr ignore;
	if (activeConnection)
		ignore = dynamic_pointer_cast<Session>(activeConnection);

	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		if ((*it).second != ignore)
			(dynamic_pointer_cast<Session>((*it).second))->notifyViewUpdated( view );
}

/**
 * Broadcast to all session the fact that a view property is changed
 */
void Kernel::broadcastViewPropertyUpdate( ViewPtr view, PropertyPtr property )
{
	// Ignore broadcasts originating from the current session
	SessionPtr ignore;
	if (activeConnection)
		ignore = dynamic_pointer_cast<Session>(activeConnection);

	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		if ((*it).second != ignore)
			(dynamic_pointer_cast<Session>((*it).second))->notifyViewPropertyUpdate( view, property );
}

/**
 * Create a new instance of the WebserverConnection
 */
WebserverConnectionPtr Kernel::openConnection( const std::string& domain, const std::string uri )
{
	// Return new Session instance
	return dynamic_pointer_cast<WebserverConnection>(
			make_shared<Session>( shared_from_this(), domain, uri )
		);
}

/**
 * Calculate and return the next view ID
 */
string Kernel::getNextViewID()
{
	// Compose view ID 
	ostringstream oss;
	oss << "v" << (++lastViewID);
	// Return view-index
	return oss.str();
}
