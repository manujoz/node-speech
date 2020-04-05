const addon = require('bindings')('node-speech');
const { parentPort } = require("worker_threads");

class WorkerTTS
{
	constructor()
	{
		// We send the addon the response function

		addon.result_function(this._get_result.bind(this));
	}

	/**
	 * @method	speak
	 * 
	 * Speak some element
	 * 
	 * @param 	{string} type Type of element that will be passed to synthesized
	 * @param 	{object} element Element to be speaked
	 * @returns	{void}
	 */
	speak( type, element )
	{
		if( type == "string" ) {
			addon.speak_from_string( element );
		}
	}

	/**
	 * @method	_get_result
	 * 
	 * Method that is called from the addon when the recognition engine returns some 
	 * event.
	 * 
	 * @param	{string}	evName		Nombre del evento al que hay que llamar
	 * @param	{string}	result		Resultado del evento del motor de reconocimiento
	 * @returns	{void}
	 */
	_get_result( evName, result )
	{
		let response = {
			evName: evName,
			result: result
		}
		
		parentPort.postMessage( response );
	}
}

const worker = new WorkerTTS();

parentPort.on( "message", response => {
	if( response.action == "speak" ) {
		worker.speak( response.type, response.element );
	}
});