const path = require("path");
const addon = require('bindings')('node-speech');
const { Worker } = require("worker_threads");
const events = require( "events" ).EventEmitter;

class NodeSpeech extends events 
{
	constructor( culture = "" )
	{
		super();

		this._worker = null;
		this._setedFunction = false;
		this._isConstructed = true;

		if( addon.constructorJS( culture )) {
			this._init();
		} else {
			this._isConstructed = false;
			console.error( "[node-speech]: Culture [" + culture + "] is not installed on the device.");
		}

	}

	/**
	 * @method	speak
	 * 
	 * Send to synthesid a element to be speaked
	 * 
	 * @param 	{string} 		element 		Element to be speaked
	 * @returns	{void}
	 */
	speak( element ) 
	{
		if( !this._isConstructed || !this._worker ) {
			console.error( "[node-speech]: Addon is not instantiated" );
			return;
		}
		
		if( typeof element != "string" ) {
			console.error( "[node-speech]: You only can pass a string to the synthesized." );
			return;
		}

		this._worker.postMessage({
			action: "speak",
			type: typeof element,
			element: element
		});
	}

	/**
	 * @method	get_voices
	 * 
	 * Get the voices installed for the culture initialized on addon
	 * 
	 * @returns	{array}			Array of objects
	 */
	get_voices()
	{
		if( !this._isConstructed) {
			console.error( "[node-speech]: Addon is not instantiated" );
			return;
		}
		return JSON.parse( addon.get_voices() );
	}

	/**
	 * @method	set_voice
	 * 
	 * Set the voice to sinthesized by passed name
	 * 
	 * @returns	{void}
	 */
	set_voice( name )
	{
		if( !this._isConstructed ) {
			console.error( "[node-speech]: Addon is not instantiated" );
			return;
		}

		addon.set_voice( name );
	}

	/**
	 * @method	installed_cultures
	 * 
	 * Return an array width installed cultures installed on device
	 * 
	 * @returns	{array}
	 */
	installed_cultures()
	{
		return addon.installed_cultures();
	}

	/**
	 * @method	set_voice
	 * 
	 * Get is a passed culture is installed on device
	 * 
	 * @returns	{void}
	 */
	is_installed_culture( culture )
	{
		if( !culture.match(/[a-z]\-[A-Z]/g) ){
			console.error( "[node-speech]: The passed culture do not have correct format (xx-XX)" );
			return;
		}
		return addon.is_installed_culture( culture );
	}

	/**
	 * @method	_init
	 * 
	 * Initialize the addon
	 * 
	 * @returns	{void}
	 */
	_init()
	{
		// We stop if the addon has not been built

		if( !this._isConstructed ) {
			console.error( "[node-speech]: Addon is not instantiated" );
			return;
		}

		// Init thread
		
		this._worker = new Worker( path.resolve( __dirname , "worker.js" ) );
		this._worker.on( "message", response => {
			this._get_result( response.evName, response.result );
		});

		this._worker.on( "error", error => {
			console.error( "[node-speech]: ", error )
		});

		this._worker.on( "exit", () => {
			
		});

	}

	/**
	 * @method	_get_result
	 * 
	 * Method that is called from the addon when the recognition engine returns some
	 * event.
	 * 
	 * @param	{string}	evName		Name of the event to call.
	 * @param	{string}	result		Recognition engine event result
	 * @returns	{void}
	 */
	_get_result( evName, result )
	{
		if( evName == "tts:started" ){
			this._started(result);
		} else if( evName == "tts:progress" ){
			this._progress(result);
		} else if( evName == "tts:completed" ){
			this._completed(result);
		} else if( evName == "tts:voiceChange" ){
			this._voideChange(result);
		} else if( evName == "tts:error" ){
			this._error_addon(result);
		}
	}

	/**
	 * @method	_started
	 * 
	 * Handles the started event returned by the addon.
	 * 
	 * @param 	{string} 	error 			Error returned by the addon.
	 * @returns	{void}
	 */
	_started( result ) 
	{
		this.emit("tts:started");
	}

	/**
	 * @method	_progress
	 * 
	 * Handles the progress event returned by the addon.
	 * 
	 * @param 	{string} 	error 			Error returned by the addon.
	 * @returns	{void}
	 */
	_progress( result ) 
	{
		let response = JSON.parse( result );
		this.emit("tts:progress", response );
	}

	/**
	 * @method	_completed
	 * 
	 * // NOTE: Only for async speak. Not implemented in C#
	 * Handles the completed event returned by the addon.
	 * 
	 * @param 	{string} 	error 			Error returned by the addon.
	 * @returns	{void}
	 */
	_completed( result ) 
	{
		this.emit("tts:completed");
	}

	/**
	 * @method	_voideChange
	 * 
	 * Handles the voice change event returned by the addon.
	 * 
	 * @param 	{string} 	error 			Error returned by the addon.
	 * @returns	{void}
	 */
	_voideChange( result ) 
	{
		let response = JSON.parse( result );
		this.emit("tts:voiceChange", response );
	}

	/**
	 * @method	_error_addon
	 * 
	 * Handles the errors returned by the addon.
	 * 
	 * @param 	{string} 	error 			Error returned by the addon.
	 * @returns	{void}
	 */
	_error_addon( error )
	{
		console.error( "[node-speech]: " + error );
	}
}

module.exports.NodeSpeech = NodeSpeech;