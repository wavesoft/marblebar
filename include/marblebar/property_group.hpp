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

#ifndef _MARBLEBAR_PROPERTY_GROUP_HPP_
#define _MARBLEBAR_PROPERTY_GROUP_HPP_

#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>

using namespace std;

namespace mb {

	// Forward declarations
	class PropertyGroup;
	typedef std::shared_ptr<PropertyGroup> 	PropertyGroupPtr;
	typedef std::weak_ptr<PropertyGroup> 	PropertyGroupWeakPtr;

}

// view.hpp depends on us, so we should define pointers first
#include <marblebar/view.hpp>

namespace mb {

	/**
	 * PropertyGroup base class from which widgets can derrive
	 */
	class PropertyGroup : public enable_shared_from_this<PropertyGroup> {
	public:

		/**
		 * PropertyGroup constructor
		 */
		PropertyGroup( const string & title, const ViewPtr & view )
		: properties(), view(view), title(title) { }
		
		/**
		 * Add a property
		 */
		template<class T> 
		shared_ptr<T> 				addProperty( shared_ptr<T> property );

	public:

		/**
		 * The group title
		 */
		string						title;

		/**
		 * The parent view 
		 */
		ViewPtr						view;

		/**
		 * List of properties
		 */
		vector< PropertyPtr >		properties;

	};

};


#endif /* _MARBLEBAR_PROPERTY_GROUP_HPP_ */