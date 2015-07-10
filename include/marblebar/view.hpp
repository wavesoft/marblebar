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

#ifndef _MARBLEBAR_VIEW_HPP_
#define _MARBLEBAR_VIEW_HPP_

#include <memory>
#include <vector>
#include <json/json.h>

using namespace std;

namespace mb {

	// Forward declarations
	class View;
	typedef std::shared_ptr<View> 	ViewPtr;
	typedef std::weak_ptr<View> 	ViewWeakPtr;
}

// property.hpp & kernel.hpp depends on us, so we should define pointers first
#include <marblebar/property.hpp>
#include <marblebar/kernel.hpp>

namespace mb {
	
	/**
	 * MarbleBar View
	 */
	class View : public enable_shared_from_this<View> {
	public:

		/**
		 * Initialize a MarbleBar view
		 */
		View( const string & title = "" );

		/**
		 * Attach to a kernel
		 */
		void 						attach( const KernelPtr& kernel, const string & id );

		/**
		 * Add a property
		 */
		template<class T> 
		shared_ptr<T> 				addProperty( shared_ptr<T> property ) 
		{

			// Create property
			properties.push_back(
					dynamic_pointer_cast<Property>( property )
				);

			// Attach to this
			property->attach( shared_from_this(), getNextPropertyID() );

			// Pass-through
			return property;

		}

		/**
		 * Mark a particular property as dirty
		 */
		void 						markPropertyAsDirty( const PropertyPtr& property );

		/**
		 * Get view UI specifications
		 */
		Json::Value					getUISpecs();

		/**
		 * Update a metadata field
		 */
		ViewPtr 					meta( const string & property, const Json::Value & value );

		/**
		 * Get next property ID
		 */
		string 						getNextPropertyID();

	public:

		/**
		 * The unique session ID for this instance
		 */
		string 						id;

		/**
		 * List of properties
		 */
		vector< PropertyPtr >		properties;
		
		/**
		 * Metatada information
		 */
		Json::Value 				metadata;

		/**
		 * The kernel that hosts the property
		 */
		KernelPtr					kernel;

	private:

		/**
		 * Flag if this view is attached
		 */
		bool 						attached;

		/**
		 * Last view id
		 */
		int 						lastPropertyID;

	};

};


#endif /* _MARBLEBAR_VIEW_HPP_ */