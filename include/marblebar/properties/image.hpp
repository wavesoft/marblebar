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

#ifndef _MARBLEBAR_PROP_IMAGE_HPP_
#define _MARBLEBAR_PROP_IMAGE_HPP_

#include <json/json.h>
#include <string>
#include <memory>

using namespace std;

// view.hpp depends on us, so we should define pointers first
#include <marblebar/view.hpp>

namespace mb {

	// Forward declarations
	class PImage;
	typedef std::shared_ptr<PImage> 	PImagePtr;
	typedef std::weak_ptr<PImage> 		PImageWeakPtr;

	/**
	 * Property base class from which widgets can derrive
	 */
	class PImage : public Property {
	public:

		/**
		 * Initialize a MarbleBar property
		 */
		PImage( const string & title, const int width = -1, const int height = -1, const string & defaultValue = "about:blank" );

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
		PImage & operator= ( const string & str );
		PImage & operator= ( string str );
		PImage & operator= ( const char* str );
		PImage & operator= ( char* str );

		/**
		 * Set from binary buffer
		 */
		void 				setBinary( const unsigned char * buffer, size_t len, const string & contentType );

	private:

		/**
		 * The internal property
		 */
		string 				value;

		/**
		 * Sie of the image
		 */
		int 				width;
		int 				height;

	};

};


#endif /* _MARBLEBAR_PROP_IMAGE_HPP_ */