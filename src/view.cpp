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

#include "marblebar/view.hpp"
using namespace mb;

/**
 * Marblebar View constructor
 */
View::View() : 
	attached(false), id(""), properties()
{ }

/**
 * Marblebar View constructor
 */
void View::attach( const KernelPtr& kernel, const string& id )
{
	this->kernel = kernel;
	this->id = id;
	this->attached = true;
}

/**
 * Marblebar View constructor
 */
ViewPtr View::addProperty( PropertyPtr property )
{
	// Do not do anything unless attached
	if (!this->attached) return shared_from_this();

	// Attach property to me
	property->attach( shared_from_this(), "" );

	// Keep property
	properties.push_back( property );

	// Return instance for chain-calling
	return shared_from_this();

}

/**
 * Mark property as dirty
 */
void View::markPropertyAsDirty( const PropertyPtr & property )
{
	// Do not do anything unless attached
	if (!this->attached) return;

	// Broadcast to the kerne
	kernel->broadcastViewPropertyUpdate( shared_from_this(), property );

}

/**
 * Get view UI specifications
 */
Json::Value View::getUISpecs()
{
	// Do not do anything unless attached
	if (!this->attached) return Json::Value();

	// Get View ID
	Json::Value value, prop;
	value["id"] = id;

	// Iterate over properties and collect specs
	for (auto it = properties.begin(); it != properties.end(); ++it) {
		prop.append( (*it)->getUISpecs() );
	}
	value["properties"] = prop;

	// Return specifications
	return value;
}
