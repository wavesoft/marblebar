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

#ifndef _MARBLEBAR_PROPERTY_GROUP_TPL_HPP_
#define _MARBLEBAR_PROPERTY_GROUP_TPL_HPP_

#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>

using namespace std;

// view.hpp depends on us, so we should define pointers first
#include <marblebar/property_group.hpp>

namespace mb {

	/**
	 * Define the template function
	 */
	template<class T> 
	shared_ptr<T> PropertyGroup::addProperty( shared_ptr<T> property ) 
	{

		// Create property
		properties.push_back(
				dynamic_pointer_cast<Property>( property )
			);

		// Attach to this
		property->attach( view, view->getNextPropertyID() );

		// Pass-through
		return property;

	}

};


#endif /* _MARBLEBAR_PROPERTY_GROUP_TPL_HPP_ */