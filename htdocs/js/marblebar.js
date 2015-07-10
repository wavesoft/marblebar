(function() {

	/**
	 * Where to find marblebar server
	 */
	var WS_ENDPOINT = "ws://127.0.0.1:15234/";

	////////////////////////////////////////////////
	// MarbleBar CORE API
	////////////////////////////////////////////////

	/**
	 * The MarbleBar interface API
	 */
	var Marblebar = function() {

		// Local properties
		this.socket = null;
		this.connected = false;

		// Event ID and callback tracking
		this.lastID = 0;
		this.responseCallbacks = {};
		this.actionHandlers = [];

		// Try to connect
		this.connect();

	};

	/**
	 * Handle raw incoming data
	 */
	Marblebar.prototype.__handleData = function( data ) {
		var o = JSON.parse(data);

		// Forward all the frames of the given ID to the
		// frame-handling callback (ex. responses)
		if (o['id']) {
			var cb = this.responseCallbacks[o['id']];
			if (cb != undefined) cb(o);
		}

		// Fire handlers if we got an action request
		else if (o['type'] == "action") {
			var data = o['data'];
			// Forward to user
			for (var i=0; i<this.actionHandlers.length; i++) {
				this.this.actionHandlers[i]( o['name'], o['data'] );
			}
		}
	}

	/**
	 * Send an event to server JSON frame
	 */
	SocketPrototype.sendEvent = function(eventName, data, responseEvents, responseTimeout) {
		var self = this;
		var timeoutTimer = null;

		// Calculate next frame's ID
		var frameID = "a-" + (++this.lastID);
		var frame = {
			'type': 'event',
			'name': eventName,
			'id': frameID,
			'data': data || { }
		};

		// Register generic reply callback
		if (typeof(responseEvents) == 'function') {

			// Register a timeout timer
			// unless the responseTimeout is set to 0
			if (responseTimeout !== 0) {
				timeoutTimer = setTimeout(function() {

					// Remove slot
					delete self.responseCallbacks[frameID];

					// Send error event
					responseEvents(null, "Response timeout");

				}, responseTimeout || 10000);
			}

			// Register a callback that will be fired when we receive
			// a frame with the specified ID.
			this.responseCallbacks[frameID] = function(data) {

				// We got a response, reset timeout timer
				if (timeoutTimer!=null) clearTimeout(timeoutTimer);

				// Wait for a result event
				if (data['type'] == 'result') {
					// Delete callback slot
					delete self.responseCallbacks[frameID];
					// Fire callback
					responseEvents(data['data']);
				}

			};

		// Register event listeners callbacks
		} else if (typeof(responseEvents) == 'object') {
			var eventify = function(name) {
					if (!name) return "";
					return "on" + name[0].toUpperCase() + name.substr(1);
				}

			// Register a timeout timer
			// unless the responseTimeout is set to 0
			if (responseTimeout !== 0) {
				timeoutTimer = setTimeout(function() {

					// Remove slot
					delete self.responseCallbacks[frameID];

					// Send error event
					if (responseEvents['onError'])
						responseEvents['onError']("Response timeout");

				}, responseTimeout || 10000);
			}

			// Register a callback that will be fired when we receive
			// a frame with the specified ID.
			this.responseCallbacks[frameID] = function(data) {

				// We got a response, reset timeout timer
				if (timeoutTimer!=null) clearTimeout(timeoutTimer);

				// Cleanup when we received a 'succeed' or 'failed' event
				if ((data['name'] == 'succeed') || (data['name'] == 'failed')) {
					delete self.responseCallbacks[frameID];
				}

				// Pick and fire the appropriate event response
				var evName = eventify(data['name']);
				if (responseEvents[evName]) {

					// Fire the function with the array list as arguments
					responseEvents[evName].apply(
							self, data['data'] || []
						);

				}

			};
		}

		// Send JSON Frame
		this.socket.send(JSON.stringify(frame));
	}

	/**
	 * Disconnect from the websocket & cleanup GUI
	 */
	Marblebar.prototype.disconnect = function() {
		if (!this.connected) return;

		// Disconnect
		this.socket.close();
		this.connected = false;
		this.socket = null;

		// Destroy GUI
		this.destroyGUI();
	}

	/**
	 * Connect to the websocket and initialize GUI
	 */
	Marblebar.prototype.connect = function( timeout ) {
		var self = this;
		try {
			// Calculate timeout
			if (!timeout) timeout=500;

			// Open websocket
			var socket = new WebSocket(WS_ENDPOINT);

			// Safari bugfix: When everything else fails
			var timedOut = false,
				timeoutCb = setTimeout(function() {
					timedOut = true;
					socket.close();
					self.socket = null;
					self.connected = false;
				}, timeout);

			// Setup websocket & callbacks
			socket.onerror = function(e) {
				console.warn("[socket] Error connecting to socket", e);
				if (timedOut) return;
				clearTimeout(timeoutCb);
				socket.close();
				self.socket = null;
				self.connected = false;
			};
			socket.onopen = function(e) {
				if (timedOut) return;
				clearTimeout(timeoutCb);
				self.socket = socket;
				self.connected = true;
				self.initGUI();
			};
			socket.onclose = function() {
				console.warn("[socket] Socket disconnected");
				self.disconnect();
			};
			socket.onmessage = function(e) {
				self.__handleData();
			};

		} catch(e) {
			console.warn("[socket] Error setting up socket", e);
			if (timedOut) return;
			clearTimeout(timeoutCb);
			socket.close();
			self.socket = null;
			self.connected = false;
		}		
	}

	////////////////////////////////////////////////
	// MarbleBar GUI API
	////////////////////////////////////////////////

	/**
	 * The MarbleBar GUI-Specific API
	 */
	var MarbleGUI = function( hostDOM ) {
		var self = this;
		Marblebar.call(this);
		this.hostDOM = $(hostDOM);

		// Bind listeners
		this.actionHandlers.push(function(action, data) {

			if (action == 'view/add') {
				self.addView( data['id'], data['specs'] );
			} else if (action == 'view/remove') {
				self.remView( data['id'] );
			} else if (action == 'view/propchange') {
				self.setViewProperty( data['id'], data['prop'], data['value'] );
			}

		});

	};

	// Subclass from MarbleBar
	MarbleGUI.prototype = Object.create( Marblebar.prototype );

	/**
	 * Add a MarbleBar View
	 */
	MarbleGUI.prototype.addView = function( id, specs ) {

	}

	/**
	 * Remove a MarbleBar View
	 */
	MarbleGUI.prototype.remView = function( id ) {

	}

	/**
	 * Set a view property value
	 */
	MarbleGUI.prototype.setViewProperty = function( id, prop, value ) {

	}

	/**
	 * Initialize MarbleBar GUI
	 */
	MarbleGUI.prototype.initGUI = function() {

		// Send a request to initialize the GUI
		this.sendEvent("init");

	}

	/**
	 * Destroy MarbleBar GUI
	 */
	MarbleGUI.prototype.destroyGUI = function() {
		this.hostDOM.empty();
	}

	////////////////////////////////////////////////
	// Initialization Hook
	////////////////////////////////////////////////
	
	// Create a marblebar GUI API instance
	window.marblebar = new MarbleGUI($("#mb-gui"));
	// Connect and initialize
	window.marblebar.connect();

})();