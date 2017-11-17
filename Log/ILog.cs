

using System;

namespace Utils
{
   
    public interface ILog
    {
       
        void WriteAction(string text, Log.ActTypes actType);

        void WriteInfo(string text);

        
        void WriteAction(string text);

     
        void WriteError(string text);

       
        void WriteException(Exception ex, string errMsg = "", params object[] args);

     
        void WriteLine(string text = "");

        void WriteBreak();
    }
}
