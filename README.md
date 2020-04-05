# Node Speech

English documentatio will be write soon.

## Documentación en español

Sintetizador de texto a voz que funciona sin necesidad de navegador ni otro complemente. Es completamente nativo y depende de las voces y culturas instaldas en Windows 10. Se ejecuta en un hilo separado por lo que no detiene la ejecución de la aplicación mientras está hablado el sintentizador y no necesita que la aplicación esté en primer planto para funcionar.

## Build

El paquete viene compilado para la versión de node.js 12.16.0, es posible hacer reconstrucciones para otras versiones de node, pero no se garantiza un correcto funcioamiento para versiones anteriores anteriores a la 12 ya que el módulo hace uso de _Worker Threads_ que está disponible solo a partir de la versión 11.7.0 de node.js y otros complementos que solo están disponibles en las versiones más modernas de nodejs.

Para construir el paquete para node, tan solo ejecutar el siguiente comando después de instalar las dependencias de desarrollo:

```shell
$ npm run rebuild
```

Si se quiere hacer uso del paquete en algún entorno de ejecución distinto como por ejemplo NWJS o Electrón hay dos maneras de compilar el módulo. La primera es editar el _package.json_ y modificar las siguientes líneas para ajustarlo a tu versión de NWJS o Electrón:

```javascript
"scripts": {
    "test": "node test/test.js",
    "clean": "cmake-js clean",
    "build": "cmake-js build",
    "rebuild": "cmake-js rebuild",
    "build-nw": "cmake-js build --runtime=nw --runtime-version=<<YOUR-NW-VERSIONI>> --arch=x64",
    "rebuild-nw": "cmake-js rebuild --runtime=nw --runtime-version=<<YOUR-NW-VERSIONI>> --arch=x64",
    "build-electron": "cmake-js build --runtime=electron --runtime-version=<<YOUR-ELECTRON-VERSIONI>> --arch=x64",
    "rebuild-electron": "cmake-js build --runtime=electron --runtime-version=<<YOUR-ELECTRON-VERSIONI>> --arch=x64"
  }
```

Reemplazar el texto *\<\<YOUR-XX-VERSION>>* por la versión de NWJS o electrón en la que se vaya a compilar el módulo y luego ejecutar el script correspondiente en función de donde estés compilando el módulo

```shell
# Para NWJS
$ npm run rebuild-nw

# Para Electrón
$ npm run rebuild-electron
```

También puedes realizar una compilación manual más específicas siguiendo las instrucciones de <a href="https://github.com/cmake-js/cmake-js">cmake-js</a>.

### Iniciar el sintetizador

Es necesario instanciar el reconocedor para una cultura concreta que esté instalada en el sistema operatvio, para hacerlo hay que instanciar el reconocedor con una de las culturas aceptadas.

```javascript
const tts = require("node-speech");
const Synth = new tts( "es-ES" );
```

Las culturas disponibles con reconocimiento de voz en Windows 10 son:

- Español - España: (es-ES)
- Español - México: (es-MX)
- Inglés - EEUU: (en-US)
- Inglés - Reino Unido: (en-GB)
- Inglés - Canadá: (en-CA)
- Inglés - Australiz: (en-AU)
- Inglés - India: (en-IN)
- Francés - Francia: (fr-FR)
- Francés - Canadá: (fr-CA)
- Alemán - Alemania: (de-DE)
- Italiano - Italia: (it-IT)
- Portugués - Brasil: (pt-BR)
- Japonés - Japón: (ja-JP)
- Chino - China (zh-CN)
- Chino - Taiwan (zh-TW)
- Chino - Hong Kong (zh-HK)

Puedes instalar las diferentes culturas en Configuración -> Hora e idioma -> Idioma

### Obtener culturas instaladas

Se pueden obtener las culturas instaladas en el sistema operativo con el siguiente método:

```javascript
let cultures = Synth.installed_cultures()
```

Esto devolverá un array con las diferentes culturas instaladas ej.: ["es-ES","en-US","en-GB"]

### Obtener voces instaladsa

Obtiene las voces instaladas en el sistema operativo y devuelve un array con los datos de éstas.

```javascript
Synth.get_voices();
```

### Asignar una voz al sinteizador

Asigna una de las voces instaladas al sintetizador a través del nombre de la voz. Puedes obetner el nombre de las voces con el método __get_voices__


```javascript
Synth.set_voice( "Nombre de la voz" );
```

### Hablar

Haz que el sintetizador hable con el método __speak__

```javascript
Synth.speak( "Di esto en voz alta." );
```

### Escucha de eventos

El reconocedor enviará enventos cuando el motor realice tareas como devolver un texto reconocido o enviar un texto que no ha reconocido en la gramática.

Los eventos que están disponible para la escucha son:

#### tts:started

Este evento ocurre cuando el el sintetizador empieza a leer un texto.

```javascript
Synth.on( "tts:started", () => {
	console.log( "Start to speak" );
});
```

#### tts:progress

Devuelve el resultado con el progreso del texto reconocido.

```javascript
Synth.on( "tts:progress", result => {
	console.log( result );
});
```

#### tts:voiceChange

Devuelve este evento cuando se cambia la coz del sintetizadoe

```javascript
Synth.on( "tts:voiceChange", result => {
	console.log( result );
})
```
_!**NOTA**! Poco a poco iré implementando más funcionalidades hasta entonces es completamente funcional con estas caracterásticas descritas._





