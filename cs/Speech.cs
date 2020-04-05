using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;
using System.Speech.Synthesis;
using System.Speech.Recognition;
using System.Web.Script.Serialization;

namespace CsNodeSpeech
{
    public partial class Speech
    {
        // Speech Synthesizer
        public SpeechSynthesizer Synth;

        // Voice to speak
        private string Voice;
        private string Culture;

        // Lista de voces instaladas
        private List<Voice> Voices = new List<Voice>();

        // Web extensions
        private JavaScriptSerializer JSON = new JavaScriptSerializer();

        public Speech()
        {
            
        }

        /**
         * @methos  Init
         * 
         * Initialize de synthesized
         * 
         * @returns {void}
         */
        public bool Init(string culture)
        {
            bool installed = IsInstalledCulture(culture);

            if( installed )
            {
                // Initialize the synthetised
                Synth = new SpeechSynthesizer();
                Synth.SetOutputToDefaultAudioDevice();
                Culture = culture;

                Synth.SpeakStarted += new EventHandler<SpeakStartedEventArgs>(EventSpeakStarted);
                Synth.SpeakProgress += new EventHandler<SpeakProgressEventArgs>(EventSpeakProgress);
                Synth.SpeakCompleted += new EventHandler<SpeakCompletedEventArgs>(EventSpeakCompleted);
                Synth.VoiceChange += new EventHandler<VoiceChangeEventArgs>(EventVoiceChange);

                // Get avalaible voices
                GetVoices(culture);
            } else
            {
                emitEventToCpp("Culture " + culture + " is not installed on system.", "tts:error");
                return false;
            }

            return true;
        }

        /**
         * @method  SpeakFromString
         * 
         * Speak a text given in a string
         * 
         * @params  {string}    text        Text to speak
         * @returns {void}
         */
        public void SpeakFromString(string text)
        {
            if(text == null)
            {
                return;
            }

            if(Voice == null)
            {
                SetDefaultVoice();
            }

            if(Voice == null)
            {
                emitEventToCpp("You don't have installed voices compatible with the synth culture (" + Culture + ")", "tts:error");
            }

            Synth.Speak(text);
        }

        /**
         * @method  SetVoice
         * 
         * Set a voice for synthesized
         * 
         * @params  {string}    name        Name of voice
         * @returns {void}
         */
        public bool SetVoice(string name)
        {
            if( IsVoiceInstalled(name) )
            {
                Synth.SelectVoice(name);
                return true;
            }

            emitEventToCpp("Voice " + name + " is not installed on system.", "tts:error");
            return false;
        }

        /**
         * @method  GetVoices
         * 
         * Return string JSON with installed voices on device
         * 
         * @returns {string}    JSON string
         */
        public string GetVoices()
        {
            return JSON.Serialize(Voices);
        }

        /**
         * @method  GetActiveVoice
         * 
         * Return active voice in sinthesized
         * 
         * @returns {string}    JSON string
         */
        public string GetActiveVoice()
        {
            foreach(Voice v in Voices)
            {
                if(v.Name == Voice && v.Culture == Culture)
                {
                    return JSON.Serialize(v);
                }
            }

            return "";
        }

        /**
         * @method  GetActiveVoice
         * 
         * Return serialized List with installed cultures
         * 
         * @returns {string}    JSON string
         */
        public string InstalledCulures()
        {
            List<string> installeds = new List<string>();
            foreach (RecognizerInfo installed in SpeechRecognitionEngine.InstalledRecognizers())
            {
                installeds.Add(installed.Culture.Name.ToString());
            }

            return JSON.Serialize(installeds);
        }

        /**
         * @method  IsInstalledCulture
         * 
         * Return if a culture is installed on device
         * 
         * @params  {string}    culture         String with culture that we wnat to verify
         * @returns {boolean}
         */
        public bool IsInstalledCulture(string culture)
        {
            foreach (RecognizerInfo installed in SpeechRecognitionEngine.InstalledRecognizers())
            {
                if (installed.Culture.Name.ToString() == culture)
                {
                    return true;
                }
            }

            return false;
        }

        /**
         * @method
         * 
         * Get installed voices
         * 
         * @params  {string}    culture         Culture string that we want the voices. Ex.: "en-US"
         * @returns {void}
         */
        private void GetVoices(string culture)
        {
            foreach (InstalledVoice v in Synth.GetInstalledVoices())
            {
                VoiceInfo info = v.VoiceInfo;

                Voice VoiceObj = new Voice();
                VoiceObj.Id = info.Id;
                VoiceObj.Name = info.Name;
                VoiceObj.Gender = info.Gender.ToString();
                VoiceObj.Age = info.Age.ToString();
                VoiceObj.Culture = info.Culture.Name;

                if (culture == null || culture == VoiceObj.Culture)
                {
                    Voices.Add(VoiceObj);
                }
            }

        }

        /**
         * @method  IsVoiceInstalled
         * 
         * Return if a specific voice is installed
         * 
         * @returns {voolean}
         */
        private bool IsVoiceInstalled(string name)
        {
            if( Voices.Count == 0 )
            {
                return false;
            }

            foreach( Voice v in Voices)
            {
                if( v.Name == name && v.Culture == Culture )
                {
                    return true;
                }
            }

            return false;
        }

        /**
         * @method  SetDefaultVoice
         * 
         * Select a default voice
         * 
         * @returns {void}
         */
        private void SetDefaultVoice()
        {
            foreach (Voice v in Voices)
            {
                if (v.Culture == Culture)
                {
                    SetVoice(v.Name);
                    Voice = v.Name;
                    break;
                }
            }
        }
    }
}
