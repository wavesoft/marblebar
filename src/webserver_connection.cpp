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

#include "marblebar/server/webserver_connection.hpp"
#include <sstream>

using namespace mb;

/**
 * Webserver connection constructor
 */
WebserverConnection::WebserverConnection( const string& domain, const string uri ) :
    isIterated(false), domain(domain), uri(uri), egress(), connected(true)
{

}

/**
 * Handle incoming raw request from the browser
 */
void WebserverConnection::handleRawData( const char * buf, const size_t len ) 
{

    // Parse the incoming buffer as JSON
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( buf, buf+len, root );
    if ( !parsingSuccessful ) {
        // report to the user the failure and their locations in the document.
        sendError("Unable to parse to JSON the incoming request");
        return;
    }

    // Get event ID
    if (!root.isMember("id")) {
        sendError("Missing 'id' parameter in the incoming request");
        return;
    }
    string id = root["id"].asString();

    // Ensure we have an action defined
    if (!root.isMember("type")) {
        sendError("Missing 'type' parameter in the incoming request", id);
        return;
    }
    if (!root.isMember("name")) {
        sendError("Missing 'name' parameter in the incoming request", id);
        return;
    }
    if (!root.isMember("data")) {
        sendError("Missing 'data' parameter in the incoming request", id);
        return;
    }
    // Fetch type, action and ID
    string type = root["type"].asString();
    string name = root["name"].asString();

    // Ensure type = action
    if (type != "event") {
        sendError("Unknown request type", id);
        return;
    }

    // Handle action
    handleEvent( id, name, root["data"] );

}

/**
 * Return the next available egress packet
 */
string WebserverConnection::getEgressRawData() 
{
    // Return empty string if the queue is empty
    if (egress.empty())
        return "";

    // Pop first element
    string ans = egress.front();
    egress.pop();
    return ans;
}

/**
 * Send a raw response to the server
 */
void WebserverConnection::sendRawData( const string& data ) 
{
    // Add data to the egress queue
    egress.push(data);
}

/**
 * Send error response
 */
void WebserverConnection::sendError( const string& error, const string& id ) 
{
    // Build and send an error response
    ostringstream oss;
    oss << "{\"type\":\"error\",";
    if (!id.empty())
        oss << "\"id\":\"" << id << "\",";
    oss << "\"error\":\"" << error << "\"}";
    sendRawData( oss.str() );
}

/**
 * Send a json-formatted action response
 */
void WebserverConnection::reply( const string& id, const Json::Value& data ) 
{

    // Build and send an action response
    Json::FastWriter writer;
    Json::Value root;

    // Populate core fields
    root["type"] = "result";
    root["id"] = id;

    // Populate data
    root["data"] = data;

    // Compile JSON response
    sendRawData( writer.write(root) );
}

/**
 * Send a json-formatted action response
 */
void WebserverConnection::sendAction( const string& event, const Json::Value& data, const string& id ) 
{

    // Build and send an action response
    Json::FastWriter writer;
    Json::Value root;

    // Populate core fields
    root["type"] = "action";
    root["name"] = event;
    root["id"] = id;
    root["data"] = data;

    // Compile JSON response
    string jsonResponse = writer.write(root);
    sendRawData( jsonResponse );
}

/**
 * Check if the socket is connected
 */
bool WebserverConnection::isConnected()
{
    return true;
}

/**
 * Cleanup sequence
 */
void WebserverConnection::cleanup()
{
}
