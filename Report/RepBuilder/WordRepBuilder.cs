
 */

using System;
using System.IO;
using System.Xml;

namespace Utils.Report
{
    
    public abstract class WordRepBuilder : RepBuilder
    {
       
        protected const string BrPref = "w";
     
        protected const string BrName = "br";
       
        protected const string ElemName = "w:t";
      
        protected const int RowTreeHeight = 4;


        protected XmlDocument xmlDoc;


     
        public WordRepBuilder()
            : base()
        {
            xmlDoc = null;
        }


     
        public override string RepFormat
        {
            get
            {
                return "WordprocessingML";
            }
        }


      
        protected bool FindAttr(string str, string attrName, out string attrVal)
        {
            // "attrName=attrVal", вместо '=' может быть любой символ
            attrVal = "";
            if (str.StartsWith(attrName))
            {
                int start = attrName.Length + 1;
                if (start < str.Length)
                {
                    int end = str.IndexOf(" ", start);
                    if (end < 0)
                        end = str.IndexOf("x", start, StringComparison.OrdinalIgnoreCase);
                    if (end < 0)
                        end = str.Length;
                    attrVal = str.Substring(start, end - start);
                }
                return true;
            }
            else
                return false;
        }

        protected bool GetTreeNodes(XmlNode xmlNode, out XmlNode rowNode, out XmlNode tblNode)
        {
            try
            {
                rowNode = xmlNode;
                for (int i = 0; i < RowTreeHeight; i++)
                    rowNode = rowNode.ParentNode;
                tblNode = rowNode.ParentNode;
                return true;
            }
            catch
            {
                rowNode = null;
                tblNode = null;
                return false;
            }
        }

   
        protected void SetNodeTextWithBreak(XmlNode xmlNode, string text, string textBreak)
        {
            XmlNode parNode = xmlNode.ParentNode;
            if (parNode == null)
                throw new Exception("Parent XML element is missing.");

       
            parNode.RemoveChild(xmlNode);
            XmlNode cloneNode = xmlNode.Clone();

            if (text == null) text = "";
            string uri = parNode.NamespaceURI;
            int breakLen = textBreak.Length;

            do
            {
              
                int breakPos = text.IndexOf(textBreak);
                bool haveBreak = breakPos >= 0;
                string line = haveBreak ? text.Substring(0, breakPos) : text;

          
                XmlNode newNode = cloneNode.Clone();
                newNode.InnerText = line;
                parNode.AppendChild(newNode);

           
                if (haveBreak)
                    parNode.AppendChild(xmlDoc.CreateElement(BrPref, BrName, uri));

         
                text = haveBreak && breakPos + breakLen < text.Length ?
                    text.Substring(breakPos + breakLen) : "";
            } while (text != "");
        }


        protected void SetNodeTextWithBreak(XmlNode xmlNode, object text, string textBreak)
        {
            string textStr = text == null || text.ToString() == "" ? " " : text.ToString();
            SetNodeTextWithBreak(xmlNode, textStr, textBreak);
        }

        protected void SetNodeTextWithBreak(XmlNode xmlNode, string text)
        {
            SetNodeTextWithBreak(xmlNode, text, "\n");
        }

        
        protected void SetNodeTextWithBreak(XmlNode xmlNode, object text)
        {
            SetNodeTextWithBreak(xmlNode, text, "\n");
        }



        protected virtual void StartXmlDocProc()
        {
        }

      
        protected virtual void XmlDocProc(XmlNode xmlNode)
        {
            if (xmlNode.Name == ElemName)
            {
               
                string nodeVal = xmlNode.InnerText;
                string attrVal;
                if (FindAttr(nodeVal, "repRow", out attrVal))
                {
                    if (nodeVal.Length < 8  + attrVal.Length)
                        xmlNode.InnerText = "";
                    else
                        xmlNode.InnerText = nodeVal.Substring(8 + attrVal.Length);
                    ProcRow(xmlNode, attrVal);
                }
                else if (FindAttr(nodeVal, "repVal", out attrVal))
                    ProcVal(xmlNode, attrVal);
            }
            else
            {
                
                XmlNodeList children = xmlNode.ChildNodes;
                foreach (XmlNode node in children)
                    XmlDocProc(node);
            }
        }
 
        protected virtual void FinalXmlDocProc()
        {
        }

 
        protected virtual void ProcVal(XmlNode xmlNode, string valName)
        {
        }

        
        protected virtual void ProcRow(XmlNode xmlNode, string rowName)
        {
        }


        
        public override void Make(Stream outStream, string templateDir)
        {
            
            string templFileName = templateDir + TemplateFileName;

            
            xmlDoc = new XmlDocument();
            xmlDoc.Load(templFileName);

            
            StartXmlDocProc();
            XmlDocProc(xmlDoc.DocumentElement);
            FinalXmlDocProc();
 
            xmlDoc.Save(outStream);
        }
    }
}
