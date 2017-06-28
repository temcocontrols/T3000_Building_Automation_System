using System;
using System.Reflection;

namespace PH_App
{


    internal static class AssemblyDateGeneration
    {
        public static readonly DateTime Value;

        static AssemblyDateGeneration()
        {
            Version version = Assembly.GetExecutingAssembly().GetName().Version;
            Value = new DateTime(2000, 1, 1).AddDays(version.Build).AddSeconds(version.MinorRevision * 2);
        }
    }
}
