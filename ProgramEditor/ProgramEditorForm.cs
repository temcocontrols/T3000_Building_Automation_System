namespace T3000.Forms
{
    using FastColoredTextBoxNS;
    using System;
    using System.Linq;
    using System.Collections.Generic;
    using System.Windows.Forms;
    using Irony;
    using Irony.Parsing;
    using System.Drawing;
    using System.ComponentModel;
    using PRGReaderLibrary;

  

    public partial class ProgramEditorForm : Form
    {
        public string Code { get; set; }
        Grammar _grammar;
        LanguageData _language;
        ParseTree _parseTree;
        Parser _parser;
        //SyntaxSettings _syntaxcolors;

        public ProgramEditorForm()
        {
            InitializeComponent();

            editTextBox.Grammar = new T3000Grammar();
            //LRUIZ :: Enable a new set of grammar, language and parser, to get Program Code Errors
            _grammar = new T3000Grammar();
            _language = new LanguageData(_grammar);
            _parser = new Parser(_language);
            //LRUIZ

            var items = new List<AutocompleteItem>();
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

            foreach (var item in keywords)
                items.Add(new AutocompleteItem(item) { ImageIndex = 1 });

            var snippets = new[]{
                "if(^)\n{\n}",
                "if(^)\n{\n}\nelse\n{\n}",
                "for(^;;)\n{\n}", "while(^)\n{\n}",
                "do${\n^}while();",
                "switch(^)\n{\n\tcase : break;\n}"
            };
            foreach (var item in snippets)
                items.Add(new SnippetAutocompleteItem(item) { ImageIndex = 1 });

            //set as autocomplete source
            //autocompleteMenu.Items.SetAutocompleteItems(items);

            this.WindowState = FormWindowState.Maximized;

          
            
        }

        public ProgramEditorForm(string code) : this()
        {
            this.Text = "Edit Code: ";
            SetCode(code);
        }

        public ProgramEditorForm (Prg CurrentPrg, int Index) : this()
        {
            
            ProgramCode prgcode = CurrentPrg.ProgramCodes[Index];
            //CurrentPrg.Programs[Index].Description
            this.Text = "Edit Code: " +  CurrentPrg.Programs[Index].Label ;
            SetCode(prgcode.Code.ToString());
        }



        private string RemoveInitialNumbers(string text)
        {
            var lines = text.ToLines();
            for (var i = 0; i < lines.Count; ++i)
            {
                var words = lines[i].Split(' ');
                if (words.Length < 2)
                {
                    continue;
                }

                words = words.Skip(1).ToArray();
                lines[i] = string.Join(' '.ToString(), words);
            }

            return string.Join(Environment.NewLine, lines);
        }

        private string AddInitialNumbers(string text)
        {
            var lines = text.ToLines();
            for (var i = 0; i < lines.Count; ++i)
            {
                lines[i] = $"{(i + 1) * 10} {lines[i]}";
            }

            return string.Join(Environment.NewLine, lines);
        }

        private void SetCode(string code)
        {
            Code = code;
            //editTextBox.Text = RemoveInitialNumbers(code);
            editTextBox.Text = Code;
            //LRUIZ: Parse and show compile errors

            ParseCode();

        }

        
        private void cmdClear_Click(object sender, EventArgs e)
        {
            ClearCode();
        }

        private void ClearCode()
        {
            //local member Code is not cleared, to allow recovering with REFRESH (F8)
            editTextBox.Text = "";
        }

        private void ParseCode()
        {
            ClearParserOutput();
            if (_parser == null || !_parser.Language.CanParse()) return;
            _parseTree = null;
            GC.Collect(); //to avoid disruption of perf times with occasional collections
            _parser.Context.TracingEnabled = true;
            try
            {
                _parser.Parse(editTextBox.Text, "<source>");
            }
            catch (Exception ex)
            {
                gridCompileErrors.Rows.Add(null, ex.Message, null);
                
                throw;
            }
            finally
            {
                _parseTree = _parser.Context.CurrentParseTree;
                ShowCompilerErrors();

                //TODO: Show Compile Stats

                //    ShowCompileStats();
               
            }
        }

        private void ClearParserOutput()
        {

            //TODO: Add this stats labels to bottom of this form.

            //lblSrcLineCount.Text = string.Empty;
            //lblSrcTokenCount.Text = "";
            //lblParseTime.Text = "";
            //lblParseErrorCount.Text = "";

            //lstTokens.Items.Clear();
            gridCompileErrors.Rows.Clear();
           
            Application.DoEvents();
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
            ParseCode();
        }

        private void cmdRefresh_Click(object sender, EventArgs e)
        {
            RefreshCode();
        }


        private void RefreshCode()
        {
            editTextBox.Text = Code;
        }
        private void ProgramEditorForm_KeyDown(object sender, KeyEventArgs e)
        {
            switch(e.KeyCode )
            {
                case Keys.F2:
                    SendCode();break;
                case Keys.F4:
                    ClearCode(); break;
                case Keys.F6:
                    SaveFile();break;
                case Keys.F7:
                    LoadFile(); break;
                case Keys.F8:
                    RefreshCode();break;

            }//switch.
        }

        private void cmdLoad_Click(object sender, EventArgs e)
        {
            LoadFile();
        }

        /// <summary>
        /// Load a text file into editor
        /// </summary>
        private void LoadFile()
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

        private void cmdSend_Click(object sender, EventArgs e)
        {
            SendCode();
        }

        private void SendCode()
        {
            Code = editTextBox.Text;
        }

        private void cmdSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void SaveFile()
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


    //public class SyntaxSettings
    //    {
    //        Color mTextColor ;

    //        [Browsable(true)]                         // this property should be visible
    //        [ReadOnly(false)]                          // but just read only
    //        [Description("Text Color")]             // sample hint1
    //        [Category("Syntax Coloring")]                   // Category that I want
    //        [DisplayName("Default Text Color")]       // I want to say more, than just DisplayInt
    //        public Color TextColor
    //        {
    //            get { return mTextColor; }
    //            set { mTextColor = value; }

    //        }

    //    }

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
    }


}
