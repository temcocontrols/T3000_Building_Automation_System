

using System;
using System.IO;
using System.Web;
using System.Windows.Forms;

namespace Utils.Report
{
 
    public abstract class RepBuilder
    {
   
        public abstract string RepName { get; }

   
        public virtual string RepDescr
        {
            get
            {
                return "";
            }
        }

     
        public abstract string RepFormat { get; }

       
        public virtual string TemplateFileName
        {
            get
            {
                return "";
            }
        }

    
        public virtual object DataRef
        {
            get
            {
                return null;
            }
            set
            {
            }
        }

     
        public virtual Form WinForm
        {
            get
            {
                return null;
            }
        }

     
        public virtual string WebFormFileName
        {
            get
            {
                return "";
            }
        }


      
        public RepBuilder()
        {
        }


     
        public virtual void SetParams(params object[] repParams)
        {
        }

      
        public abstract void Make(Stream outStream, string templateDir);

 
        public virtual void Make(string outFileName, string templateDir)
        {
            Stream outStream = new FileStream(outFileName, FileMode.Create, FileAccess.Write);
            try
            {
                Make(outStream, templateDir);
            }
            finally
            {
                outStream.Close();
            }
        }


        public virtual void Generate(object[] repParams, string templateDir, string fileName, HttpResponse response)
        {
            if (response == null)
                throw new ArgumentNullException("response");

            try
            {
                response.ClearHeaders();
                response.ClearContent();

                response.ContentType = "application/octet-stream";
                if (!string.IsNullOrEmpty(fileName))
                    response.AppendHeader("Content-Disposition", "attachment;filename=\"" + fileName + "\"");

                SetParams(repParams);
                Make(response.OutputStream, templateDir);
            }
            catch
            {
                response.ClearHeaders();
                response.ClearContent();
                response.ContentType = "text/html";
                throw;
            }
        }
    }
}
