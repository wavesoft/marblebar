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

#ifndef _MARBLEBAR_PROP_STRING_HPP_
#define _MARBLEBAR_PROP_STRING_HPP_

#include <json/json.h>
#include <string>
#include <memory>

using namespace std;

// view.hpp depends on us, so we should define pointers first
#include <marblebar/view.hpp>

namespace mb {

	// Forward declarations
	class PBool;
	typedef std::shared_ptr<PBool> 	PBoolPtr;
	typedef std::weak_ptr<PBool> 	PBoolWeakPtr;

	/**
	 * Property base class from which widgets can derrive
	 */
	class PBool : public Property {
	public:

		/**
		 * Initialize a MarbleBar property
		 */
		PBool( const string & title, const bool & defaultValue = false);

		/**
		 * Overridable function to apply a property change to it's contents
		 */
		virtual void 		handleUIEvent( const string & event, const Json::Value & data );

		/**
		 * Overridable function to render the property value to a JSON value
		 */
		virtual Json::Value getUIValue();

		/**
		 * Overridable function to return property specifications for the js UI
		 */
		virtual Json::Value getUISpecs();

	public:

		/**
		 * Static cast to string
		 */
		operator bool() const;

		/**
		 * Assign operator
		 */
		PBool & operator= ( const bool & str );
		PBool & operator= ( bool str );

	private:

		/**
		 * The internal property
		 */
		bool 				value;

	};

};


#endif /* _MARBLEBAR_PROP_STRING_HPP_ */