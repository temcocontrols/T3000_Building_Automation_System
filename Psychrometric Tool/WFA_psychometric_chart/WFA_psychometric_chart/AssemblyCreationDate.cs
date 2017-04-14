using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WFA_psychometric_chart
{
    internal static class AssemblyCreationDate
    {
        public static readonly DateTime Value;

        static AssemblyCreationDate()
        {
            Version version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
            Value = new DateTime(2000, 1, 1).AddDays(version.Build).AddSeconds(version.MinorRevision * 2);
        }
    }


}
