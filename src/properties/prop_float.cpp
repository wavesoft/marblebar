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

#include "marblebar/properties/float.hpp"
using namespace mb;

/**
 * PFloat Constructor
 */
PFloat::PFloat( const string & title, const float defaultValue )
 : Property(), value(defaultValue)
{
	metadata["title"] = title;
	metadata["min"] = min;
	metadata["max"] = max;
	metadata["step"] = step;
}

/**
 * Overridable function to apply a property change to it's contents
 */
void PFloat::handleUIEvent( const string & event, const Json::Value & data )
{
	if (event == "update") {
		value = (float)( data["value"].asDouble() );
		this->markAsDirty();
	}
}

/**
 * Overridable function to render the property value to a JSON value
 */
Json::Value PFloat::getUIValue()
{
	return value;
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value PFloat::getUISpecs()
{
	Json::Value data;
	data["id"] = id;
	data["widget"] = "slider";
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
PFloat::operator float() const
{
	return value;
}

/**
 * Assign operator
 */
PFloat & PFloat::operator= ( const float & value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PFloat & PFloat::operator= ( float value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}


/**
 * Assign operator
 */
PFloat & PFloat::operator+= ( const float & value )
{
	this->value += value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PFloat & PFloat::operator+= ( float value )
{
	this->value += value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PFloat & PFloat::operator-= ( const float & value )
{
	this->value -= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PFloat & PFloat::operator-= ( float value )
{
	this->value -= value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PFloat & PFloat::operator/= ( const float & value )
{
	this->value /= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PFloat & PFloat::operator/= ( float value )
{
	this->value /= value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PFloat & PFloat::operator*= ( const float & value )
{
	this->value *= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PFloat & PFloat::operator*= ( float value )
{
	this->value *= value;
	this->markAsDirty();
	return *this;
}
