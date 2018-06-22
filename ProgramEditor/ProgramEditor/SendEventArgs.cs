using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Irony;
using Irony.Parsing;
using PRGReaderLibrary.Utilities;


namespace T3000.Forms
{

    /// <summary>
    /// Send Event Arguments
    /// Code and tokens list
    /// </summary>
    public class SendEventArgs : EventArgs
    {
        
        string codetext;
        List<EditorTokenInfo> tokenlist = new List<EditorTokenInfo>();

     

        /// <summary>
        /// Preprocessed token list, constructor
        /// Usually receives a copy of token list from ProgramEditorForm
        /// </summary>
        /// <param name="code">Copy of plain text code</param>
        /// <param name="tokens">Preprocessed Tokens List</param>
        public SendEventArgs(string code, List<EditorTokenInfo> tokens)
        {
            codetext  = code;
            tokenlist = tokens;
        }

        /// <summary>
        /// Plain text code with numbered lines
        /// </summary>
        public string Code
        {
            get { return codetext; }
            private set { codetext = value; }
        }

        /// <summary>
        /// List of Tokens
        /// </summary>
        public List<EditorTokenInfo> Tokens
        {
           get{ return tokenlist; }
           set { tokenlist = value; }
        }

        /// <summary>
        /// Send EventArgs ToString() override
        /// </summary>
        /// <returns>string concatenation of all tokens</returns>
        public override string ToString()
        {
            string result = "";
            foreach (var tok in this.tokenlist)
            {
                result += tok.ToString();
            }
            return result;
        }

    }
}