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
#include <sstream>
using namespace mb;

/**
 * Marblebar View constructor
 */
View::View( const string & title ) : 
	attached(false), id(""), propertyGroups(), metadata(), lastPropertyID(0)
{
	metadata["title"] = title;
}

/**
 * Create/Return a property group
 */
PropertyGroupPtr View::group( const string& title )
{
	// Create if missing
	if (propertyGroups.find(title) == propertyGroups.end()) {
		propertyGroups[title] = make_shared<PropertyGroup>( title, shared_from_this() );
	}
	// Return
	return propertyGroups[title];
}

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
 * Set a metadata property
 */
ViewPtr View::meta( const string & property, const Json::Value & value )
{
	// Update property
	metadata[property] = value;
	// Return instance for chaining calls
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
	Json::Value value, specs, groups;
	value["id"] = id;

	// Iterate over property groups
	for (auto it = propertyGroups.begin(); it != propertyGroups.end(); ++it) {
		Json::Value prop;
		for (auto jt = (*it).second->properties.begin(); jt != (*it).second->properties.end(); ++jt) {
			prop.append( (*jt)->getUISpecs() );
		}

		// Store group
		groups[(*it).second->title] = prop;
	}

	value["properties"] = groups;
	value["meta"] = metadata;

	// Return specifications
	return value;
}


/**
 * Calculate and return the next property ID
 */
string View::getNextPropertyID()
{
	// Compose view ID 
	ostringstream oss;
	oss << "p" << (++lastPropertyID);
	// Return view-index
	return oss.str();
}

/**
 * Return a property by it's ID
 */
PropertyPtr View::propertyById( const string& id )
{
	// Iterate over property groups
	for (auto it = propertyGroups.begin(); it != propertyGroups.end(); ++it) {
		// Iterate over their properties
		for (auto jt = (*it).second->properties.begin(); jt != (*it).second->properties.end(); ++jt) {
			// Compare IDs
			if ((*jt)->id == id)
				return *jt;
		}
	}
	// Return nothing
	return PropertyPtr();
}
