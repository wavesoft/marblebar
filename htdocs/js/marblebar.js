(function() {

	/**
	 * Where to find marbleBar server
	 */
	var WS_ENDPOINT = "ws://127.0.0.1:15234";

	////////////////////////////////////////////////
	// MarbleBar CORE API
	////////////////////////////////////////////////

	/**
	 * The MarbleBar interface API
	 */
	var MarbleBar = window.MarbleBar = function() {

		// Local properties
		this.socket = null;
		this.connected = false;

		// Event ID and callback tracking
		this.lastID = 0;
		this.responseCallbacks = {};
		this.actionHandlers = [];

	};

	/**
	 * Helper function to allocate new ID
	 */
	MarbleBar.cid = 0;
	MarbleBar.new_id = function() {
		return 'uid-'+(++MarbleBar.cid); 
	};

	/**
	 * Handle raw incoming data
	 */
	MarbleBar.prototype.__handleData = function( data ) {
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
				this.actionHandlers[i]( o['name'], o['data'] );
			}
		}
	}

	/**
	 * Send an event to server JSON frame
	 */
	MarbleBar.prototype.sendEvent = function(eventName, data, responseEvents, responseTimeout) {
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
	MarbleBar.prototype.disconnect = function() {
		// Destroy GUI
		this.destroyGUI();

		// Disconnect
		if (this.socket) {
			this.socket.close();
			this.connected = false;
			this.socket = null;
		}

	}

	/**
	 * Connect to the websocket and initialize GUI
	 */
	MarbleBar.prototype.connect = function( timeout ) {
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
				self.__handleData( e.data );
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
	// Widget Base Class
	////////////////////////////////////////////////

	/**
	 * Widget base class
	 */
	var Widget = MarbleBar.Widget = function( hostDOM, specs ) {
		this.id = "";
		this.view = null;
		this.hostDOM = hostDOM;
	};

	/**
	 * Create a new widget
	 */
	Widget.create = function( constructor, methods ) {
		
		// Create typed widget constructor
		var CustomWidget = function( hostDOM, specs ) {
			Widget.call(this, hostDOM, specs);
			constructor.call(this, hostDOM, specs);
		};

		// Subclass from widget
		CustomWidget.prototype = Object.create( Widget.prototype );

		// Register all methods on the prototype
		for (var k in methods)
			if (methods.hasOwnProperty(k))
				CustomWidget.prototype[k] = methods[k];

		// Return custom widget
		return CustomWidget;
	};

	/**
	 * Trigger a particular event
	 */
	Widget.prototype.trigger = function(event, data) {
		if (!this.view) return;

		// Forward event through the kernel
		this.view.kernel.sendEvent("property/event", {
			"view": this.view.id,
			"prop": this.id,
			"name": event,
			"data": data
		});
	}

	/**
	 * Overridable function to update widget value
	 */
	Widget.prototype.update = function(value) {
	}

	////////////////////////////////////////////////
	// Widget Repository
	////////////////////////////////////////////////

	/**
	 * The widgets repository
	 */
	var Widgets = MarbleBar.Widgets = { };

	////////////////////////////////////////////////
	// View Interface
	////////////////////////////////////////////////

	/**
	 * The View interface API
	 */
	var View = MarbleBar.View = function( kernel, hostDOM, specs ) {

		// Initialize properties
		this.id = specs.id;
		this.kernel = kernel;
		this.properties = [];
		this.propertyIndex = { };

		// Initialize tab DOMs
		var eid = MarbleBar.new_id();
		this.hostDOM = hostDOM;
		this.tabDOM = $(' <li role="presentation"><a href="#'+eid+'" aria-controls="'+eid+'" role="tab" data-toggle="tab">'+specs.meta.title+'</a></li>')
			.appendTo(hostDOM.find(".mb-gui-tabnav"));
		this.bodyDOM = $('<div role="tabpanel" class="tab-pane mb-view" id="'+eid+'"></div>')
			.appendTo(hostDOM.find(".mb-gui-tabpane"));

		// Initialize property host
		this.propertyDOM = $('<form class="form-horizontal"></form>').appendTo( this.bodyDOM );

		// Create property groups and their properties
		for (var group in specs.properties) {
			if (!specs.properties.hasOwnProperty(group)) continue;
			var props = specs.properties[group],
				panelDOM = $('<div class="panel panel-default"></div>').appendTo( this.bodyDOM ),
				headDOM = $('<div class="panel-heading"></div>').text(group).appendTo(panelDOM),
				bodyDOM = $('<div class="panel-body"></div>').appendTo(panelDOM),
				formDOM = $('<form class="form-horizontal"></form>').appendTo( bodyDOM );

			// Delete head panel if missing
			if (group == "") headDOM.remove();

			// Create properties
			for (var i=0; i<props.length; i++) {
				this.createProperty( props[i], formDOM );
			}
		}

	}

	/**
	 * Add a property in the view
	 */
	View.prototype.createProperty = function( specs, host ) {

		// Instance property from specs
		var widget_CLASS = Widgets[specs.widget || "text"],
			widgetDOM = $('<div class="form-group mb-property"></div>').appendTo( host );

		// Check for errors
		if (!widget_CLASS) {
			console.error("[widget] Unknown widget '" + specs.widget + "'");
			return;
		}

		// Create and initialize widget
		var widget = new widget_CLASS( widgetDOM, specs );
		widget.view = this;
		widget.id = specs.id;

		// Store on index
		this.propertyIndex[ specs.id ] = widget;
		this.properties.push( widget );

		// Apply value
		try {
			widget.update( specs.value );
		} catch (e) { }

	}

	/**
	 * Add a property in the view
	 */
	View.prototype.updateProperty = function( id, value ) {
		if (!this.propertyIndex[id]) return;

		// Apply value
		try {
			this.propertyIndex[id].update( value );
		} catch(e) { };
	}

	/**
	 * Erase view properties
	 */
	View.prototype.wipe = function() {
		for (var i=0; i<this.properties; i++) {

		}
		this.properties = [];
	}

	////////////////////////////////////////////////
	// MarbleBar GUI API
	////////////////////////////////////////////////

	/**
	 * The MarbleBar GUI-Specific API
	 */
	var MarbleGUI = function( hostDOM ) {
		var self = this;
		MarbleBar.call(this);

		// Initialize properties
		this.views = [];
		this.viewIndex = {};

		// Bind listeners
		this.actionHandlers.push(function(action, data) {

			if (action == 'view/add') {
				self.addView( data['id'], data );
			} else if (action == 'view/remove') {
				self.remView( data['id'] );
			} else if (action == 'view/propchange') {
				self.setViewProperty( data['id'], data['prop'], data['value'] );
			}

		});

		// Initialize DOMs
		this.hostDOM = $(hostDOM);

	};

	// Subclass from MarbleBar
	MarbleGUI.prototype = Object.create( MarbleBar.prototype );

	/**
	 * Add a MarbleBar View
	 */
	MarbleGUI.prototype.addView = function( id, specs ) {
		// Create a DOM & View instance
		var view = new View( this, this.hostDOM, specs );

		// Put on index
		this.views.push(view);
		this.viewIndex[id] = view;

		// Activate	view's tab dom if first
		if (this.views.length == 1) {
			view.tabDOM.tab('show');
			view.bodyDOM.addClass("active");
		}

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
		// Make sure we have that view and property
		if (!this.viewIndex[id]) return;
		if (!this.viewIndex[id].propertyIndex[prop]) return;
		// Apply value changes
		try {
			this.viewIndex[id].propertyIndex[prop].update( value );
		} catch (e) { };
	}

	/**
	 * Initialize MarbleBar GUI
	 */
	MarbleGUI.prototype.initGUI = function() {

		// Send a request to initialize the GUI
		this.sendEvent("ui/init");

	}

	/**
	 * Destroy MarbleBar GUI
	 */
	MarbleGUI.prototype.destroyGUI = function() {
		this.hostDOM.empty();

		// Create a blank GUI
		this.hostDOM.append(
			$('<div class="alert alert-danger" role="alert"><strong>Disconnected from application</strong>. The application might have stopped or the connection with the application was interrupted. Try refreshing and if the problem insists restart the application.</div>')
		);

		// Close window
		window.open('','_self','');
        window.close();

	}

	////////////////////////////////////////////////
	// Initialization Hook
	////////////////////////////////////////////////
	
	$(function() {

		// Create a marbleBar GUI API instance
		window.marbleBar = new MarbleGUI($("#mb-gui"));
		// Connect and initialize
		window.marbleBar.connect();

	});

})()