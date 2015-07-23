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

#include "marblebar/properties/list.hpp"
using namespace mb;

/**
 * PList Constructor
 */
PList::PList( const string & title, const int & defaultIndex )
 : Property(), index(defaultIndex), options()
{
	metadata["title"] = title;
}

/**
 * Overridable function to apply a property change to it's contents
 */
void PList::handleUIEvent( const string & event, const Json::Value & data )
{
	if (event == "update") {
		// Update index
		index = data["index"].asInt();
		this->markAsDirty();
	}
}

/**
 * Add an option in the list
 */
PListPtr PList::addOption( const string & name, const string & value )
{
	// Update options
	options.push_back( make_pair( name, value ) );

	// Return a shared pointer to this
	return dynamic_pointer_cast<PList>( shared_from_this() );
}

/**
 * Overridable function to render the property value to a JSON value
 */
Json::Value PList::getUIValue()
{
	return index;
}

/**
 * Overridable function to return property specifications for the js UI
 */
Json::Value PList::getUISpecs()
{
	Json::Value data, options;

	// Add options
	for (auto o = this->options.begin(); o != this->options.end(); ++o) {
		Json::Value opt;
		opt.append( (*o).first );
		opt.append( (*o).second );
		options.append( opt );
	}

	data["id"] = id;
	data["widget"] = "list";
	data["value"] = index;
	data["options"] = options;
	data["meta"] = metadata;
	return data;
}

//////////////////////////////////////////////
// Ease of use operators
//////////////////////////////////////////////

/**
 * Static cast to int
 */
PList::operator int() const
{
	return index;
}

/**
 * Static cast to string
 */
PList::operator string() const
{
	return options[index].second;
}

/**
 * Assign operator
 */
PList & PList::operator= ( const int idx )
{
	this->index = idx;
	this->markAsDirty();
	return *this;
}

/**
 * Assign operator
 */
PList & PList::operator= ( const string & str )
{
	int idx = 0;
	for (auto o = this->options.begin(); o != this->options.end(); ++o) {
		if (!(*o).second.compare( str )) {
			this->index = idx;
			this->markAsDirty();
			return *this;
		}
		idx ++;
	}
	return *this;
}

/**
 * Assign operator
 */
PList & PList::operator= ( const char* str )
{
	int idx = 0;
	for (auto o = this->options.begin(); o != this->options.end(); ++o) {
		if (!(*o).second.compare( str )) {
			this->index = idx;
			this->markAsDirty();
			return *this;
		}
		idx ++;
	}
	return *this;
}

