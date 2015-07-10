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

#include <sstream>
#include "marblebar/platform.hpp"

using namespace mb;

/**
 * Open the platform webbrowser pointing to the GUI
 */
void mb::openGUIURL( ConfigPtr config )
{

	// Calculate url to visit
	ostringstream oss;
	oss << "xdg-open \"http://127.0.0.1:" << config->webserverPort << "/gui.html\"";

	// Get URL string
	std::string url = oss.str();
	system(url.c_str());

}
