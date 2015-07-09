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

#ifndef _MARBLEBAR_KERNEL_HPP_
#define _MARBLEBAR_KERNEL_HPP_

#include <memory>
#include <map>
#include <vector>
#include <marblebar/config.hpp>
#include <marblebar/view.hpp>
#include <marblebar/server/webserver.hpp>

using namespace std;

namespace mb {

	// Forward declarations
	class Kernel;
	typedef std::shared_ptr<Kernel> 	KernelPtr;
	typedef std::weak_ptr<Kernel> 		KernelWeakPtr;

	/**
	 * Configuration class
	 */
	class Kernel : private enable_shared_from_this<Kernel>, public Webserver {
	public:

		/**
		 * Initialize the MarbleBar kernel
		 */
		Kernel( ConfigPtr config );

		/**
		 * Add a view and return a chaining instance
		 */
		KernelPtr addView( ViewPtr config );

	protected:

		/**
		 * Create a new instance of the WebserverConnection
		 */
		virtual WebserverConnectionPtr openConnection( const std::string& domain, const std::string uri );

	private:

		/**
		 * Kernel configuration
		 */
		ConfigPtr 					config;

		/**
		 * Views registered in the kernel
		 */
		vector< ViewPtr >			views;

		/**
		 * Kernel state 
		 */
		map< string, string >		state;

	};

};


#endif /* _MARBLEBAR_KERNEL_HPP_ */