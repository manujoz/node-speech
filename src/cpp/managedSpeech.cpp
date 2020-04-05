#include "managedSpeech.h"


namespace ManagedRecognizer
{
	bool _constructor(string culture)
	{
		System::String^ cult = Utils::convert_to_cs_string(culture);

		bool state = TTS::synth->Init(cult);
		return state;
	}

	string _getVoices()
	{
		return Utils::convert_from_cs_string(TTS::synth->GetVoices());
	}

	bool _setVoice(string name)
	{
		bool OK = TTS::synth->SetVoice(Utils::convert_to_cs_string(name));
		if (!OK)
		{
			return false;
		}

		return true;
	}

	void _speakFromString(string text)
	{
		TTS::synth->SpeakFromString(Utils::convert_to_cs_string(text));
	}
	
	string _installedCultures()
	{
		return Utils::convert_from_cs_string(TTS::synth->InstalledCulures());
	}

	bool _isInstalledCulture(string culture)
	{
		return TTS::synth->IsInstalledCulture(Utils::convert_to_cs_string(culture));
	}

}