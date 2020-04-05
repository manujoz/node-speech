using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Speech.Synthesis;

namespace CsNodeSpeech
{
    public partial class Speech
    {
        // Function that issues events to CPP
        public Func<string, string, string> emitEventToCpp;

        /**
         * @method  EventSpeakStarted
         * 
         * Method invoke when sinthesized start to speak
         * 
         * @param   {object}    sender          Object sent by the recognition event.
         * @paran   {object}    e               Event with the result returned by the sinthesized.
         */
        private void EventSpeakStarted(object sender, SpeakStartedEventArgs e)
        {
            EventDispatch("", "tts:started");
        }

        /**
         * @method  EventSpeakProgress
         * 
         * Method invoke while sinthesized progress
         * 
         * @param   {object}    sender          Object sent by the recognition event.
         * @paran   {object}    e               Event with the result returned by the sinthesized
         */
        private void EventSpeakProgress(object sender, SpeakProgressEventArgs e)
        {
            string data = "{\"text\":\"" + e.Text + "\",\"characterCount\":\"" + e.CharacterCount + "\",\"characterPosition\":\"" + e.CharacterPosition + "\"}";
            EventDispatch(data, "tts:progress");
        }

        /**
         * @method  EventSpeakCompleted
         * 
         * Method invoke when sinthesized end of speak
         * 
         * @param   {object}    sender          Object sent by the recognition event.
         * @paran   {object}    e               Event with the result returned by the sinthesized
         */
        private void EventSpeakCompleted(object sender, SpeakCompletedEventArgs e)
        {
            EventDispatch("", "tts:completed");
        }

        /**
         * @method  EventTrigger
         * 
         * Method invoke when sinthesized change of voice
         * 
         * @param   {object}    sender          Object sent by the recognition event.
         * @paran   {object}    e               Event with the result returned by the sinthesized
         */
        private void EventVoiceChange(object sender, VoiceChangeEventArgs e)
        {
            string data = "{\"id\":\"" + e.Voice.Id + "\",\"name\":\"" + e.Voice.Name + "\",\"gender\":\"" + e.Voice.Gender + "\",\"age\":\"" + e.Voice.Age + "\",\"culture\":\"" + e.Voice.Culture.Name + "\"}";
            EventDispatch(data, "tts:voiceChange");
        }

        /**
         * @method  EventTrigger
         * 
         * Returns the values of the events sent by the recognition engine
         * 
         * @param   {string}    data            Data of event
         * @paran   {string}    evName          Name of event
         */
        private void EventDispatch(string data, string evName)
        {
            emitEventToCpp(data, evName);
        }

    }
}
