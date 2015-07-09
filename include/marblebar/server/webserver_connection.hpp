/**
 * This file is part of CernVM Web API Plugin.
 *
 * CVMWebAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CVMWebAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CVMWebAPI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Developed by Ioannis Charalampidis 2013
 * Contact: <ioannis.charalampidis[at]cern.ch>
 */

#pragma once
#ifndef _MB_WEBSERVER_CONNECTION_H_
#define _MB_WEBSERVER_CONNECTION_H_

#include <mongoose.h>
#include <json/json.h>

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
using namespace std;

namespace mb {

	// Forward declarations
	class WebserverConnection;
	typedef std::shared_ptr<WebserverConnection> 	WebserverConnectionPtr;
	typedef std::weak_ptr<WebserverConnection> 		WebserverConnectionWeakPtr;

	/**
	 * Abstract class for connection handlers
	 */
	class WebserverConnection {
	///////////////////////////////////////////////////
	// High-level operations, used by the user       //
	///////////////////////////////////////////////////
	public:

		/**
		 * Reply to an action
		 */
		void 					reply( const string& id, const Json::Value& data );

		/**
		 * Send error response
		 */
		void 					sendError( const string& message, const string& id = "" );

		/**
		 * Send a RAW message
		 */
		void 					sendRawData( const string& data );

		/**
		 * Request to disconnect from the socket.
		 */
		void 					disconnect() { connected = false; };

		/**
		 * Send a named action with arbitrary json data
		 */
		void 					sendAction( const string& event, const Json::Value& data, const string& id = "" );

		/**
		 * Handle incoming actions
		 */
		virtual void			handleEvent( const string& id, const string& event, const Json::Value& data ) = 0;

	///////////////////////////////////////////////////
	// Low-level operations, used by the Webserver   //
	///////////////////////////////////////////////////
	public:

		/**
		 * Constructor
		 */
		WebserverConnection( const string& domain, const string uri );

		/**
		 * Abstract function to cleanup before destruction
		 */
		void 					cleanup();

		/**
		 * This function is called when there is an incoming data frame 
		 * from the browser.
		 */
		void					handleRawData( const char * buffer, const size_t len );

		/**
		 * This function should return FALSe only when the connection handler
		 * decides to drop the connection.
		 */
		bool 					isConnected();

		/**
		 * Pops and returns the next frame from the egress queue, or returns
		 * an empty string if there are no data in the egress queue.
		 */
		string 					getEgressRawData();

		/**
		 * Internal flag used to track lost connections
		 */
		bool 					isIterated;

	protected:

		/**
		 * The domain where the plugin resides
		 */
		string 					domain;

		/**
		 * The request URI
		 */
		string 					uri;

		/**
		 * The egress queue
		 */
		queue< string >			egress;

		/**
		 * A status flag to let the server know when to drop the connection
		 */
		bool 					connected;

	};

};



#endif /* _MB_WEBSERVER_CONNECTION_H_ */
