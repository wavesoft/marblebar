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

#ifndef _MARBLEBAR_PROP_BOOL_HPP_
#define _MARBLEBAR_PROP_BOOL_HPP_

#include <json/json.h>
#include <string>
#include <memory>

using namespace std;

// view.hpp depends on us, so we should define pointers first
#include <marblebar/view.hpp>

namespace mb {

	// Forward declarations
	class PString;
	typedef std::shared_ptr<PString> 	PStringPtr;
	typedef std::weak_ptr<PString> 		PStringWeakPtr;

	/**
	 * Property base class from which widgets can derrive
	 */
	class PString : public Property {
	public:

		/**
		 * Initialize a MarbleBar property
		 */
		PString( const string & title, const string & defaultValue = "");

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
		operator string() const;

		/**
		 * Assign operator
		 */
		PString & operator= ( const string & str );
		PString & operator= ( string str );
		PString & operator= ( const char* str );
		PString & operator= ( char* str );

		/**
		 * Append operator
		 */
		PString & operator+= ( const string & str );
		PString & operator+= ( string str );
		PString & operator+= ( const char* str );
		PString & operator+= ( char* str );

	private:

		/**
		 * The internal property
		 */
		string 				value;

	};

};


#endif /* _MARBLEBAR_PROP_BOOL_HPP_ */