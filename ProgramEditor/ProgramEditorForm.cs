namespace T3000.Forms
{
    using FastColoredTextBoxNS;
    using Irony;
    using Irony.Parsing;
    using PRGReaderLibrary.Extensions;
    using PRGReaderLibrary.Types.Enums.Codecs;
    using PRGReaderLibrary.Utilities;
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Text.RegularExpressions;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;
    using System.ComponentModel;




    /// <summary>
    /// Delegate to event handler Send
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void SendEventHandler(object sender, SendEventArgs e);


    [Guid("E8789EA8-CA99-48DF-B920-1A1DCB4FF6AD")]
    [ClassInterface(ClassInterfaceType.None)]
    public partial class ProgramEditorForm : UserControl, IProgramEditor
    {

        /// <summary>
        /// Required copy of Control Points Labels just for semantic validations
        /// </summary>
        public ControlPoints Identifiers { get; set; } = new ControlPoints();

        [Description("Code Text"), Category("ProgramEditor")]
        public string Code
        {
           get { return editTextBox.Text; }
           set { editTextBox.Text = value; ParseCode(false); }
        }


        List<EditorTokenInfo> Tokens = new List<EditorTokenInfo>();

        //int ParsingTimes = 0;

        /// <summary>
        /// Form caption
        /// </summary>
        [Description("Window Caption"), Category("ProgramEditor")]
        public string Caption
        {
            get { return this.Text; }
            set { this.Text = value; }
        }

        

        Grammar _grammar;
        LanguageData _language;
        ParseTree _parseTree;
        Parser _parser;


        /// <summary>
        /// Stack of recursive examined functions, counting subexpressions
        /// </summary>
        Stack<EditorTokenInfo> functions = new Stack<EditorTokenInfo>();
        /// <summary>
        /// Stack of recursive examined branches, counting subexpressions
        /// </summary>
        Stack<EditorTokenInfo> branches = new Stack<EditorTokenInfo>();


        //Container of all line numbers
        List<EditorLineInfo> Lines;
        List<EditorJumpInfo> Jumps;
        long LastParseTime;

        /// <summary>
        /// Event Send
        /// </summary>
        public event SendEventHandler Send;

        /// <summary>
        /// Overridable OnSend Event Handler
        /// </summary>
        public void OnSend(SendEventArgs e)
        {

            Code = editTextBox.Text;

            //if (Send != null)
            //{
            //    Send(this, e);
            //}


            Send?.Invoke(this, e);


        }

        /// <summary>
        /// Default constructor of ProgramEditorForm
        /// Use: SetCode(string) to assign program code to Editor.
        /// </summary>
        public ProgramEditorForm()
        {
            InitializeComponent();

            editTextBox.Grammar = new T3000Grammar();
            editTextBox.SetParser(new LanguageData(editTextBox.Grammar));
            //LRUIZ :: Enable a new set of grammar, language and parser, to get Program Code Errors
            _grammar = new T3000Grammar();
            _language = new LanguageData(_grammar);
            _parser = new Parser(_language);
            //LRUIZ
      

            
            //var items = new List<AutocompleteItem>();
            var keywords = new List<string>()
            {
                "REM",
                "IF",
                "IF-",
                "IF+",
                "THEN",
                "ELSE",
                "TIME-ON"
            };
            keywords.AddRange(T3000Grammar.Functions);

            //foreach (var item in keywords)
            //    items.Add(new AutocompleteItem(item) { ImageIndex = 1 });

            //var snippets = new[]{
            //    "if(^)\n{\n}",
            //    "if(^)\n{\n}\nelse\n{\n}",
            //    "for(^;;)\n{\n}", "while(^)\n{\n}",
            //    "do${\n^}while();",
            //    "switch(^)\n{\n\tcase : break;\n}"
            //};
            //foreach (var item in snippets)
            //    items.Add(new SnippetAutocompleteItem(item) { ImageIndex = 1 });

            //set as autocomplete source
            //autocompleteMenu.Items.SetAutocompleteItems(items);


           // this.WindowState = FormWindowState.Maximized;
            //this.WindowState = FormWindowState.Normal;

            
            
        }

        /// <summary>
        /// Get next line number
        /// </summary>
        /// <returns>new line number (string)</returns>
        string GetNextLineNumber()
        {
            
            Lines = new List<EditorLineInfo>();
            var lines = editTextBox.Text.ToLines(StringSplitOptions.RemoveEmptyEntries);
            //Preload ALL line numbers
            for (var i = 0; i < lines.Count; i++)
            {
                var words = lines[i].Split(' ');

                var LINFO = new EditorLineInfo(Convert.ToInt32(words[0]), (i + 1) * 10);
                this.Lines.Add(LINFO);
            }

            return (Lines.LastOrDefault().Before  + 10).ToString();

        }

        /// <summary>
        /// Try to renumber all lines and their references.
        /// Show errors as semantic ones.
        /// </summary>
        public void LinesValidator()
        {
            
            if(_parseTree == null) return;

            if (_parseTree.ParserMessages.Any()) return;

            int pos = 0;
            int col = 0;
            bool Cancel = false;
            Lines = new List<EditorLineInfo>();
            Jumps = new List<EditorJumpInfo>();
                       

            var lines = editTextBox.Text.ToLines( StringSplitOptions.RemoveEmptyEntries);

            //Preload ALL line numbers
            for (var i = 0; i < lines.Count; i++)
            {
                var words = lines[i].Split(' ');
                
                var LINFO = new EditorLineInfo(Convert.ToInt32(words[0]), (i + 1) * 10);
                this.Lines.Add(LINFO);
            }

            for (var i = 0; i < lines.Count; i++)
            {
                var words = lines[i].Split(' ');
                

                for(var j=0; j< words.Count(); j++)
                {
                    JumpType type = JumpType.GOTO ;
                    int linenumber = -1;
                    int offset = 0;


                    switch (words[j])
                    {
                        case "GOTO":
                        case "GOSUB":
                        case "ON-ERROR":
                        case "ON-ALARM":
                        case "THEN":

                            switch (words[j][0])
                            {
                                case 'G':
                                    type = words[j] == "GOTO" ? JumpType.GOTO : JumpType.GOSUB; break;
                                case 'O':
                                    type = words[j] == "ON-ERROR" ? JumpType.ONERROR : JumpType.ONALARM; break;
                                case 'T':
                                    type = JumpType.THEN;break;
                            }
                    

                            offset = j + 1;
                            int BeforeLineNumber = -1;
                            if (!Int32.TryParse(words[offset],out BeforeLineNumber)) break;

                            //var BeforeLineNumber = Convert.ToInt32(words[offset]);
                            linenumber = Lines.FindIndex(k => k.Before == BeforeLineNumber );
                            if(linenumber == -1)
                            {
                                //There is a semantic error here
                                //Add error message to parser and cancel renumbering.
                                //Don't break it inmediately, to show all possible errors of this type
                                _parseTree.ParserMessages.Add(new LogMessage(ErrorLevel.Error,
                                    new SourceLocation(pos + words[j].Count() + 1, i , col + words[j].Count()+1), 
                                    $"Semantic Error: Line number {BeforeLineNumber.ToString()} for {words[j]} does not exist", 
                                    new ParserState("Validating Lines")));
                                ShowCompilerErrors();
                                Cancel = true;
                            }
                            EditorJumpInfo JINFO = new EditorJumpInfo(type, i, offset );
                            Jumps.Add(JINFO);
                            //Change reference to new linenumber
                            words[offset] = linenumber == -1? BeforeLineNumber.ToString():Lines[linenumber].ToString();
                            break;
                            
                    }//switch jumps
                     pos += words[j].Count() + 1;
                    col += words[j].Count() + 1;
                }//for words
                pos++;
                col = 0;
                //change current linenumber
                words[0] = Lines[i].ToString();
                lines[i] = string.Join(' '.ToString(), words);
                

            }//for lines
            string newcode = string.Join(Environment.NewLine, lines);
            if (Cancel) return;
            editTextBox.Text = newcode;
        }

        /// <summary>
        /// Set code to EditBox, ProgramCode is automatically parsed. 
        /// </summary>
        /// <param name="code">String contaning plain text Control Basic with numbered lines {Not Bytes Encoded Programs}</param>
        public void SetCode(string code)
        {
            Code = code;
            //editTextBox.Text = RemoveInitialNumbers(code);
            
            editTextBox.Text = Code;
           
            //LRUIZ: Parse and show syntax errors

            ParseCode(false);
            
        }
        
        private void cmdClear_Click(object sender, EventArgs e)
        {
            ClearCode();
        }

        /// <summary>
        /// Clear editBox only.
        /// If you want to update/clear the inner code, use SetCode
        /// </summary>
        public void ClearCode()
        {
            //local member Code is not cleared, to allow recovering with REFRESH (F8)
            editTextBox.Text = "";
        }

        /// <summary>
        /// Override of ToString -> GetCode
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return GetCode();
        }

        /// <summary>
        /// Get current code
        /// </summary>
        /// <returns></returns>
        public string GetCode()
        {
            Code = editTextBox.Text;
            return Code;
        }

        /// <summary>
        /// Forces parsing the code contained in EditTextBox
        /// </summary>
        public void ParseCode(bool fullParsing = false)
        {
            ClearParserOutput();
            if (_parser == null || !_parser.Language.CanParse()) return;
            _parseTree = null;
            GC.Collect(); //to avoid disruption of perf times with occasional collections
            _parser.Context.TracingEnabled = true;
            try
            {
                Stopwatch stopwatch = Stopwatch.StartNew(); //creates and start the instance of Stopwatch

                _parser.Parse(editTextBox.Text, "<source>");
                _parseTree = _parser.Context.CurrentParseTree;


                if (_parseTree.ParserMessages.Any() || _parseTree.HasErrors()) return;

                if (fullParsing) //Only do this checks in full parsing.
                {
                    LinesValidator(); // Check semantic errors on jumps and renumber lines.
                    ProcessTokens(); //Check for other semantic errors and make some changes to local list of tokens

                    if (_parseTree.ParserMessages.Any() || _parseTree.HasErrors())
                    {
                        MessageBox.Show($"{_parseTree.ParserMessages.Count()} error(s) found!{Environment.NewLine}Compiler halted.", "Semantic Errors Found!");
                        return;
                    }
                }
                

                System.Threading.Thread.Sleep(500);
                stopwatch.Stop();
                LastParseTime = stopwatch.ElapsedMilliseconds - 500;
                lblParseTime.Text = $"Parse Time: {LastParseTime}ms";

            }
            catch (Exception ex)
            {
                gridCompileErrors.Rows.Add(null, ex.Message, null);
                
                //throw;
            }
            finally
            {
                
                ShowCompilerErrors();

               
                ShowCompileStats();
               
            }
        }

        private void ClearParserOutput()
        {

            lblSrcLineCount.Text = string.Empty;
            lblSrcTokenCount.Text = "";
            lblParseTime.Text = "";
            lblParseErrorCount.Text = "";

        
            gridCompileErrors.Rows.Clear();
           
            Application.DoEvents();
        }

        private void ShowCompileStats()
        {
            lblSrcLineCount.Text = $"Lines: {editTextBox.Lines.Count()} ";
            lblSrcTokenCount.Text = $"Tokens: {_parseTree.Tokens.Count()}";
            lblParseTime.Text = $"Parse Time: {LastParseTime}ms";
            lblParseErrorCount.Text = _parseTree.HasErrors() ? $"Errors: {_parseTree.ParserMessages.Count() }" : "No Errors";

        }

        /// <summary>
        /// Updates Compile Errors Gridview
        /// </summary>
        private void ShowCompilerErrors()
        {
            gridCompileErrors.Rows.Clear();
            if (_parseTree == null || _parseTree.ParserMessages.Count == 0) return;
            foreach (var err in _parseTree.ParserMessages)
                gridCompileErrors.Rows.Add(err.Location, err, err.ParserState);
        }

        /// <summary>
        /// Allows to position over token at selected error.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void gridCompileErrors_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0 || e.RowIndex >= gridCompileErrors.Rows.Count) return;
            var err = gridCompileErrors.Rows[e.RowIndex].Cells[1].Value as LogMessage;
            switch (e.ColumnIndex)
            {
                case 0: //state
                case 1: //stack top
                    ShowSourcePosition(err.Location.Position, 1);
                    break;
            }//switch
        }

        /// <summary>
        /// Shows a caret in editTextBox for a selected token
        /// </summary>
        /// <param name="position"></param>
        /// <param name="length"></param>
        private void ShowSourcePosition(int position, int length)
        {
            if (position < 0) return;
            editTextBox.SelectionStart = position;
            editTextBox.SelectionLength = length;
            //editTextBox.Select(location.Position, length);
            editTextBox.DoCaretVisible();
            editTextBox.Focus();
            
        }

        /// <summary>
        /// Parse code delayed after editing program.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void editTextBox_TextChangedDelayed(object sender, TextChangedEventArgs e)
        {
            ParseCode(false);
            
        }

        private void cmdRefresh_Click(object sender, EventArgs e)
        {
            RefreshCode();
        }

        /// <summary>
        /// Refresh, reload assigned code into editBox
        /// </summary>
        public void RefreshCode()
        {
            
            editTextBox.Text = Code;
          
        }
        private void ProgramEditorForm_KeyDown(object sender, KeyEventArgs e)
        {
            
            

            switch(e.KeyCode )
            {
                case Keys.F2:
                    SendCode(); e.Handled = true; break;
                case Keys.F4:
                    ClearCode(); e.Handled = true; break;
                case Keys.F6:
                    SaveFile(); e.Handled = true; break;
                case Keys.F7:
                    LoadFile(); e.Handled = true; break;
                case Keys.F8:
                    RefreshCode(); e.Handled = true; break;
                case Keys.F10:
                    LinesValidator(); e.Handled = true; break;
                
            }//switch.
           
        }

        private void cmdLoad_Click(object sender, EventArgs e)
        {
            
            LoadFile();
           
        }

        /// <summary>
        /// Open file dialog to load a text file into editor
        /// </summary>
        public void LoadFile()
        {
            // Create an instance of the open file dialog box.
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            // Set filter options and filter index.
            openFileDialog1.Filter = "Text Files (.txt)|*.txt|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;

            openFileDialog1.Multiselect = true;

            // Call the ShowDialog method to show the dialog box.
            DialogResult  userClickedOK = openFileDialog1.ShowDialog();

            // Process input if the user clicked OK.
            if (userClickedOK == DialogResult.OK )
            { 
                string text = System.IO.File.ReadAllText(openFileDialog1.FileName);
                
                editTextBox.Text = text;
               
            }
        }

        /// <summary>
        /// User call to SendCode event
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdSend_Click(object sender, EventArgs e)
        {
            SendCode();
        }



        /// <summary>
        /// Triggers SendCode Event
        /// </summary>
        private void SendCode()
        {
            ParseCode(true); //Performs full parsing and semantic checks
            
            if(_parseTree.HasErrors() || _parseTree.ParserMessages.Any())
            {
                MessageBox.Show("Send operation, aborted", "Error(s) found");
                return;
            }

            Code = editTextBox.Text;
            OnSend(new SendEventArgs(Code,Tokens));
            
        }

        private void cmdSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        /// <summary>
        /// Open File dialog to save a copy of program code into a file.
        /// </summary>
        public void SaveFile()
        {
            // Create an instance of the open file dialog box.
            SaveFileDialog openFileDialog1 = new SaveFileDialog();

            // Set filter options and filter index.
            openFileDialog1.Filter = "Text Files (.txt)|*.txt|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;

            

            // Call the ShowDialog method to show the dialog box.
            DialogResult userClickedOK = openFileDialog1.ShowDialog();

            // Process input if the user clicked OK.
            if (userClickedOK == DialogResult.OK)
            {
                System.IO.File.WriteAllText (openFileDialog1.FileName,editTextBox.Text );
               
            }
        }


        private void cmdSettings_Click(object sender, EventArgs e)
        {
            EditSettings();
        }

        private void EditSettings()
        {
            


            SettingsBag.SelectedObject = editTextBox ;
            SettingsBag.Top = editTextBox.Top;
            SettingsBag.Height = editTextBox.Height;
            SettingsBag.Left = editTextBox.Width - SettingsBag.Width;
           
            SettingsBag.Visible = !SettingsBag.Visible ;


            ////NOT WORKING: Serialize SettingsBag;
            //IFormatter formatter = new BinaryFormatter();
            //Stream stream = new FileStream("EditorSettings.bin", FileMode.Create, FileAccess.Write, FileShare.None);
            //formatter.Serialize(stream, editTextBox  );
            //stream.Close();

            

        }

        private void ProgramEditorForm_ResizeEnd(object sender, EventArgs e)
        {
            if (SettingsBag.Visible)
            {
                SettingsBag.Top = editTextBox.Top;
                SettingsBag.Height = editTextBox.Height;
                SettingsBag.Left = editTextBox.Width - SettingsBag.Width;
            }
        }

        private void ProgramEditorForm_Resize(object sender, EventArgs e)
        {
            if (SettingsBag.Visible)
            {
                SettingsBag.Top = editTextBox.Top;
                SettingsBag.Height = editTextBox.Height;
                SettingsBag.Left = editTextBox.Width - SettingsBag.Width;
            }
        }

        private void cmdRenumber_Click(object sender, EventArgs e)
        {
            LinesValidator();
        }




        /// <summary>
        /// Pre-process tokens from parser
        /// </summary>
        /// <remarks>Expressions are converted to RPN</remarks>
        public void ProcessTokens()
        {
            
            string[] excludeTokens = { "CONTROL_BASIC", "LF" };
            bool isFirstToken = true;
            var Cancel = false;

            int WaitCount = 0;

            try
            {
                Tokens = new List<EditorTokenInfo>();

                if (_parseTree == null) return;

                //foreach (var tok in _parseTree.Tokens)
                for(var idxToken = 0; idxToken < _parseTree.Tokens.Count; idxToken++)
                {
                    var tok = _parseTree.Tokens[idxToken];
                    var tokentext = tok.Text;
                    var terminalname = tok.Terminal.Name;

                    switch (tok.Terminal.Name)
                    {

                        #region Comments
                        case "Comment":
                            //split Comments into two tokens
                            Tokens.Add(new EditorTokenInfo("REM", "REM"));
                            Tokens.Last().Type = (short)LINE_TOKEN.REM;
                            Tokens.Last().Token = (short)LINE_TOKEN.REM;
                            var commentString = tok.Text.Substring(4).TrimEnd(' ');
                            Tokens.Add(new EditorTokenInfo(commentString, "Comment"));
                            Tokens.Last().Type = (short)commentString.Length;
                            Tokens.Last().Token = (short)LINE_TOKEN.STRING;
                            break;
                        #endregion

                        case "PhoneNumber":
                            var PhoneString = tok.Text.TrimEnd(' ');
                            Tokens.Add(new EditorTokenInfo(PhoneString, "PhoneNumber"));
                            Tokens.Last().Type = (short)PhoneString.Length;
                            Tokens.Last().Token = (short)LINE_TOKEN.STRING;
                            break;

                        case "IntegerNumber":
                            //rename to LineNumber only if first token on line.
                            Tokens.Add(new EditorTokenInfo(tokentext, isFirstToken ? "LineNumber" : terminalname));
                            break;

                        case "LocalVariable":
                            EditorTokenInfo NewLocalVar = new EditorTokenInfo(tokentext, terminalname);
                            NewLocalVar.Type = (short)PCODE_CONST.LOCAL_VAR;
                            NewLocalVar.Token = (short)TYPE_TOKEN.IDENTIFIER;
                            Tokens.Add(NewLocalVar);
                            break;

                        #region Control Points Generics | Identifiers, before any expressions
                        //Before any expression, as in assigments
                        //Acá faltan varios tipos de identificadores, agregarlos posteriormente
                        //VARS | PIDS | WRS | ARS | OUTS | INS | PRG | GRP | DMON | AMON
                        case "VARS":
                        case "INS":
                        case "OUTS":
                       
                            string output = Regex.Match(tokentext, @"\d+").Value;
                            int CtrlPointIndex = Convert.ToInt16(output) - 1; //VAR1 will get index 0, and so on.
                                                                              //Prepare token identifier to encode: Token + Index + Type
                            EditorTokenInfo CPIdentifier = new EditorTokenInfo(tokentext, tok.Terminal.Name);
                            switch (tok.Terminal.Name)
                            {
                                case "VARS":
                                    CPIdentifier.Type = (short)PCODE_CONST.VARPOINTTYPE;
                                    break;
                                case "INS":
                                    CPIdentifier.Type = (short)PCODE_CONST.INPOINTTYPE;
                                    break;
                                case "OUTS":
                                    CPIdentifier.Type = (short)PCODE_CONST.OUTPOINTTYPE;
                                    break;
                                default:
                                    CPIdentifier.Type = (short)PCODE_CONST.UNDEFINED_SYMBOL;
                                    break;
                            }
                            
                            CPIdentifier.Index = (short)CtrlPointIndex;
                            CPIdentifier.Token = (short)PCODE_CONST.LOCAL_POINT_PRG;
                            Tokens.Add(CPIdentifier);
                            break;

                        case "PRG":
                            string idxPRG = Regex.Match(tokentext, @"\d+").Value;
                            int CPIdx = Convert.ToInt16(idxPRG) - 1; //PRG1 will get index 0, and so on.
                                                                     //Prepare token identifier to encode: Index (single Byte)
                            EditorTokenInfo CPIdPRG = new EditorTokenInfo(tokentext, "PRG");
                            CPIdPRG.Type = (short)TYPE_TOKEN.KEYWORD;
                            CPIdPRG.Index = (short)CPIdx;
                            CPIdPRG.Token = (short)PCODE_CONST.LOCAL_POINT_PRG;
                            Tokens.Add(CPIdPRG);
                            break;

                        case "MRK": //termporary marker for commas between arguments, generated when
                            //encoding ARGCOUNT for CALL PRG
                            //Translated to EOE
                            Tokens.Add(new EditorTokenInfo("EOE", "EOE"));
                            Tokens.Last().Token = (short)LINE_TOKEN.EOE;

                            break;

                        case "Identifier":
                            //Locate Identifier and Identify Token associated ControlPoint.
                            //To include this info in TokenInfo.Type and update TokenInfo.TerminalName
                            int PointIndex = 0;
                            var TokenType = Encoder.GetTypeIdentifier(tokentext, out PointIndex);
                            if (TokenType == PCODE_CONST.UNDEFINED_SYMBOL)
                            {
                                //There is a semantic error here
                                //Add error message to parser and cancel renumbering.
                                //Don't break it inmediately, to show all possible errors of this type
                                _parseTree.ParserMessages.Add(new LogMessage(ErrorLevel.Error,
                                    tok.Location,
                                    $"Semantic Error: Undefined Identifier: {tok.Text}{System.Environment.NewLine}Check if PRG object is valid.",
                                    new ParserState("Validating Tokens")));
                                ShowCompilerErrors();
                                Cancel = true;
                            }
                            else
                            {
                                //Prepare token identifier to encode: Token + Index + Type
                                EditorTokenInfo NewIdentifier = new EditorTokenInfo(tokentext, terminalname);
                                NewIdentifier.Type = (short)TokenType;
                                NewIdentifier.Index = (short)PointIndex;
                                NewIdentifier.Token = (short)PCODE_CONST.LOCAL_POINT_PRG;
                                Tokens.Add(NewIdentifier);
                            }
                            break;

                        #endregion

                        #region Assigments and Expressions
                        case "ASSIGN":

                            if(Tokens.Last().TerminalName == "PRG")
                            {
                                //CALL PRG ASSIGMENT ARGS ...
                                //counter for identifiers as arguments
                                Tokens.Add(new EditorTokenInfo("ARGCOUNT", "ARGCOUNT"));

                                //count identifiers (arguments)
                                var ArgIdx = Tokens.Count - 1;
                                var idCnt = 0;
                                var NxtId = idxToken + 1;

                                while (_parseTree.Tokens[NxtId].Terminal.Name == "Identifier"
                                    || _parseTree.Tokens[NxtId].Terminal.Name == "COMMA")
                                {
                                    if (_parseTree.Tokens[NxtId].Terminal.Name == "Identifier") 
                                    {
                                        //count this identifier
                                        idCnt++;
                                    }
                                    else
                                    {
                                        //It's a comma, change it to marker 0xFF
                                        _parseTree.Tokens[NxtId].Terminal.Name = "MRK";
                                    }
                                    NxtId++;
                                }
                                //Also produce a new token here, last MRK = 0xFF
                                _parseTree.Tokens.Insert(NxtId, new Token(new Terminal("MRK"), new SourceLocation(0,0,0), "MRK", null));
                                Tokens[ArgIdx].Index = (short)idCnt;

                                break; //Only when ASSIGN found after  PRG
                            }

                            //Any other way, encode an assigment ↓ ↓ ↓ ↓ 
                            EditorTokenInfo assignToken, last;

                            var index = Tokens.Count - 1;
                            last = Tokens[index];
                            Tokens.RemoveAt(index);
                            assignToken = new EditorTokenInfo(tokentext, terminalname);
                            assignToken.Token = (short)LINE_TOKEN.ASSIGN;
                            //insert it before assignar var.
                            Tokens.Add(assignToken);
                            Tokens.Add(last);
                            //get the expression in postfix
                            functions = new Stack<EditorTokenInfo>();
                            ///////////////////////////////////////////////
                            // ALL FUNCTIONS AND LITERALS IN EXPRESSIONS
                            ///////////////////////////////////////////////
                            Tokens.AddRange(GetExpression(ref idxToken, ref Cancel));
                            //En caso que haya un then o un else en la pila
                            //extraer y poner un delimintador EOE
                            if(branches.Count > 0)
                            {
                                switch (branches.Peek().TerminalName)
                                {
                                    case "THEN":
                                    case "ELSE":
                                        branches.Pop();
                                        Tokens.Add(new EditorTokenInfo("EOE", "EOE"));
                                        Tokens.Last().Token = (short)LINE_TOKEN.EOE;
                                        Tokens.Last().Index = 0; 
                                        break;
                                    default:
                                        break;
                                }
                            }
                            break;
                        #endregion

                        #region Numeric Literals and Constants
                        case "Number":
                        case "CONNUMBER":
                        case "TABLENUMBER":
                        case "SYSPRG":
                        case "TIMER":
                        case "PANEL":
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            Tokens.Last().Token = (short)PCODE_CONST.CONST_VALUE_PRG;
                            Tokens.Last().Index = (short)Convert.ToInt16(tokentext);
                            break;

                        case "PRT_A":
                        case "PRT_B":
                        case "PRT_0":
                        case "ALL":
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            PRT_TOKEN PrtToken = (PRT_TOKEN)Enum.Parse(typeof(PRT_TOKEN), terminalname.ToString().Trim());
                            Tokens.Last().Token = (short)PrtToken;
                                                        
                            break;
                        #endregion

                        #region IF IF+ IF- THEN ELSE
                        case "IF":
                        case "IF+":
                        case "IF-":
                            EditorTokenInfo IfToken = new EditorTokenInfo(tokentext,terminalname);
                            
                            LINE_TOKEN TypeToken = (LINE_TOKEN)Enum.Parse(typeof(LINE_TOKEN), terminalname.ToString().Trim());
                            IfToken.Token = (short)TypeToken;
                            IfToken.Precedence = 200;
                            Tokens.Add(IfToken);
                            branches.Push(IfToken);
                            var LastIdx = idxToken;
                            //GET IF CLAUSE
                            Tokens.AddRange(GetExpression(ref idxToken, ref Cancel));
                            
                            break;

                        case "THEN":
                            //START MARKER FOR THEN PART
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            Tokens.Last().Token = (short)LINE_TOKEN.EOE; //End marker for Expr.
                            Tokens.Last().Index = (short) Tokens.Count; //Next token will be OFFSET

                            if (branches.Count > 0)
                            {
                                switch (branches.Peek().Text)
                                {
                                    case "IF":
                                    case "IF+":
                                    case "IF-":
                                        branches.Pop();//Pop last IF*
                                        branches.Push(Tokens.Last()); //Push corresponding THEN
                                        break;
                                    default:
                                        break;
                                }

                            }

                            //Offset to be treated as a NUMBER
                            Tokens.Add(new EditorTokenInfo("OFFSET", "OFFSET"));
                            Tokens.Last().Token = 0;
                            
                    
                            break;

                        case "ELSE":
                            //ELSE
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            Tokens.Last().Token = (short)LINE_TOKEN.ELSE; //End marker for Expr.

                            if (branches.Count > 0)
                            {
                                switch (branches.Peek().Text)
                                {
                                    case "THEN":
                                    
                                        branches.Pop();//Pop last THEN*
                                        
                                        break;
                                    default:
                                        break;
                                }

                            }

                            branches.Push(Tokens.Last()); //Push corresponding ELSE

                            //START MARKER FOR ELSE PART
                            Tokens.Add(new EditorTokenInfo("EOE", "EOE"));
                            Tokens.Last().Token = (short)LINE_TOKEN.EOE; //End marker for Expr.
                            Tokens.Last().Index = (short)Tokens.Count; //Next token will be OFFSET



                            //Offset to be treated as a NUMBER
                            Tokens.Add(new EditorTokenInfo("OFFSET", "OFFSET"));
                            Tokens.Last().Token = 0;

                            break;
                        #endregion

                        case "LF":
                        case "EOF":
                            if (branches.Count > 0)
                            {
                                switch (branches.Peek().Text)
                                {
                                    case "THEN":
                                    case "ELSE":
                                        var offsetIdx = branches.Pop().Index;
                                        //references token with end marker 
                                        Tokens[offsetIdx].Index  = (short) Tokens.Count;
                                        Tokens.Add(new EditorTokenInfo("EOE", "EOE"));
                                        Tokens.Last().Token = (short)LINE_TOKEN.EOE;
                                        break;
                                    default:
                                        break;
                                }
                            }

                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));

                            break;

                        #region Commands
                        //Simple Commands
                        case "START":
                        case "STOP":
                        case "CLEAR":
                        case "RETURN":
                        case "HANGUP":
                        case "GOTO":
                        case "GOSUB":
                        case "ON_ALARM":
                        case "ON_ERROR":
                        case "ENABLEX":
                        case "DISABLEX":
                        case "ENDPRG":
                        case "RUN_MACRO":
                        case "CALL":
                        case "SET_PRINTER":
                        case "PRINT_AT":
                        case "ALARM_AT":
                        case "PHONE":
                        case "PRINT":
                        
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            LINE_TOKEN SimpleToken = (LINE_TOKEN)Enum.Parse(typeof(LINE_TOKEN), terminalname.ToString().Trim());
                            Tokens.Last().Token = (short)SimpleToken;
                            break;

                        case "DECLARE":

                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            LINE_TOKEN DeclareToken = (LINE_TOKEN)Enum.Parse(typeof(LINE_TOKEN), terminalname.ToString().Trim());
                            Tokens.Last().Token = (short)DeclareToken;

                            Tokens.Add(new EditorTokenInfo("ARGCOUNT", "ARGCOUNT"));
                            
                            //count identifiers (arguments)
                            var DeclareIdx = Tokens.Count - 1;
                            var idCount = 0;
                            var NextId = idxToken + 1;

                            while (_parseTree.Tokens[NextId].Terminal.Name == "Identifier")
                            {
                                idCount++;
                                NextId++;
                            }
                            Tokens[DeclareIdx].Index = (short) idCount;


                            break;
                        case "WAIT":
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            LINE_TOKEN WaitToken = (LINE_TOKEN)Enum.Parse(typeof(LINE_TOKEN), terminalname.ToString().Trim());
                            Tokens.Last().Token = (short)WaitToken;
                            Tokens.AddRange(GetExpression(ref idxToken, ref Cancel));
                            //Add EOE and counter
                            WaitCount++;
                            Tokens.Add(new EditorTokenInfo("WAITCOUNTER", "WAITCOUNTER"));
                            Tokens.Last().Token = (short)LINE_TOKEN.EOE;
                            Tokens.Last().Index = (short) WaitCount;
                           
                            break;
                        #endregion

                        #region Defaults
                        case "LET":
                        default: // No special cases, or expected to be ready to encode.
                            Tokens.Add(new EditorTokenInfo(tokentext, terminalname));
                            
                            break;
                        #endregion
                    }
                    isFirstToken = terminalname == "LF" ? true : false;

                }
            }
            catch( Exception ex) {
                MessageBox.Show(ex.Message, "ProcessTokens()");
                ex = null;
            }


        }

        

 
        /// <summary>
        /// Parse tokens from infix notation into postfix (RPN)
        /// </summary>
        /// <param name="Index">Start Index</param>
        /// <param name="Cancel">Cancel processing because of at least one semantic error</param>
        /// <returns>RPN Expression, ready to be encoded</returns>
        public List<EditorTokenInfo> GetExpression(ref int Index, ref bool Cancel)
        {
            // _parseTree.Tokens.Count
            List<EditorTokenInfo> Expr = new List<EditorTokenInfo>();
            Stack<EditorTokenInfo> Oper = new Stack<EditorTokenInfo>();
            
            //Last processed token was a BEGIN EXPRESSION MARKER
            Index++; //Jump over next token.

            for (; Index < _parseTree.Tokens.Count; Index++)
            {
                var tok = _parseTree.Tokens[Index];
                var tokentext = tok.Text;
                var terminalname = tok.Terminal.Name;
                

                switch (terminalname)
                {

                    #region PARENTHESIS

                    case "(":
                        //If the incoming symbol is a left parenthesis, push it on the stack.
                        Oper.Push(new EditorTokenInfo(tokentext, terminalname));
                        break;
                    
                    case ")":
                        //	If the incoming symbol is a right parenthesis, 
                        // pop the stack and print the operators until you see a left parenthesis. 
                        // Discard the pair of parentheses.
                        if (Oper.Count > 0)
                        {
                            while (Oper.Peek().TerminalName != "(")
                            {
                                Expr.Add(Oper.Pop());
                            }
                        }
                        if(Oper.Count >0) Oper.Pop(); //Discard left parenthesis
                        //see if those parenthesis were parts of a function call.
                        if(Oper.Count > 0 && Oper.Peek().Precedence == 200 )
                        {
                            //Function Call
                            //Add function token to expression.
                            Expr.Add(Oper.Pop());
                            if (functions.Count > 0)
                            {
                                if (Expr.Last().TerminalName == functions.Peek().TerminalName)
                                {
                                    //Add the counter into Index property of token function
                                    
                                    Expr.Last().Index = functions.Peek().Index ;
                                    functions.Pop();
                                }
                               
                            }

                        }
                        break;

                    #endregion

                    case "COMMA":
                        //Add 1 to counter of subexpressions, comma means here comes another one.
                        if (functions.Count > 0) functions.Peek().Index++;
                        //Save everything down to Left Parenthesis but don't discard it
                        if (Oper.Count > 0)
                        {
                            while (Oper.Peek().TerminalName != "(")
                            {
                                Expr.Add(Oper.Pop());
                            }
                        }
                        break;

                    #region END MARKERS FOR EXPRESSION
                    case "LF":
                    case "THEN":
                    case "EOF":
                    case "REM":
                    case "ELSE":
                   

                        //Pop all operators remaining in stack.
                        //Return expression
                        while (Oper.Count > 0)
                        {
                            Expr.Add(Oper.Pop());
                        }
                        
                        Index--; //Get back, this token should be processed by parent function.
                        return Expr;
                    #endregion

                    #region Identifier
                    //TODO: Acá faltan varios tipos de identificadores, agregarlos posteriormente
                    case "VARS":
                    case "INS":
                    case "OUTS":
                    case "PRG":
                        string output = Regex.Match(tokentext, @"\d+").Value;
                        int CtrlPointIndex = Convert.ToInt16(output) - 1; //VAR1 will get index 0, and so on.
                                                                          //Prepare token identifier to encode: Token + Index + Type
                        EditorTokenInfo CPIdentifier = new EditorTokenInfo(tokentext, "Identifier");
                        switch (terminalname)
                        {
                            case "VARS":
                                CPIdentifier.Type = (short)PCODE_CONST.VARPOINTTYPE;
                                break;
                            case "INS":
                                CPIdentifier.Type = (short)PCODE_CONST.INPOINTTYPE;
                                break;
                            case "OUTS":
                                CPIdentifier.Type = (short)PCODE_CONST.OUTPOINTTYPE;
                                break;
                            default:
                                CPIdentifier.Type = (short)PCODE_CONST.UNDEFINED_SYMBOL;
                                break;
                        }
                        
                        CPIdentifier.Index = (short)CtrlPointIndex;
                        CPIdentifier.Token = (short) PCODE_CONST.LOCAL_POINT_PRG;
                        Expr.Add(CPIdentifier);
                        break;

                    case "Identifier":
                        //Locate Identifier and Identify Token associated ControlPoint.
                        //To include this info in TokenInfo.Type and update TokenInfo.TerminalName
                        int PointIndex = 0;
                        var TokenType = Encoder.GetTypeIdentifier(tokentext, out PointIndex);
                        if (TokenType == PCODE_CONST.UNDEFINED_SYMBOL)
                        {
                            //There is a semantic error here
                            //Add error message to parser and cancel renumbering.
                            //Don't break it inmediately, to show all possible errors of this type
                            _parseTree.ParserMessages.Add(new LogMessage(ErrorLevel.Error,
                                tok.Location,
                                $"Semantic Error: Undefined Identifier: {tok.Text}",
                                new ParserState("Validating Tokens")));
                            ShowCompilerErrors();
                            Cancel = true;
                        }
                        else
                        {
                            //Prepare token identifier to encode: Token + Index + Type
                            EditorTokenInfo NewIdentifier = new EditorTokenInfo(tokentext, terminalname);
                            NewIdentifier.Type = (short)TokenType;
                            NewIdentifier.Index = (short)PointIndex;
                            NewIdentifier.Token = (short)PCODE_CONST.LOCAL_POINT_PRG;
                            Expr.Add(NewIdentifier);
                        }
                        break;

                    #endregion

                    #region OPERATORS

                    case "PLUS":
                    case "MINUS":
                    case "MUL":
                    case "DIV":
                    case "POW":
                    case "MOD":
                    case "LT":
                    case "GT":
                    case "LE":
                    case "GE":
                    case "EQ":
                    case "NE":
                    case "AND":
                    case "XOR":
                    case "OR":
                    case "NOT":
                    
                        //All operators are cast directly into token of TYPE_TOKEN and with precedence attribute.
                        //To allow further transforms by RPN Parser of Expressions
                        var op= new EditorTokenInfo(tokentext, terminalname);
                        TYPE_TOKEN TypeToken = (TYPE_TOKEN)Enum.Parse(typeof(TYPE_TOKEN), terminalname.ToString().Trim());
                        op.Token = (short)TypeToken;
                        op.Precedence = (short)tok.KeyTerm.Precedence;

                        if(Oper.Count == 0)
                        {
                            Oper.Push(op);
                        }
                        else
                        {
                            while(Oper.Count > 0 && op.Precedence <= Oper.Peek().Precedence)
                            {
                                Expr.Add(Oper.Pop());
                            }

                            Oper.Push(op);
                        }
                        break;
                    #endregion

                    #region Number
                    case "Number":
                    case "CONNUMBER":
                    case "TABLENUMBER":
                    case "SYSPRG":
                    case "TIMER":
                        Expr.Add(new EditorTokenInfo(tokentext, terminalname));
                        Expr.Last().Token = (short)PCODE_CONST.CONST_VALUE_PRG;
                        break;
                    #endregion

                    #region FUNCTIONS
                    case "ABS":
                    case "INTERVAL":
                    case "_INT":
                    case "LN":
                    case "LN_1":
                    case "SQR":
                    case "_Status":
                    case "TBL":
                    case "CONPROP":
                    case "CONRATE":
                    case "CONRESET":
                    case "TIME":
                    case "TIME_ON":
                    case "TIME_OFF":
                    case "WR_ON":
                    case "WR_OFF":
                    case "DOY":
                    case "DOM":
                    case "DOW":
                    case "POWER_LOSS":
                    case "UNACK":
                    case "SCANS":
                    case "USER_A":
                    case "USER_B":

                   
                    #region Functions with variable list of expressions, must add count of expressions as last token.
                    case "AVG":
                    case "MAX":
                    case "MIN":

                        //All operators are cast directly into token of TYPE_TOKEN and with precedence attribute.
                        //To allow further transforms by RPN Parser of Expressions
                        var fxToken = new EditorTokenInfo(tokentext, terminalname);
                        FUNCTION_TOKEN tokenValue = (FUNCTION_TOKEN)Enum.Parse(typeof(FUNCTION_TOKEN), terminalname.ToString().Trim());
                        fxToken.Token = (short)tokenValue;

                        //fx.Precedence = (short)tok.KeyTerm.Precedence;
                        fxToken.Precedence = 200;
                        fxToken.Index = 1; //At least one expression to count
                        if (Oper.Count == 0)
                        {
                            Oper.Push(fxToken);
                            functions.Push(fxToken);

                        }
                        else
                        {
                            while (Oper.Count > 0 && fxToken.Precedence <= Oper.Peek().Precedence)
                            {
                                Expr.Add(Oper.Pop());
                            }

                            Oper.Push(fxToken);
                            functions.Push(fxToken);

                        }
                        break; 
                        #endregion

                        #endregion
                }

            }


            //Pop All operators remaining in stack.
            while (Oper.Count > 0)
            {
                Expr.Add(Oper.Pop());
            }

            //Check: If Expr.Count < 1 then semantic error found, expected Expression.


            Index-=1;
            return Expr;
        }

        [ComRegisterFunction()]
        public static void RegisterClass(Type type) => ComUtilities.RegisterControlClass(type);

        [ComUnregisterFunction()]
        public static void UnregisterClass(Type type) => ComUtilities.UnregisterControlClass(type);


    }

}