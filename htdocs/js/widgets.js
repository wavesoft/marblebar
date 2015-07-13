/**
 * [button] A clickable Widget
 */
MarbleBar.Widgets['button'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.button = $('<button class="btn'+(specs['meta']['class'] || "")+'" id="'+id+'"></button>').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

		// Click
		this.button.click((function() {
			this.trigger("click");
		}).bind(this));

	},{

		// Update widget value
		update: function(value) {
			// Update label's contents
			this.button.html(value);
		}

	}
);

/**
 * [label] A Read-only text widget
 */
MarbleBar.Widgets['label'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.labelBody = $('<span id="'+id+'"></span>').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

	},{

		// Update widget value
		update: function(value) {
			// Update label's contents
			this.labelBody.html(value);
		}

	}
);

/**
 * [text] A Text widget rendered as an input element
 */
MarbleBar.Widgets['text'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.input = $('<input class="form-control" id="'+id+'"></input>').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

		// Register updates
		$(this.input).on("click blur keyup", (function() {
			// Trigger value update
			this.trigger("update", { "value": this.input.val() });
		}).bind(this));

	}, {

		// Update widget value
		update: function(value) {
			// Set to text field value
			this.input.attr("value", value);
		}

	}

);

/**
 * [toggle] A Togglable widget for boolean values
 */
MarbleBar.Widgets['toggle'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.btn = $('<button type="button" id="'+id+'" class="btn btn-danger">Off</button>').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

		// Register updates
		$(this.btn).on("click", (function() {

			// Toggle class
			if (this.btn.hasClass("btn-danger")) {
				this.trigger("update", { "value": true });
				this.btn
					.removeClass("btn-danger")
					.addClass("btn-success")
					.text("On");
			} else {
				this.trigger("update", { "value": false });
				this.btn
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
				this.btn
					.removeClass("btn-danger")
					.addClass("btn-success")
					.text("On");

			} else {
				this.btn
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
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.input = $('<input id="'+id+'" type="text" />').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

		// Create slider
		$(this.input).slider({
			'min': specs.meta.min || 0,
			'max': specs.meta.max || 100,
			'step': specs.meta.step || 1,
			'value': specs.value
		});
		$(this.input).on("slide", (function(slideEvt) {
			this.trigger("update", { "value": slideEvt.value });
		}).bind(this));

	}, {

		// Update widget value
		update: function(value) {
			// Set slider value
			this.input.slider('setValue', value);
		}

	}

);

/**
 * [image] An image widget for rendering images (duh!)
 */
MarbleBar.Widgets['image'] = MarbleBar.Widget.create(

	// Constructor
	function( hostDOM, specs ) {

		// Initialize widget
		var id = MarbleBar.new_id();
		this.label = $('<label class="col-sm-2 control-label" for="'+id+'"></label>').text( specs['meta']['title'] ).appendTo( hostDOM );
		this.img = $('<img class="img-thumbnail" id="'+id+'" src="" />').appendTo(
			$('<div class="col-sm-10"></div>').appendTo(hostDOM)
		);

		// Apply width/height if defined
		if (specs.meta.width >= 0)
			this.img.attr("width", specs.meta.width);
		if (specs.meta.height >= 0)
			this.img.attr("height", specs.meta.height);

	}, {

		// Update widget value
		update: function(value) {
			// Update image contents
			this.img.attr('src', value);
		}

	}

);
