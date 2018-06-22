using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace T3000.Forms
{
    /// <summary>
    /// Interface ProgramEditor
    /// Functionalities to be implemented by NewEditor
    /// </summary>
   public interface IProgramEditor
    {
         string Code { get; set; }
        string Caption { get; set; }

    }
}
