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

#include "marblebar/properties/double.hpp"
using namespace mb;

/**
 * PDouble Constructor
 */
PDouble::PDouble( const string & title, const double defaultValue, const double min, const double max, const double step )
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
void PDouble::handleUIEvent( const string & event, const Json::Value & data )
{
	if (event == "update") {
		value = data["value"].asDouble();
		this->markAsDirty();
	}
}

/**
 * Overridable function to render the property value to a JSON value
 */
Json::Value PDouble::getUIValue()
{
	return value;
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value PDouble::getUISpecs()
{
	Json::Value data;
	data["id"] = id;
	data["widget"] = "number";
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
PDouble::operator double() const
{
	return value;
}

/**
 * Assign operator
 */
PDouble & PDouble::operator= ( const double & value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PDouble & PDouble::operator= ( double value )
{
	this->value = value;
	this->markAsDirty();
	return *this;
}


/**
 * Assign operator
 */
PDouble & PDouble::operator+= ( const double & value )
{
	this->value += value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PDouble & PDouble::operator+= ( double value )
{
	this->value += value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PDouble & PDouble::operator-= ( const double & value )
{
	this->value -= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PDouble & PDouble::operator-= ( double value )
{
	this->value -= value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PDouble & PDouble::operator/= ( const double & value )
{
	this->value /= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PDouble & PDouble::operator/= ( double value )
{
	this->value /= value;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PDouble & PDouble::operator*= ( const double & value )
{
	this->value *= value;
	this->markAsDirty();
	return *this;
}

/**
 * Copy-assign operator
 */
PDouble & PDouble::operator*= ( double value )
{
	this->value *= value;
	this->markAsDirty();
	return *this;
}
