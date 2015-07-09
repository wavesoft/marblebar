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

#ifndef _MARBLEBAR_PROPERTY_HPP_
#define _MARBLEBAR_PROPERTY_HPP_

#include <string>
#include <memory>
using namespace std;

namespace mb {

	// Forward declarations
	class Property;
	typedef std::shared_ptr<Property> 	PropertyPtr;
	typedef std::weak_ptr<Property> 	PropertyWeakPtr;

	/**
	 * Property class
	 */
	class Property {
	public:

		/**
		 * Initialize a MarbleBar property
		 */
		Property( const string & name );

	};

};


#endif /* _MARBLEBAR_PROPERTY_HPP_ */