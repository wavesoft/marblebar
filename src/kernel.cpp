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
using namespace mb;

/**
 * Marblebar kernel constructor
 */
Kernel::Kernel( ConfigPtr config ) : Webserver(config), config(config) 
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

	// Broadcast the fact that a view is added
	this->broadcastViewAdded( view );

	// Return instance for chain-calling
	return shared_from_this();

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
	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		(dynamic_pointer_cast<Session>((*it).second))->notifyViewAdded( view );
}

/**
 * Broadcast to all session the fact that a view is removed
 */
void Kernel::broadcastViewRemoved( ViewPtr view )
{
	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		(dynamic_pointer_cast<Session>((*it).second))->notifyViewRemoved( view );
}

/**
 * Broadcast to all session the fact that a view is updated
 */
void Kernel::broadcastViewUpdated( ViewPtr view )
{
	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
		(dynamic_pointer_cast<Session>((*it).second))->notifyViewUpdated( view );
}

/**
 * Broadcast to all session the fact that a view property is changed
 */
void Kernel::broadcastViewPropertyUpdate( ViewPtr view, PropertyPtr property )
{
	// Forward to all connections
	for (auto it = connections.begin(); it != connections.end(); ++it)
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
