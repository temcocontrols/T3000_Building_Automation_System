
using System;

namespace Utils
{
  
    public class LogStub : ILog
    {
       
        public void WriteAction(string text, Log.ActTypes actType)
        {
        }

       
        public void WriteInfo(string text)
        {
        }

      
        public void WriteAction(string text)
        {
        }

     
        public void WriteError(string text)
        {
        }

      
        public void WriteException(Exception ex, string errMsg = "", params object[] args)
        {
        }

      
        public void WriteLine(string text = "")
        {
        }

       
        public void WriteBreak()
        {
        }
    }
}
