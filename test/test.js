const { NodeSpeech } = require("../src/node-speech.js");
const Speech = NodeSpeech;

const Speaker = new Speech( "es-ES" );

Speaker.speak( "El médico medicó una receta" );

let i = 1;
while( i < 10 ) {
	if( i == 3 ) {
		setTimeout(() => {
			console.log( "PARAMOS" );
			Speaker.stop();
		}, 1000 * i);
	} else if( i == 7 ) {
		setTimeout(() => {
			console.log( "HABLAMOS DE NUEVO" );
			Speaker.speak( "Mi mujer se cree siempre que le hago la pelota cuando no se la hago.");
		}, 1000 * i);
	} else if( i == 9 ) {
		setTimeout(() => {
			console.log( "PARAMOS DEL TODO" );
			Speaker.stop();
		}, 1000 * i);
	} else {
		setTimeout(() => {
			console.log( "OTHER TASK" );
		}, 1000 * i);
	}
	i++;
}

Speaker.on( "tts:started", () => {
	console.log( "Started" );
} );
Speaker.on( "tts:progress", result => {
	console.log( result );
} );