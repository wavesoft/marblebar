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

#include "marblebar/properties/bool.hpp"
using namespace mb;

/**
 * PBool Constructor
 */
PBool::PBool( const string & title, const bool & defaultValue )
 : Property(), value(defaultValue)
{
	metadata["title"] = title;
}

/**
 * Overridable function to apply a property change to it's contents
 */
void PBool::handleUIEvent( const string & event, const Json::Value & data )
{
	if (event == "update") {
		value = data["value"].asBool();
		this->markAsDirty();
	}
}

/**
 * Overridable function to render the property value to a JSON value
 */
Json::Value PBool::getUIValue()
{
	return value;
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value PBool::getUISpecs()
{
	Json::Value data;
	data["id"] = id;
	data["widget"] = "toggle";
	data["value"] = value;
	data["meta"] = metadata;
	return data;
}

//////////////////////////////////////////////
// Ease of use operators
//////////////////////////////////////////////

/**
 * Static cast to string
 */
PBool::operator bool() const
{
	return value;
}

/**
 * Assign operator
 */
PBool & PBool::operator= ( const bool & value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PBool & PBool::operator= ( bool value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}
