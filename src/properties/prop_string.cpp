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

#include "marblebar/properties/string.hpp"
using namespace mb;

/**
 * PString Constructor
 */
PString::PString( const string & title, const string & defaultValue )
 : Property(), value(defaultValue)
{
	metadata["title"] = title;
}

/**
 * Overridable function to apply a property change to it's contents
 */
void PString::handleUIEvent( const string & event, const Json::Value & data )
{
	if (event == "update") {
		value = data["value"].asString();
		this->markAsDirty();
	}
}

/**
 * Overridable function to render the property value to a JSON value
 */
Json::Value PString::getUIValue()
{
	return value;
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value PString::getUISpecs()
{
	Json::Value data;
	data["id"] = id;
	data["widget"] = "text";
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
PString::operator string() const
{
	return value;
}

/**
 * Assign operator
 */
PString & PString::operator= ( const string & str )
{
	this->value = str;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PString & PString::operator= ( string str )
{
	this->value = str;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PString & PString::operator= ( const char* str )
{
	this->value = str;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PString & PString::operator= ( char* str )
{
	this->value = str;
	this->markAsDirty();
	return *this;
}


/**
 * Assign operator
 */
PString & PString::operator+= ( const string & str )
{
	this->value += str;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PString & PString::operator+= ( string str )
{
	this->value += str;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PString & PString::operator+= ( const char* str )
{
	this->value += str;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PString & PString::operator+= ( char* str )
{
	this->value += str;
	this->markAsDirty();
	return *this;
}

