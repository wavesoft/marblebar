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

#ifndef _MARBLEBAR_CALLBACKS_HPP_
#define _MARBLEBAR_CALLBACKS_HPP_

#include <string>
#include <memory>
#include <functional>
 
using namespace std;

namespace mb {

	// Forward declarations
	class Config;
	typedef std::shared_ptr<Config> 	ConfigPtr;
	typedef std::weak_ptr<Config> 		ConfigWeakPtr;

	/**
	 * Return a default config instance
	 */
	inline ConfigPtr defaultConfig()
		{ return std::make_shared<Config>(); };

	/**
	 * Configuration class
	 */
	class Config {
	public:

		/**
		 * Intiialize MarbleBar config
		 */
		Config()
			: webserverPort( 15234 )
		{ }

		/**
		 * The server version
		 */
		const string version 	= "0.0.1";

		/**
		 * The port to listen at
		 */
		int webserverPort;

	};

};


#endif /* _MARBLEBAR_CALLBACKS_HPP_ */