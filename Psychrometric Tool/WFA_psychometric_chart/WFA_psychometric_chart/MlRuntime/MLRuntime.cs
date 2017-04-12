using System;

namespace MLRuntime
{
    public sealed class MLRuntime
    {
        private MLRuntime()
        {
            // Private constructor prevents instantiation
        }

        public delegate void LanguageChangedDelegate();
        public static event LanguageChangedDelegate LanguageChanged;

        public static void BroadcastLanguageChanged()
        {
            LanguageChanged();
        }
    }
}
