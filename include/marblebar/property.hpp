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

#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>

using namespace std;

namespace mb {

	// Forward declarations
	class Property;
	typedef std::shared_ptr<Property> 	PropertyPtr;
	typedef std::weak_ptr<Property> 	PropertyWeakPtr;

	// Event handling function
	typedef std::function<void ( const Json::Value & args )>	EventCallback;

}

// view.hpp depends on us, so we should define pointers first
#include <marblebar/view.hpp>

namespace mb {

	/**
	 * Property base class from which widgets can derrive
	 */
	class Property : public enable_shared_from_this<Property> {
	public:

		/**
		 * Property constructor
		 */
		Property();

		/**
		 * Attach to a view
		 */
		void 				attach( const ViewPtr& view, const string & id );

		/**
		 * Mark property value as dirty
		 */
		void				markAsDirty();

		/**
		 * Receive a UI event
		 */
		void 				receiveUIEvent( const string & event, const Json::Value & data );

		/**
		 * Update a metadata field
		 */
		PropertyPtr 		meta( const string & property, const Json::Value & value );

		/**
		 * Register an event handler
		 */
		PropertyPtr 		on( const string & event, EventCallback callback );

		/**
		 * Unregister an event handler
		 */
		PropertyPtr 		off( const string & event, EventCallback callback );

		/**
		 * Overridable function to apply a property change to it's contents
		 */
		virtual void 		handleUIEvent( const string & event, const Json::Value & data ) { };

		/**
		 * Overridable function to render the property value to a JSON value
		 */
		virtual Json::Value getUIValue() = 0;

		/**
		 * Overridable function to return property specifications for the js UI
		 */
		virtual Json::Value getUISpecs();

	public:

		/**
		 * The parent view
		 */
		ViewPtr			view;

		/**
		 * Metatada information
		 */
		Json::Value 	metadata;

		/**
		 * The view is ID
		 */
		string 			id;

	protected:

		/**
		 * Flag if this view is attached
		 */
		bool 			attached;

		/**
		 * List of event callbacks
		 */
		map< string, vector< EventCallback > > eventCallbacks;

	};

};


#endif /* _MARBLEBAR_PROPERTY_HPP_ */