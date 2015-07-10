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
#ifndef _MB_WEBSERVER_H_
#define _MB_WEBSERVER_H_

#include <mongoose.h>
#include <marblebar/config.hpp>
#include <marblebar/server/webserver_connection.hpp>

#include <string>
#include <map>
#include <memory>
#include <mutex>
using namespace std;

namespace mb {

	// Forward declarations
	class Webserver;
	typedef std::shared_ptr<Webserver> 	WebserverPtr;
	typedef std::weak_ptr<Webserver> 	WebserverWeakPtr;

	/**
	 * This class encapsulates the Mongoose (webserver) instance and provides
	 * the core functionality for interfacing with javascript via JSON RPC.
	 */
	class Webserver {
	public:

		/**
		 * Create a webserver and setup listening port
		 */
		Webserver( ConfigPtr config );

		/**
		 * Cleanup and destroy server
		 */
		virtual ~Webserver();

		/**
		 * Poll server for incoming events. 
		 * This function should be called periodically to receive events.
		 */
		void poll( const int timeout = 100 );

		/**
		 * Start the infinite loop for the server.
		 * After calling this function the only way to stop the server is
		 * an interrupt signal or to call ``stop`` function from another thread.
		 */
		void start();

		/**
		 * Check if there are live registered connections
		 */
		bool hasLiveConnections();

	public:

		/**
		 * Serve a static resource under the given URL
		 */
		void serve_static( const string& url, const string& file );

	protected:

		/**
		 * Create a new instance of the WebserverConnection
		 */
		virtual WebserverConnectionPtr openConnection( const string& domain, const string uri ) = 0;

		/**
		 * A list of active webserver connections
		 */
		map<mg_connection*, WebserverConnectionPtr>		connections;

		/**
		 * The current connection under processing
		 */
		WebserverConnectionPtr							activeConnection;

	private:

		/**
		 * Configuration details
		 */
		ConfigPtr 										config;

		/**
		 * Mutex for accessing the connections array
		 */
		mutex 											connMutex;

		/**
		 * The mongoose server instance
		 */
		mg_server*										server;

		/**
		 * Map of static resources
		 */
		map< string, string > 							staticResources;

		/**
		 * Iterator over the websocket connections
		 */
		static int 	iterate_callback(struct mg_connection *c, enum mg_event ev);

		/**
		 * This is the entry point for the CernVM Web API I/O
		 */
		static int 	api_handler(struct mg_connection *conn);

		/**
		 * Raw event handler
		 */
		static int ev_handler(struct mg_connection *conn, enum mg_event ev);

	};

};

#endif /* _MB_WEBSERVER_H_ */
