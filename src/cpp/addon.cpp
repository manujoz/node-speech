#include "addon.h"


void Init()
{
    TTS::synth->emitEventToCpp = gcnew System::Func<System::String^, System::String^, System::String^>(dispatchEventFromCs);
}

/*
* @function	triggerEventFromCs
*
* Function that receives the events of the recognition engine in CS.
*
* @returns	{nullptr}
*/
System::String^ dispatchEventFromCs(System::String^ data, System::String^ evName)
{
    napi_status status;
    napi_value result;
    napi_value params[2];

    // Convertimos los System::String en std::string
    string stringName = Utils::convert_from_cs_string(evName);
    string stringData = Utils::convert_from_cs_string(data);

    // Convertimos los std::string en javascript Strings
    napi_create_string_utf8(FuncResult.env, stringName.c_str(), NAPI_AUTO_LENGTH, &params[0]);
    napi_create_string_utf8(FuncResult.env, stringData.c_str(), NAPI_AUTO_LENGTH, &params[1]);

    // Obtenemos la función de la referencia
    napi_value func;
    napi_get_reference_value(FuncResult.env, FuncResult.func, &func);

    // Llamamos a la función javascript
    status = napi_call_function(FuncResult.env, FuncResult.global, func, 2, params, &result);
    assert(status == napi_ok);

    return nullptr;
}


#pragma unmanaged

/*
* @function Constructor
*
* Initialize the recognition engine with a given culture.
*
* @return   {boolean}       TRUE if is initialized, FALS if not.
*/
napi_value Constructor(napi_env env, napi_callback_info info)
{
    napi_status status;

    // Get the past culture
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    string culture = Utils::get_javascript_string(env, args[0]);
    bool state = ManagedRecognizer::_constructor(culture);

    return Utils::create_napi_boolean(env, state);
}

/*
* @function SetResultFunction
*
* Set the event broadcast to javascript
*
* @return   {nullptr}
*/
napi_value SetResultFunction(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value result;

    // Get javascript result function
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    if (argc < 1) {
        napi_throw_type_error(env, nullptr, "You must provide one argument");
        return nullptr;
    }

    // Persisnten function
    FuncResult.env = env;
    status = napi_get_global(env, &FuncResult.global);
    assert(status == napi_ok);

    napi_create_reference(env, args[0], 1, &FuncResult.func);

    return nullptr;
}

/*
* @function SetVoice
*
* Set voice to synthesized
*
* @return   {napi_boolean}
*/
napi_value SetVoice(napi_env env, napi_callback_info info)
{
    napi_status status;

    // Get the past culture
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    string name = Utils::get_javascript_string(env, args[0]);

    bool OK = ManagedRecognizer::_setVoice(name);

    return Utils::create_napi_boolean(env, OK);
}

/*
* @function GetVoices
*
* Return voices installed
*
* @return   {napi_string}
*/
napi_value GetVoices(napi_env env, napi_callback_info info)
{
    napi_status status;
    string voices = ManagedRecognizer::_getVoices();

    napi_value str;
    napi_create_string_utf8(env, voices.c_str(), NAPI_AUTO_LENGTH, &str);
    assert(status == napi_ok);

    return str;
}

/*
* @function SpeakFromString
*
* Speak a given string
*
* @return   {napi_string}
*/
napi_value SpeakFromString(napi_env env, napi_callback_info info)
{
    napi_status status;

    // Get the past culture
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    string text = Utils::get_javascript_string(env, args[0]);

    ManagedRecognizer::_speakFromString(text);

    return nullptr;
}

/*
* @function InstalledCulures
*
* Return installed cultures on devie
*
* @return   {napi_string}
*/
napi_value InstalledCulures(napi_env env, napi_callback_info info)
{
    napi_status status;
    string cultures = ManagedRecognizer::_installedCultures();

    napi_value str;
    napi_create_string_utf8(env, cultures.c_str(), NAPI_AUTO_LENGTH, &str);
    assert(status == napi_ok);

    return str;
}

/*
* @function IsInstalledCulture
*
* Return if given cultures is installed
*
* @return   {napi_string}
*/
napi_value IsInstalledCulture(napi_env env, napi_callback_info info)
{
    napi_status status;

    // Get the past culture
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    string culture = Utils::get_javascript_string(env, args[0]);
    bool installed = ManagedRecognizer::_isInstalledCulture(culture);

    return Utils::create_napi_boolean(env, installed);
}


#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value InitModule(napi_env env, napi_value exports) {
    napi_status status;

    // Managed assemblies load
    // . . . . . . . . . . . . . . . . . 

    Utils::assemblerLoads();

    // Invoke Init function
    // . . . . . . . . . . . . . . . . . 

    Init();

    // Constructor
    napi_property_descriptor constructorJS = DECLARE_NAPI_METHOD("constructorJS", Constructor);
    status = napi_define_properties(env, exports, 1, &constructorJS);
    assert(status == napi_ok);

    // Result function emitter to javascript
    napi_property_descriptor result_function = DECLARE_NAPI_METHOD("result_function", SetResultFunction);
    status = napi_define_properties(env, exports, 1, &result_function);
    assert(status == napi_ok);

    napi_property_descriptor get_voices = DECLARE_NAPI_METHOD("get_voices", GetVoices);
    status = napi_define_properties(env, exports, 1, &get_voices);
    assert(status == napi_ok);

    napi_property_descriptor set_voice = DECLARE_NAPI_METHOD("set_voice", SetVoice);
    status = napi_define_properties(env, exports, 1, &set_voice);
    assert(status == napi_ok);

    napi_property_descriptor speak_from_string = DECLARE_NAPI_METHOD("speak_from_string", SpeakFromString);
    status = napi_define_properties(env, exports, 1, &speak_from_string);
    assert(status == napi_ok);

    napi_property_descriptor installed_cultures = DECLARE_NAPI_METHOD("installed_cultures", InstalledCulures);
    status = napi_define_properties(env, exports, 1, &installed_cultures);
    assert(status == napi_ok);

    napi_property_descriptor is_installed_culture = DECLARE_NAPI_METHOD("is_installed_culture", IsInstalledCulture);
    status = napi_define_properties(env, exports, 1, &is_installed_culture);
    assert(status == napi_ok);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, InitModule)

