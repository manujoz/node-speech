#pragma once

#include "includes.h"
#include "globals.h"
#include "utils.h"


namespace ManagedRecognizer
{
    bool    _constructor(string culture);
    string  _getVoices();
    bool    _setVoice(string name);
    void    _speakFromString(string text);
    string  _installedCultures();
    bool    _isInstalledCulture(string culture);
}
