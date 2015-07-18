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

#include "marblebar/server/webserver.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace mb;

/**
 * This function is generated at build-time and contains the static resources
 */
extern const char *find_embedded_file(const string&, size_t *);

/**
 * Send an error message
 */
int send_error( struct mg_connection *conn, const char* message, const int code = 500 ) 
{

    // Send error code
    mg_send_status(conn, code);

    // Send payload
    mg_printf_data( conn, 
        "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>CernVM WebAPI :: Error</title>\n</head>\n"
        "<body><h1>CernVM WebAPI Error %i</h1><p>%s</p></body>"
        "</html>", code, message );

    // Request processed
    return MG_TRUE;

}

/**
 * This is the entry point for the CernVM Web API I/O
 */
int Webserver::api_handler(struct mg_connection *conn) 
{

	// Fetch 'this' from the connection server object
	Webserver* self = static_cast<Webserver*>(conn->server_param);

    // Try to identify domain by the 'Origin' header
    const char * c_origin = mg_get_header(conn, "Origin");
    string domain = ""; 
    if (c_origin != NULL) {
    	domain=c_origin;
    	size_t slashPos = domain.find("//");
    	if (slashPos != string::npos) {
	    	domain = domain.substr( slashPos+2, domain.length()-slashPos-2 );
    	}
        size_t colonPos = domain.find(":");
        if (colonPos != string::npos) {
            domain = domain.substr( 0, colonPos );
        }
    }

    // Move URI to std::string
    std::string url = conn->uri;

    // DEBUG response
    if (conn->is_websocket) {

        // Check if a connection is active
        WebserverConnectionPtr c;
        if (self->connections.find(conn) == self->connections.end()) {
	        unique_lock<mutex> objectLock(self->connMutex, std::try_to_lock);

            // Initialize a new connection if such connection
            // does not exist.
            c = self->openConnection(domain, url);
            c->isIterated = true;
            self->connections[conn] = c;

        } else {
            c = self->connections[conn];
        }

        // Handle TEXT frames 
        if ( (conn->wsbits & 0x0F) == 0x01) {
            self->activeConnection = c;
            c->handleRawData(conn->content, conn->content_len);
            self->activeConnection = WebserverConnectionPtr();
        }

        // Check if connection is closed
        return c->isConnected() ? MG_TRUE : MG_FALSE;

    } else {

        // Trim trailing & heading slash from URL
        if (url[url.length()-1] == '/')
            url = url.substr(0, url.length()-1);
        if (url[0] == '/')
            url = url.substr(1, url.length()-1);

        // Check for embedded resources
        size_t res_size;
        const char *res_buffer = find_embedded_file( url, &res_size);
        if ( url == "info" ) {

            // Enable CORS (important for allowing every website to contact us)
            mg_send_header(conn, "Access-Control-Allow-Origin", "*" );
            mg_printf_data(conn, "{\"status\":\"ok\",\"request\":\"%s\",\"domain\":\"%s\",\"version\":\"%s\"}", conn->uri, domain.c_str(), self->config->version.c_str());
            return MG_TRUE;

        } else if (res_buffer == NULL) {
            
            // File not found
            return send_error( conn, "File not found", 404);

        } else {

            // Get MIME type of the file to send and send header
            const char * mimeType = mg_get_mime_type( url.c_str(), "text/plain" );
            mg_send_header(conn, "Content-Type", mimeType );

            // Send data
            mg_send_data( conn, res_buffer, res_size );
            return MG_TRUE;

        }

	    
    }
    return 1;

}

/**
 * Iterator over the websocket connections
 */
int Webserver::iterate_callback(struct mg_connection *conn, enum mg_event ev) 
{

    // Fetch 'this' from the connection server object
    Webserver* self = static_cast<Webserver*>(conn->callback_param);

    // Handle websockets
    if ((ev == MG_POLL) && conn->is_websocket) {

        // Check if a WebserverConnectionPtr is active
        WebserverConnectionPtr c;
        if (self->connections.find(conn) == self->connections.end()) {
            // This connection is not handled by us!
            return MG_TRUE;

        } else {
            c = self->connections[conn];
        }

        // Mark socket as iterated
        c->isIterated = true;

        // Send all frames of the egress queue
        std::string buf;
        while ( !(buf = c->getEgressRawData()).empty() ) {
            mg_websocket_write(conn, 0x01, buf.c_str(), buf.length());
        }

        // If we are disconnected, send disconnect frame
        if (!c->isConnected()) {

            // Send Connection Close Frame
            mg_websocket_write(conn, 0x08, NULL, 0);

            // Mark as non-iterated so it's deleted on poll()
            c->isIterated = false;

        }

    }

    // We are done with
    return MG_TRUE;

}

/**
 * RAW Request handler
 */
int Webserver::ev_handler(struct mg_connection *conn, enum mg_event ev) 
{
    if (ev == MG_REQUEST) {
        return api_handler(conn);
    } else if (ev == MG_AUTH) {
        return MG_TRUE;
    } else {
        return MG_FALSE;
    }
}

/**
 * Create a webserver and setup listening port
 */
Webserver::Webserver( ConfigPtr config ) 
    : config(config), staticResources(), connections(), activeConnection(), connMutex()
{

	// Create a mongoose server, passing the pointer
	// of this class, in order for the C callbacks
	// to have access to the class instance.
	server = mg_create_server( this, Webserver::ev_handler );

	// Prepare the listening endpoint info
	ostringstream ss; ss << "127.0.0.1:" << config->webserverPort;
    mg_set_option(server, "listening_port", ss.str().c_str());

}

/**
 * WebServer destructor
 */
Webserver::~Webserver() 
{

    // Destroy mongoose server
    mg_destroy_server( &server );

	// Destroy connections
    {
        unique_lock<mutex> objectLock(connMutex, std::try_to_lock);

        std::map<mg_connection*, WebserverConnectionPtr>::iterator it;
        for (it=connections.begin(); it!=connections.end(); ++it) {
            WebserverConnectionPtr c = it->second;
            c->cleanup();
        }

        // Clear map
        connections.clear();

    }

}

/**
 * Serve a static resource under the given URL
 */
void Webserver::serve_static( const std::string& url, const std::string& file ) 
{

    // Store on staticResources
    staticResources[url] = file;

}

/**
 * Poll server for incoming events. 
 * This function should be called periodically to receive events.
 */
void Webserver::poll( const int timeout) 
{

    // Mark all the connections as 'not iterated'
    {
        unique_lock<mutex> objectLock(connMutex, std::try_to_lock);
        std::map<mg_connection*, WebserverConnectionPtr>::iterator it;
        for (it=connections.begin(); it!=connections.end(); ++it) {
            WebserverConnectionPtr c = it->second;
            c->isIterated = false;
        }
    }

    // Send the message to iterate over connections
    mg_iterate_over_connections(server, Webserver::iterate_callback, this);

	// Poll mongoose server
    mg_poll_server(server, timeout);	

    // Find dead connections
    {
        unique_lock<mutex> objectLock(connMutex, std::try_to_lock);
        std::map<mg_connection*, WebserverConnectionPtr>::iterator it;
        for (it=connections.begin(); it!=connections.end(); ++it) {
            WebserverConnectionPtr c = it->second;

            // Delete non-iterated over actions
            if (!c->isIterated) {

                // Release connection object
                c->cleanup();

                // Delete element
                connections.erase(it);

                // Skip deleted element or exit
                if (connections.empty()) {
                    break;
                    
                } else {
                    // Otherwise rewind pointer
                    it = connections.begin();
                }

            }
        }
    }

}

/**
 * Start the infinite loop for the server.
 * After calling this function the only way to stop the server is
 * an interrupt signal or to call ``stop`` function from another thread.
 */
void Webserver::start() 
{

	// Infinite loop :P
	for (;;) {
		poll();
	}

}

/**
 * Check if there are live registered connections
 */
bool Webserver::hasLiveConnections() 
{
    unique_lock<mutex> objectLock(connMutex, std::try_to_lock);
    return !connections.empty();
}
