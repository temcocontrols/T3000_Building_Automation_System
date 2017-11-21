 

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Xml;

namespace Utils.Report
{
    /// <summary>
    /// The base class for building reports in SpreadsheetML (Microsoft Excel 2003) format
    
    /// </summary>
    public abstract class ExcelRepBuilder : RepBuilder
    {
        /// <summary>
        ///  
        /// </summary>
        protected static class XmlNamespaces
        {
            /// <summary>
            ///  
            /// </summary>
            public const string noprefix = "urn:schemas-microsoft-com:office:spreadsheet";
            /// <summary>
            /// 
            /// </summary>
            public const string o = "urn:schemas-microsoft-com:office:office";
            /// <summary>
            /// 
            /// </summary>
            public const string x = "urn:schemas-microsoft-com:office:excel";
            /// <summary>
            ///  
            /// </summary>
            public const string ss = "urn:schemas-microsoft-com:office:spreadsheet";
            /// <summary>
            ///  
            /// </summary>
            public const string html = "http://www.w3.org/TR/REC-html40";
        }

        /// <summary>
        /// Книга Excel
        /// </summary>
        protected class Workbook
        {
            /// <summary>
            /// Ссылка на XML-узел, соответствующий книге Excel
            /// </summary>
            protected XmlNode node;
            /// <summary>
            ///  
            /// </summary>
            protected XmlNode stylesNode;
            /// <summary>
            ///  
            /// </summary>
            protected SortedList<string, Style> styles;
            /// <summary>
            ///  
            /// </summary>
            protected List<Worksheet> worksheets;


            /// <summary>
            /// Конструктор
            /// </summary>
            protected Workbook()
            {
            }

           
            public Workbook(XmlNode xmlNode)
            {
                node = xmlNode;
                styles = new SortedList<string, Style>();
                worksheets = new List<Worksheet>();
            }

 
            public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

            
            public XmlNode StylesNode
            {
                get
                {
                    return stylesNode;
                }
                set
                {
                    stylesNode = value;
                }
            }

           
            public SortedList<string, Style> Styles
            {
                get
                {
                    return styles;
                }
            }

           
            public List<Worksheet> Worksheets
            {
                get
                {
                    return worksheets;
                }
            }


           
            public void AppendStyle(Style style)
            {
                styles.Add(style.ID, style);
                stylesNode.AppendChild(style.Node);
            }

           
            public void RemoveStyle(int listIndex)
            {
                Style style = styles.Values[listIndex];
                stylesNode.RemoveChild(style.Node);
                styles.RemoveAt(listIndex);
            }

         
            public Worksheet FindWorksheet(string worksheetName)
            {
                foreach (Worksheet worksheet in worksheets)
                    if (worksheet.Name.Equals(worksheetName, StringComparison.OrdinalIgnoreCase))
                        return worksheet;

                return null;
            }

         
            public void AppendWorksheet(Worksheet worksheet)
            {
                worksheets.Add(worksheet);
                node.AppendChild(worksheet.Node);
            }

            
            public void InsertWorksheet(int listIndex, Worksheet worksheet)
            {
                worksheets.Insert(listIndex, worksheet);

                if (worksheets.Count == 1)
                    node.AppendChild(worksheet.Node);
                else if (listIndex == 0)
                    node.PrependChild(worksheet.Node);
                else
                    node.InsertAfter(worksheet.Node, worksheets[listIndex - 1].Node);
            }
 
            public void RemoveWorksheet(int listIndex)
            {
                Worksheet worksheet = worksheets[listIndex];
                node.RemoveChild(worksheet.Node);
                worksheets.RemoveAt(listIndex);
            }


          
            public void SetColor(XmlNode targetNode, string color)
            {
                XmlDocument xmlDoc = targetNode.OwnerDocument;
                string namespaceURI = targetNode.NamespaceURI;

                XmlAttribute styleAttr = targetNode.Attributes["ss:StyleID"];
                if (styleAttr == null)
                {
                    styleAttr = xmlDoc.CreateAttribute("ss:StyleID");
                    targetNode.Attributes.Append(styleAttr);
                }

                string oldStyleID = styleAttr == null ? "" : styleAttr.Value;
                string newStyleID = oldStyleID + "_" + color;

                if (styles.ContainsKey(newStyleID))
                {
                    
                    styleAttr.Value = newStyleID;
                }
                else
                {
                     
                    Style newStyle;
                    if (styleAttr == null)
                    {
                        XmlNode newStyleNode = xmlDoc.CreateNode(XmlNodeType.Element, "Style", namespaceURI);
                        newStyleNode.Attributes.Append(xmlDoc.CreateAttribute("ss", "ID", namespaceURI));
                        newStyle = new Style(newStyleNode);
                    }
                    else
                        newStyle = styles[oldStyleID].Clone();
                    newStyle.ID = newStyleID;

                   
                    XmlNode interNode = newStyle.Node.FirstChild;
                    while (interNode != null && interNode.Name != "Interior")
                        interNode = interNode.NextSibling;

                    if (interNode == null)
                    {
                        interNode = xmlDoc.CreateNode(XmlNodeType.Element, "Interior", namespaceURI);
                        newStyle.Node.AppendChild(interNode);
                    }
                    else
                    {
                        interNode.Attributes.RemoveNamedItem("ss:Color");
                        interNode.Attributes.RemoveNamedItem("ss:Pattern");
                    }

                    XmlAttribute xmlAttr = xmlDoc.CreateAttribute("ss", "Color", namespaceURI);
                    xmlAttr.Value = color;
                    interNode.Attributes.Append(xmlAttr);
                    xmlAttr = xmlDoc.CreateAttribute("ss", "Pattern", namespaceURI);
                    xmlAttr.Value = "Solid";
                    interNode.Attributes.Append(xmlAttr);

                  
                    styleAttr.Value = newStyleID;
                    styles.Add(newStyleID, newStyle);
                    stylesNode.AppendChild(newStyle.Node);
                }
            }
        }

        /// <summary>
        ///  
        /// </summary>
        protected class Style
        {
            /// <summary>
            ///  
            /// </summary>
            protected XmlNode node;
            /// <summary>
            /// 
            /// </summary>
            protected string id;


            /// <summary>
            /// Конструктор
            /// </summary>
            protected Style()
            {
            }

             public Style(XmlNode xmlNode)
            {
                node = xmlNode;
                id = xmlNode.Attributes["ss:ID"].Value;
            }


             public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

            public string ID
            {
                get
                {
                    return id;
                }
                set
                {
                    id = value;
                    node.Attributes["ss:ID"].Value = id;
                }
            }


          public Style Clone()
            {
                return new Style(node.Clone());
            }
        }

       
        protected class Worksheet
        {
            
            protected XmlNode node;
            
            protected string name;
        
            protected Table table;
  
            protected Workbook parentWorkbook;


      
            protected Worksheet()
            {
            }
  public Worksheet(XmlNode xmlNode)
            {
                node = xmlNode;
                name = xmlNode.Attributes["ss:Name"].Value;
                table = null;
            }

 public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

           public string Name
            {
                get
                {
                    return name;
                }
                set
                {
                    name = value;
                    node.Attributes["ss:Name"].Value = name;
                }
            }

            public Table Table
            {
                get
                {
                    return table;
                }
                set
                {
                    table = value;
                    table.ParentWorksheet = this;
                }
            }

          public Workbook ParentWorkbook
            {
                get
                {
                    return parentWorkbook;
                }
                set
                {
                    parentWorkbook = value;
                }
            }


         public void SplitHorizontal(int rowIndex)
            {
                XmlDocument xmlDoc = node.OwnerDocument;
                XmlNamespaceManager nsmgr = new XmlNamespaceManager(xmlDoc.NameTable);
                nsmgr.AddNamespace("report", XmlNamespaces.x);

                XmlNode optionsNode = node.SelectSingleNode("report:WorksheetOptions", nsmgr);

                if (optionsNode != null)
                {
                    string rowIndexStr = rowIndex.ToString();

                    XmlNode splitNode = optionsNode.SelectSingleNode("report:SplitHorizontal", nsmgr);
                    if (splitNode == null)
                    {
                        splitNode = xmlDoc.CreateElement("SplitHorizontal");
                        optionsNode.AppendChild(splitNode);
                    }
                    splitNode.InnerText = rowIndexStr;

                    XmlNode paneNode = optionsNode.SelectSingleNode("report:TopRowBottomPane", nsmgr);
                    if (paneNode == null)
                    {
                        paneNode = xmlDoc.CreateElement("TopRowBottomPane");
                        optionsNode.AppendChild(paneNode);
                    }
                    paneNode.InnerText = rowIndexStr;
                }
            }

            public Worksheet Clone()
            {
                XmlNode nodeClone = node.CloneNode(false);
                Worksheet worksheetClone = new Worksheet(nodeClone);

                Table tableClone = table.Clone();
                worksheetClone.table = tableClone;
                nodeClone.AppendChild(tableClone.Node);

                foreach (XmlNode childNode in node.ChildNodes)
                    if (childNode.Name != "Table")
                        nodeClone.AppendChild(childNode.Clone());

                return worksheetClone;
            }
        }
 protected class Table
        {
            protected XmlNode node;
           
            protected List<Column> columns;
            
            protected List<Row> rows;
            
            protected Worksheet parentWorksheet;


          
            protected Table()
            {
            }

            public Table(XmlNode xmlNode)
            {
                node = xmlNode;
                columns = new List<Column>();
                rows = new List<Row>();
                parentWorksheet = null;
            }


           public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

            public List<Column> Columns
            {
                get
                {
                    return columns;
                }
            }

           public List<Row> Rows
            {
                get
                {
                    return rows;
                }
            }

            public Worksheet ParentWorksheet
            {
                get
                {
                    return parentWorksheet;
                }
                set
                {
                    parentWorksheet = value;
                }
            }


           public void RemoveTableNodeAttrs()
            {
                node.Attributes.RemoveNamedItem("ss:ExpandedColumnCount");
                node.Attributes.RemoveNamedItem("ss:ExpandedRowCount");
            }

            public Column FindColumn(int columnIndex)
            {
                int index = 0;
                foreach (Column column in columns)
                {
                    index = column.Index > 0 ? column.Index : index + 1;
                    int endIndex = index + column.Span;

                    if (index <= columnIndex && columnIndex <= endIndex)
                        return column;

                    index = endIndex;
                }

                return null;
            }
 public void AppendColumn(Column column)
            {
                if (columns.Count > 0)
                    node.InsertAfter(column.Node, columns[columns.Count - 1].Node);
                else
                    node.PrependChild(column.Node);

                column.ParentTable = this;
                columns.Add(column);
            }

            public void InsertColumn(int listIndex, Column column)
            {
                if (columns.Count == 0 || listIndex == 0)
                    node.PrependChild(column.Node);
                else
                    node.InsertAfter(column.Node, columns[listIndex - 1].Node);

                column.ParentTable = this;
                columns.Insert(listIndex, column);
            }

           public void RemoveColumn(int listIndex)
            {
                Column column = columns[listIndex];
                column.ParentTable = null;
                node.RemoveChild(column.Node);
                columns.RemoveAt(listIndex);
            }

            public void RemoveAllColumns()
            {
                while (columns.Count > 0)
                    RemoveColumn(0);
            }

           public void AppendRow(Row row)
            {
                node.AppendChild(row.Node);
                row.ParentTable = this;
                rows.Add(row);
            }

           public void InsertRow(int listIndex, Row row)
            {
                if (rows.Count == 0)
                    node.AppendChild(row.Node);
                else if (listIndex == 0)
                    node.InsertBefore(row.Node, rows[0].Node);
                else
                    node.InsertAfter(row.Node, rows[listIndex - 1].Node);

                row.ParentTable = this;
                rows.Insert(listIndex, row);
            }

           public void RemoveRow(int listIndex)
            {
                Row row = rows[listIndex];
                row.ParentTable = null;
                node.RemoveChild(row.Node);
                rows.RemoveAt(listIndex);
            }

           public void RemoveAllRows()
            {
                while (rows.Count > 0)
                    RemoveRow(0);
            }

           public Table Clone()
            {
                XmlNode nodeClone = node.CloneNode(false);
                Table tableClone = new Table(nodeClone);
                tableClone.parentWorksheet = parentWorksheet;

                foreach (Column column in columns)
                {
                    Column columnClone = column.Clone();
                    tableClone.columns.Add(columnClone);
                    nodeClone.AppendChild(columnClone.Node);
                }

                foreach (Row row in rows)
                {
                    Row rowClone = row.Clone();
                    tableClone.rows.Add(rowClone);
                    nodeClone.AppendChild(rowClone.Node);
                }

                return tableClone;
            }
        }

       protected class Column
        {
           protected XmlNode node;
            protected Table parentTable;
           protected int index;

  protected Column()
            {
            }

            public Column(XmlNode xmlNode)
            {
                node = xmlNode;
                parentTable = null;

                XmlAttribute attr = node.Attributes["ss:Index"];
                index = attr == null ? 0 : int.Parse(attr.Value);
            }


            public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

           public Table ParentTable
            {
                get
                {
                    return parentTable;
                }
                set
                {
                    parentTable = value;
                }
            }

           public int Index
            {
                get
                {
                    return index;
                }
                set
                {
                    index = value;
                    SetAttribute(node, "Index", XmlNamespaces.ss, index <= 0 ? null : index.ToString(), true);
                }
            }

             
            public double Width
            {
                get
                {
                    string widthStr = GetAttribute(node, "ss:Width");
                    double width;
                    return double.TryParse(widthStr, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out width) ?
                        width : 0;
                }
                set
                {
                    SetColumnWidth(node, value);
                }
            }

          
            public int Span
            {
                get
                {
                    string valStr = GetAttribute(node, "ss:Span");
                    return valStr == "" ? 0 : int.Parse(valStr);
                }
                set
                {
                    SetAttribute(node, "Span", XmlNamespaces.ss, value < 1 ? "" : value.ToString(), true);
                }
            }


         
            public Column Clone()
            {
                Column columnClone = new Column(node.Clone());
                columnClone.parentTable = parentTable;
                return columnClone;
            }

          
            public static void SetColumnWidth(XmlNode columnNode, double width)
            {
                SetAttribute(columnNode, "AutoFitWidth", XmlNamespaces.ss, "0");
                SetAttribute(columnNode, "Width", XmlNamespaces.ss, 
                    width > 0 ? width.ToString(NumberFormatInfo.InvariantInfo) : "", true);
            }
        }

      
        protected class Row
        {
        
            protected XmlNode node;
         
            protected List<Cell> cells;
         
            protected Table parentTable;


            
            protected Row()
            {
            }
public Row(XmlNode xmlNode)
            {
                node = xmlNode;
                cells = new List<Cell>();
                parentTable = null;
            }


           
            public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

         
            public List<Cell> Cells
            {
                get
                {
                    return cells;
                }
            }

        
            public Table ParentTable
            {
                get
                {
                    return parentTable;
                }
                set
                {
                    parentTable = value;
                }
            }

            public double Height
            {
                get
                {
                    string heightStr = GetAttribute(node, "ss:Height");
                    double height;
                    return double.TryParse(heightStr, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out height) ?
                        height : 0;
                }
                set
                {
                    SetRowHeight(node, value);
                }
            }


        
            public Row Clone()
            {
                XmlNode nodeClone = node.CloneNode(false);
                Row rowClone = new Row(nodeClone);
                rowClone.parentTable = parentTable;

                foreach (Cell cell in cells)
                {
                    Cell cellClone = cell.Clone();
                    rowClone.cells.Add(cellClone);
                    nodeClone.AppendChild(cellClone.Node);
                }

                return rowClone;
            }

            public Cell FindCell(int cellIndex)
            {
                int index = 0;
                foreach (Cell cell in cells)
                {
                    index = cell.Index > 0 ? cell.Index : index + 1;
                    int endIndex = index + cell.MergeAcross;

                    if (index <= cellIndex && cellIndex <= endIndex)
                        return cell;

                    index = endIndex;
                }

                return null;
            }

            public void AppendCell(Cell cell)
            {
                cells.Add(cell);
                cell.ParentRow = this;
                node.AppendChild(cell.Node);
            }

          public void InsertCell(int listIndex, Cell cell)
            {
                cells.Insert(listIndex, cell);
                cell.ParentRow = this;

                if (cells.Count == 1)
                    node.AppendChild(cell.Node);
                else if (listIndex == 0)
                    node.PrependChild(cell.Node);
                else
                    node.InsertAfter(cell.Node, cells[listIndex - 1].Node);
            }
 public void RemoveCell(int listIndex)
            {
                Cell cell = cells[listIndex];
                cell.ParentRow = null;
                node.RemoveChild(cell.Node);
                cells.RemoveAt(listIndex);
            }


           public static void SetRowHeight(XmlNode rowNode, double height)
            {
                SetAttribute(rowNode, "AutoFitHeight", XmlNamespaces.ss, "0");
                SetAttribute(rowNode, "Height", XmlNamespaces.ss,
                    height > 0 ? height.ToString(NumberFormatInfo.InvariantInfo) : "", true);
            }
        }

       protected class Cell
        {
           
            public static class DataTypes
            {
               
                public const string String = "String";
               
                public const string Number = "Number";
            }

           
            protected XmlNode node;
            
            protected XmlNode dataNode;
           
            protected Row parentRow;
           
            protected int index;


       
            protected Cell()
            {
            }

          public Cell(XmlNode xmlNode)
            {
                node = xmlNode;
                dataNode = null;
                parentRow = null;

                XmlAttribute attr = node.Attributes["ss:Index"];
                index = attr == null ? 0 : int.Parse(attr.Value);
            }


         public XmlNode Node
            {
                get
                {
                    return node;
                }
            }

          public XmlNode DataNode
            {
                get
                {
                    return dataNode;
                }
                set
                {
                    dataNode = value;
                }
            }

          public Row ParentRow
            {
                get
                {
                    return parentRow;
                }
                set
                {
                    parentRow = value;
                }
            }

          public int Index
            {
                get
                {
                    return index;
                }
                set
                {
                    index = value;
                    SetAttribute(node, "Index", XmlNamespaces.ss, index <= 0 ? null : index.ToString(), true);
                }
            }

           public string DataType
            {
                get
                {
                    return GetAttribute(dataNode, "ss:Type");
                }
                set
                {
                    SetAttribute(dataNode, "Type", XmlNamespaces.ss, 
                        string.IsNullOrEmpty(value) ? DataTypes.String : value);
                }
            }

           public string Text
            {
                get
                {
                    return dataNode == null ? "" : dataNode.InnerText;
                }
                set
                {
                    if (dataNode != null)
                        dataNode.InnerText = value;
                }
            }
public string Formula
            {
                get
                {
                    return GetAttribute(node, "ss:Formula");
                }
                set
                {
                    SetAttribute(node, "Formula", XmlNamespaces.ss, value, true);
                }
            }

           
            public string StyleID
            {
                get
                {
                    return GetAttribute(node, "ss:StyleID");
                }
                set
                {
                    SetAttribute(node, "StyleID", XmlNamespaces.ss, value, true);
                }
            }

      
            public int MergeAcross
            {
                get
                {
                    string valStr = GetAttribute(node, "ss:MergeAcross");
                    return valStr == "" ? 0 : int.Parse(valStr);
                }
                set
                {
                    SetAttribute(node, "MergeAcross", XmlNamespaces.ss, value < 1 ? "" : value.ToString(), true);
                }
            }

         
            public int MergeDown
            {
                get
                {
                    string valStr = GetAttribute(node, "ss:MergeDown");
                    return valStr == "" ? 0 : int.Parse(valStr);
                }
                set
                {
                    SetAttribute(node, "MergeDown", XmlNamespaces.ss, value < 1 ? "" : value.ToString(), true);
                }
            }


          
            public int CalcIndex()
            {
                if (index > 0)
                {
                    return index;
                }
                else
                {
                    int index = 0;
                    foreach (Cell cell in parentRow.Cells)
                    {
                        index = cell.Index > 0 ? cell.Index : index + 1;
                        if (cell == this)
                            return index;
                        index += cell.MergeAcross;
                    }
                    return 0;
                }
            }

            public Cell Clone()
            {
                XmlNode nodeClone = node.CloneNode(false);
                Cell cellClone = new Cell(nodeClone);
                cellClone.parentRow = parentRow;

                if (dataNode != null)
                {
                    cellClone.dataNode = dataNode.Clone();
                    nodeClone.AppendChild(cellClone.dataNode);

                    foreach (XmlNode childNode in node.ChildNodes)
                        if (childNode.Name != "Data")
                            nodeClone.AppendChild(childNode.Clone());
                }

                return cellClone;
            }

            public void SetNumberType()
            {
                DataType = DataTypes.Number;
            }
        }


        protected const string Break = "&#10;";
   
        protected const string DirectiveElem = "Data";


        protected XmlDocument xmlDoc;

        protected Workbook workbook;


        protected Table procTable;


        protected Cell procCell;

        protected bool textBroken;


 
        public ExcelRepBuilder()
            : base()
        {
            xmlDoc = null;
        }



        public override string RepFormat
        {
            get
            {
                return "SpreadsheetML";
            }
        }



        protected static string GetAttribute(XmlNode xmlNode, string name)
        {
            if (xmlNode == null)
            {
                return "";
            }
            else
            {
                XmlAttribute xmlAttr = xmlNode.Attributes[name];
                return xmlAttr == null ? "" : xmlAttr.Value;
            }
        }

        protected static void SetAttribute(XmlNode xmlNode, string localName, string namespaceURI, string value, 
            bool removeEmpty = false)
        {
            if (xmlNode != null)
            {
                if (string.IsNullOrEmpty(value) && removeEmpty)
                {
                    xmlNode.Attributes.RemoveNamedItem(localName, namespaceURI);
                }
                else
                {
                    XmlAttribute xmlAttr = xmlNode.Attributes.GetNamedItem(localName, namespaceURI) as XmlAttribute;
                    if (xmlAttr == null)
                    {
                        xmlAttr = xmlNode.OwnerDocument.CreateAttribute("", localName, namespaceURI);
                        xmlAttr.Value = value;
                        xmlNode.Attributes.SetNamedItem(xmlAttr);
                    }
                    else
                    {
                        xmlAttr.Value = value;
                    }
                }
            }
        }

        protected bool FindDirective(string s, string attrName, out string attrVal, out string rest)
        {
            // "attrName=attrVal", вместо '=' может быть любой символ
            int valStartInd = attrName.Length + 1;
            if (valStartInd <= s.Length && s.StartsWith(attrName, StringComparison.Ordinal))
            {
                int valEndInd = s.IndexOf(" ", valStartInd);
                if (valEndInd < 0)
                {
                    attrVal = s.Substring(valStartInd);
                    rest = "";
                }
                else
                {
                    attrVal = s.Substring(valStartInd, valEndInd - valStartInd);
                    rest = s.Substring(valEndInd + 1);
                }
                return true;
            }
            else
            {
                attrVal = "";
                rest = s;
                return false;
            }
        }

 
        protected void SetNodeTextWithBreak(XmlNode xmlNode, string text, string textBreak)
        {
            if (text == null)
                text = "";
            xmlNode.InnerText = text.Replace(textBreak, Break);
            textBroken = true;
        }

        protected void SetNodeTextWithBreak(XmlNode xmlNode, object text, string textBreak)
        {
            string textStr = text == null ? "" : text.ToString();
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
            if (xmlNode.Name == DirectiveElem)
            {
                procCell.DataNode = xmlNode;
                FindDirectives(procCell); // поиск и обработка директив
            }
            else
            {
                // формирование книги Excel на основе XML-документа
                if (xmlNode.Name == "Workbook")
                {
                    workbook = new Workbook(xmlNode);
                }
                else if (xmlNode.Name == "Styles")
                {
                    workbook.StylesNode = xmlNode;
                }
                else if (xmlNode.Name == "Style")
                {
                    Style style = new Style(xmlNode);
                    workbook.Styles.Add(style.ID, style);
                }
                else if (xmlNode.Name == "Worksheet")
                {
                    procWorksheet = new Worksheet(xmlNode);
                    procWorksheet.ParentWorkbook = workbook;
                    workbook.Worksheets.Add(procWorksheet);
                }
                else if (xmlNode.Name == "Table")
                {
                    procTable = new Table(xmlNode);
                    procTable.ParentWorksheet = procWorksheet;
                    procWorksheet.Table = procTable;
                }
                else if (xmlNode.Name == "Column")
                {
                    Column column = new Column(xmlNode);
                    column.ParentTable = procTable;
                    procTable.Columns.Add(column);
                }
                else if (xmlNode.Name == "Row")
                {
                    procRow = new Row(xmlNode);
                    procRow.ParentTable = procTable;
                    procTable.Rows.Add(procRow);
                }
                else if (xmlNode.Name == "Cell")
                {
                    procCell = new Cell(xmlNode);
                    procCell.ParentRow = procRow;
                    procRow.Cells.Add(procCell);
                }

                // рекурсивный перебор потомков текущего элемента
                XmlNodeList children = xmlNode.ChildNodes;
                foreach (XmlNode node in children)
                    XmlDocProc(node);
            }
        }

        protected virtual void FinalXmlDocProc()
        {
        }

        protected virtual void ExcelProc(Table table)
        {
            foreach (Row row in table.Rows)
                ExcelProc(row);
        }

        protected virtual void ExcelProc(Row row)
        {
            // поиск и обработка директив для ячеек строки
            foreach (Cell cell in row.Cells)
                FindDirectives(cell);
        }


        protected virtual void FindDirectives(Cell cell)
        {
            XmlNode dataNode = cell.DataNode;
            if (dataNode != null)
            {
                string attrVal;
                string rest;
                if (FindDirective(dataNode.InnerText, "repRow", out attrVal, out rest))
                {
                    dataNode.InnerText = rest;
                    ProcRow(cell, attrVal);
                }
                else if (FindDirective(dataNode.InnerText, "repVal", out attrVal, out rest))
                {
                    ProcVal(cell, attrVal);
                }
            }
        }

        protected virtual void ProcVal(Cell cell, string valName)
        {
        }

        protected virtual void ProcRow(Cell cell, string rowName)
        {
        }

        public override void Make(Stream outStream, string templateDir)
        {
          
            string templFileName = templateDir + TemplateFileName;

          
            xmlDoc = new XmlDocument();
            xmlDoc.Load(templFileName);

           
            workbook = null;
            procWorksheet = null;
            procTable = null;
            procRow = null;
            procCell = null;
            textBroken = false;

            StartXmlDocProc();
            XmlDocProc(xmlDoc.DocumentElement);
            FinalXmlDocProc();

         
            if (textBroken)
            {
                StringWriter stringWriter = new StringWriter();
                xmlDoc.Save(stringWriter);
                string xmlText = stringWriter.GetStringBuilder()
                    .Replace("encoding=\"utf-16\"", "encoding=\"utf-8\"")
                    .Replace("&amp;#10", "&#10").ToString();
                byte[] bytes = Encoding.UTF8.GetBytes(xmlText);
                outStream.Write(bytes, 0, bytes.Length);
            }
            else
            {
                xmlDoc.Save(outStream);
            }
        }
    }
}
