/**
 * [button] A clickable Widget
 */
MarbleBar.Widgets['button'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Create input element and append to host DOM
		this.elm = $('<button type="button" class="btn" id="'+inputID+'">Button</button>').appendTo(hostDOM);

		// Handle events
		this.elm.click((function() { this.trigger("click"); }).bind(this));
		this.elm.mousedown((function() { this.trigger("mousedown"); }).bind(this));
		this.elm.mouseup((function() { this.trigger("mouseup"); }).bind(this));

	},{

		// Update widget value
		update: function(value) {
			// Update label's contents
			this.elm.html(value);
		},

		// Update widget specifications
		updateSpecs: function(specs) {
			// Update element class
			this.elm.attr( "class", "btn "+(specs['meta']['class'] || "") );
		}

	}
);

/**
 * [label] A Read-only text widget
 */
MarbleBar.Widgets['label'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Create element
		this.elm = $('<span class="form-control" id="'+inputID+'"></span>').appendTo( hostDOM );

		// Handle events
		this.elm.click((function() { this.trigger("click"); }).bind(this));
		this.elm.mousedown((function() { this.trigger("mousedown"); }).bind(this));
		this.elm.mouseup((function() { this.trigger("mouseup"); }).bind(this));

	},{

		// Update widget value
		update: function(value) {
			// Update label's contents
			this.elm.html(value);
		},

		// Update widget specifications
		updateSpecs: function(specs) {
			// Update element class
			this.elm.attr( "class", "form-control "+(specs['meta']['class'] || "") );
		}

	}
);

/**
 * [text] A Text widget rendered as an input element
 */
MarbleBar.Widgets['text'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Create element
		this.elm = $('<input class="form-control" id="'+inputID+'"></input>').appendTo(hostDOM);

		// Register updates
		this.elm.on("click blur keyup", (function() {
			this.trigger("update", { "value": this.elm.val() });
		}).bind(this));

		// Handle events
		this.elm.click((function() { this.trigger("click"); }).bind(this));
		this.elm.mousedown((function() { this.trigger("mousedown"); }).bind(this));
		this.elm.mouseup((function() { this.trigger("mouseup"); }).bind(this));

	}, {

		// Update widget value
		update: function(value) {
			// Set to text field value
			this.elm.attr("value", value);
		},

		// Update widget specifications
		updateSpecs: function(specs) {
			// Update element class
			this.elm.attr( "class", (specs['meta']['class'] || "") );
		}

	}

);

/**
 * [toggle] A Togglable widget for boolean values
 */
MarbleBar.Widgets['toggle'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Initialize widget
		this.elm = $('<button type="button" id="'+inputID+'" class="btn btn-danger">Off</button>').appendTo(hostDOM);

		// Register updates
		this.elm.on("click", (function() {

			// Toggle class
			if (this.elm.hasClass("btn-danger")) {
				this.trigger("update", { "value": true });
				this.elm
					.removeClass("btn-danger")
					.addClass("btn-success")
					.text("On");
			} else {
				this.trigger("update", { "value": false });
				this.elm
					.removeClass("btn-success")
					.addClass("btn-danger")
					.text("Off");
			}

		}).bind(this));

	}, {

		// Update widget value
		update: function(value) {
			// Set or unset boolean field
			if (value) {
				this.elm
					.removeClass("btn-danger")
					.addClass("btn-success")
					.text("On");

			} else {
				this.elm
					.removeClass("btn-success")
					.addClass("btn-danger")
					.text("Off");
			}
		}

	}

);

/**
 * [slider] A slider widget for integer values
 */
MarbleBar.Widgets['slider'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Initialize widget
		this.elm = $('<input id="'+inputID+'" type="text" />').appendTo(hostDOM);

		// Bind events
		this.elm.on("slide", (function(slideEvt) {
			this.trigger("update", { "value": slideEvt.value });
		}).bind(this));

	}, {

		// Update widget value
		update: function(value) {
			// Set slider value
			this.elm.slider('setValue', value);
		},

		// Update widget specifications
		updateSpecs: function(specs) {
			// Create slider
			this.elm.slider({
				'min': specs.meta.min || 0,
				'max': specs.meta.max || 100,
				'step': specs.meta.step || 1,
				'value': specs.value
			});
		}

	}

);

/**
 * [image] An image widget for rendering images (duh!)
 */
MarbleBar.Widgets['image'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, inputID ) {

		// Initialize widget
		this.elm = $('<img class="img-thumbnail" id="'+inputID+'" src="" />').appendTo(hostDOM);

	}, {

		// Update widget value
		update: function(value) {
			// Update image contents
			this.elm.attr('src', value);
		},

		// Update widget specifications
		updateSpecs: function(specs) {
			// Apply width/height if defined
			if (specs.meta.width >= 0) {
				this.elm.attr("width", specs.meta.width);
			} else {
				this.elm.removeAttr("width");
			}
			if (specs.meta.height >= 0) {
				this.elm.attr("height", specs.meta.height);
			} else {
				this.elm.removeAttr("height");
			}
		}

	}

);
