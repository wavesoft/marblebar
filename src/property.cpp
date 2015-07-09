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
using namespace mb;

/**
 * Property constructor
 */
Property::Property()
 : attached(false), id("")
{ }

/**
 * Marblebar Property constructor
 */
void Property::attach( const ViewPtr & view, const string& id )
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
