const { NodeSpeech } = require("../src/node-speech.js");
const Speech = NodeSpeech;

const Speaker = new Speech( "es-ES" );

Speaker.speak( "Hola, esto es una prueba" );
Speaker.speak( "Mi mujer es la mas guapa del mundo" );

setTimeout(() => {
	Speaker.speak( "Mi mujer se cree siempre que le hago la pelota cuando no se la hago.");
}, 5000);

let i = 1;
while( i < 10 ) {
	setTimeout(() => {
		console.log( "OTHER TASK" );
	}, 1000 * i);
	i++;
}

Speaker.on( "tts:started", () => {
	console.log( "Started" );
} );
Speaker.on( "tts:progress", result => {
	console.log( result );
} );