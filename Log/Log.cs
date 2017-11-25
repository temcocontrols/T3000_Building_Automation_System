

using System;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;

namespace Utils
{

    public class Log : ILog
    {
        public enum ActTypes
        {
         
            Information,
            
            Action,
           
            Error,
           
            Exception,
        }

     
        public enum Formats
        {
            
            Simple,
           
            Full
        }

        public delegate void WriteLineDelegate(string text);
     
        public delegate void WriteActionDelegate(string text, ActTypes actType);

       
        public const int DefCapacity = 1048576;

        private readonly Formats format;   // формат
        private readonly object writeLock; // объект для синхронизации обращения к журналу из разных потоков
        private StreamWriter writer;       // объект для записи в файл
        private FileInfo fileInfo;         // информация о файле


       
        protected Log()
        {
            format = Formats.Simple;
            writeLock = new object();
            writer = null;
            fileInfo = null;

            FileName = "";
            Encoding = Encoding.Default;
            Capacity = DefCapacity;
            CompName = Environment.MachineName;
            UserName = Environment.UserName;
            Break = new string('-', 80);
            DateTimeFormat = "yyyy'-'MM'-'dd' 'HH':'mm':'ss";
        }

  
        public Log(Formats logFormat)
            : this()
        {
            format = logFormat;
        }


    
        public string FileName { get; set; }

       
        public Encoding Encoding { get; set; }

      
        public int Capacity { get; set; }

       
        public string CompName { get; private set; }

       
        public string UserName { get; private set; }

     
        public string Break { get; set; }

       
        public string DateTimeFormat { get; set; }


     
        protected void Open()
        {
            try
            {
                writer = new StreamWriter(FileName, true, Encoding);
                fileInfo = new FileInfo(FileName);
            }
            catch
            {
            }
        }

     
        protected void Close()
        {
            try
            {
                if (writer != null)
                {
                    writer.Close();
                    writer = null;
                }
            }
            catch
            {
            }
        }

      
        protected string ActTypeToStr(ActTypes actType)
        {
            switch (actType)
            {
                case ActTypes.Exception:
                    return "EXC";
                case ActTypes.Error:
                    return "ERR";
                case ActTypes.Action:
                    return "ACT";
                default: // ActTypes.Information:
                    return "INF";
            }
        }


      
        public void WriteAction(string text, ActTypes actType)
        {
            StringBuilder sb = new StringBuilder(DateTime.Now.ToString(DateTimeFormat));

            if (format == Formats.Simple)
            {
                WriteLine(sb.Append(" ").Append(text).ToString());
            }
            else
            {
                WriteLine(sb.Append(" <")
                    .Append(CompName).Append("><")
                    .Append(UserName).Append("><")
                    .Append(ActTypeToStr(actType)).Append("> ")
                    .Append(text).ToString());
            }
        }

       
        public void WriteInfo(string text)
        {
            WriteAction(text, ActTypes.Information);
        }

       
        public void WriteAction(string text)
        {
            WriteAction(text, ActTypes.Action);
        }

 
        public void WriteError(string text)
        {
            WriteAction(text, ActTypes.Error);
        }

     
        public void WriteException(Exception ex, string errMsg = "", params object[] args)
        {
            if (string.IsNullOrEmpty(errMsg))
            {
                WriteAction(ex.ToString(), ActTypes.Exception);
            }
            else
            {
                WriteAction(new StringBuilder()
                    .Append(args == null || args.Length == 0 ? errMsg : string.Format(errMsg, args))
                    .Append(":").Append(Environment.NewLine)
                    .Append(ex.ToString()).ToString(), 
                    ActTypes.Exception);
            }
        }

     
        public void WriteLine(string text = "")
        {
            try
            {
                Monitor.Enter(writeLock);
                Open();
                if (fileInfo.Length > Capacity)
                {
                    string bakName = FileName + ".bak";
                    writer.Close();
                    File.Delete(bakName);
                    File.Move(FileName, bakName);

                    writer = new StreamWriter(FileName, true, Encoding);
                }
                writer.WriteLine(text);
                writer.Flush();
            }
            catch
            {
            }
            finally
            {
                Close();
                Monitor.Exit(writeLock);
            }
        }

        public void WriteBreak()
        {
            WriteLine(Break);
        }
    }
}