/**************************************************************************
*                           MIT License
* 
* Copyright (C) 2014 Morten Kvistgaard <mk@pch-engineering.dk>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Reflection;
using System.IO;
using System.Linq;

namespace TemcoStandardBacnetTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // Load Embbeded dll
            AppDomain.CurrentDomain.AssemblyResolve += (sender, args) =>
            {

                Assembly thisAssembly = Assembly.GetExecutingAssembly();

                //Get the Name of the AssemblyFile
                var name = args.Name.Substring(0, args.Name.IndexOf(',')) + ".dll";

                //Load form Embedded Resources - This Function is not called if the Assembly is in the Application Folder
                var resources = thisAssembly.GetManifestResourceNames().Where(s => s.EndsWith(name));
                if (resources.Count() > 0)
                {
                    var resourceName = resources.First();
                    using (Stream stream = thisAssembly.GetManifestResourceStream(resourceName))
                    {
                        if (stream == null) return null;
                        var block = new byte[stream.Length];
                        stream.Read(block, 0, block.Length);
                        return Assembly.Load(block);
                    }
                }
                return null;
            };
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainDialog());
        }
    }
}
