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

#ifndef _MARBLEBAR_SESSION_HPP_
#define _MARBLEBAR_SESSION_HPP_

#include <memory>

using namespace std;

namespace mb {

	// Forward declarations
	class Session;
	typedef std::shared_ptr<Session> 	SessionPtr;
	typedef std::weak_ptr<Session> 		SessionWeakPtr;
}

// kernel.hpp depends on us, so we should define pointers first
#include <marblebar/kernel.hpp>
#include <marblebar/server/webserver_connection.hpp>

namespace mb {
	/**
	 * Session instance class
	 */
	class Session : public enable_shared_from_this<Session>, public WebserverConnection {
	public:

		/**
		 * Initialize a MarbleBar Session
		 */
		Session( KernelPtr kernel, const string& domain, const string uri );

		/**
		 * Notify to session the fact that a view is added
		 */
		void 					notifyViewAdded( ViewPtr view );

		/**
		 * Notify to session the fact that a view is removed
		 */
		void 					notifyViewRemoved( ViewPtr view );

		/**
		 * Notify to session the fact that a view is updated
		 */
		void 					notifyViewUpdated( ViewPtr view );

		/**
		 * Notify to session the fact that a view property is changed
		 */
		void 					notifyViewPropertyUpdate( ViewPtr view, PropertyPtr property );

	protected:

		/**
		 * Javascript event handler
		 */
		virtual void 			handleEvent( const string& id, const string& event, const Json::Value& data );

	private:

		/**
		 * Update view propeties
		 */
		void 					updateViewProperties( ViewPtr view );

		/**
		 * Session kernel
		 */
		KernelPtr 				kernel;

		/**
		 * Active view
		 */
		ViewPtr					activeView;

	};

};


#endif /* _MARBLEBAR_SESSION_HPP_ */