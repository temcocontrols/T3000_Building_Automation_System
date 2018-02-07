//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  License: GNU Lesser General Public License (LGPLv3)
//
//  Email: pavel_torgashov@mail.ru.
//
//  Copyright (C) Pavel Torgashov, 2013. 

using Irony.Parsing;
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Linq;

namespace FastColoredTextBoxNS
{
    /// <summary>
    /// FastColoredTextBox with Irony parser support
    /// </summary>
    ///// <see cref="http://github.com/PavelTorgashov/FastColoredTextBox"/>
    ///// <see cref="http://irony.codeplex.com/"/>
    public class IronyFCTB : FastColoredTextBox
    {
        public event EventHandler<StyleNeededEventArgs> StyleNeeded;
        protected Parser parser;
        
        public Style WavyStyle = new WavyLineStyle(255, Color.Red);
     

              


        /// <summary>
        /// Grammar of custom language
        /// </summary>
        [Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), EditorBrowsable(EditorBrowsableState.Never)]
        public Grammar Grammar {
            get {
                if (parser != null && parser.Language != null && parser.Language.Grammar != null)
                    return parser.Language.Grammar;
                return null;
            }
            set {
                SetParser(value);
            }
        }

        /// <summary>
        /// Parser of custom language
        /// </summary>
        [Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), EditorBrowsable(EditorBrowsableState.Never)]
        public Parser Parser {
            get {
          
                return parser;
            }
            set {
                SetParser(value);
            }
        }

        /// <summary>
        /// Sets Irony parser (based on Grammar)
        /// </summary>
        public virtual void SetParser(Grammar grammar)
        {
            SetParser(new LanguageData(grammar));
        }

        /// <summary>
        /// Sets Irony parser (based on LanguageData)
        /// </summary>
        public virtual void SetParser(LanguageData language)
        {
            SetParser(new Parser(language));
        }

        /// <summary>
        /// Sets Irony parser
        /// </summary>
        public virtual void SetParser(Parser parser)
        {
            this.parser = parser;
            ClearStylesBuffer();
            AddStyle(WavyStyle);

            VariablesColor = Color.Crimson ;
            InputsColor = Color.BlueViolet;
            OutputsColor = Color.DarkBlue;

            SolidBrush variablebrush = new SolidBrush(VariablesColor);
            SyntaxHighlighter.VariableStyle = new TextStyle(variablebrush, null, FontStyle.Regular);

            SyntaxHighlighter.InitStyleSchema(Language.CSharp);
            InitBraces();
            OnTextChanged(Range);
        }

        /// <summary>
        /// Auto-hightlight delayed for text change
        /// </summary>
        /// <param name="changedRange"></param>
        public override void OnTextChangedDelayed(Range changedRange)
        {
            DoHighlighting();
            base.OnTextChangedDelayed(changedRange);
        }

        protected virtual void DoHighlighting()
        {
            if (parser == null)
                return;

            //parse text
            ParseTree tree;

            try
            {
                tree = parser.Parse(Text);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message); //oops...
                return;
            }

            //highlight errors
            if (tree.Status == ParseTreeStatus.Error)
            {
                ClearStyle(GetStyleIndexMask(new Style[] { WavyStyle }));
                foreach (var msg in tree.ParserMessages)
                {
                    var loc = msg.Location;
                    var place = new Place(loc.Column, loc.Line);
                    var r = new Range(this, place, place);
                    var f = r.GetFragment(@"[\S]");
                    f.SetStyle(WavyStyle);
                }
                return;
            }

            
            SolidBrush brush1 = new SolidBrush(VariablesColor);
            SyntaxHighlighter.VariableStyle = new TextStyle(brush1, null, FontStyle.Regular);


            //highlight syntax
            ClearStyle(StyleIndex.All);
            foreach (var t in tree.Tokens)
            {
                var arg = new StyleNeededEventArgs(t);
                OnStyleNeeded(arg);

                if (arg.Cancel)
                    continue;

                if (arg.Style != null)
                {
                    GetTokenRange(t).SetStyle(arg.Style);
                    continue;
                }


                

                switch (t.Terminal.Name)
                {
                    case "INTERVAL":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.FunctionsStyle);
                        break;
                    case "Identifier":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.VariableStyle);
                        break;
                }

                switch (t.Terminal.GetType().Name)
                {
                    case "KeyTerm":
                        if ((t.Terminal.Flags & TermFlags.IsKeyword) != 0) //keywords are highlighted only
                            GetTokenRange(t).SetStyle(SyntaxHighlighter.KeywordStyle);
                        break;
                    case "FreeTextLiteral":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.CommentStyle);
                        break;
                    case "Identifier":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.VariableStyle);
                        
                        break;
                    case "NumberLiteral":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.NumberStyle);
                        break;
                    case "StringLiteral":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.StringStyle);
                        break;
                    case "CommentTerminal":
                        GetTokenRange(t).SetStyle(SyntaxHighlighter.CommentStyle);
                        break;
                }
            }
        }

        public virtual void OnStyleNeeded(StyleNeededEventArgs e) =>
            StyleNeeded?.Invoke(this, e);

        /// <summary>
        /// Returns range of token
        /// </summary>
        public Range GetTokenRange(Token t)
        {
            var loc = t.Location;

            var place = new Place(loc.Column, loc.Line);
            var r = new Range(this, place, place);

            foreach (var c in t.Text)
                if (c != '\r')
                    r.GoRight(true);

            return r;
        }

        protected virtual void InitBraces()
        {
            LeftBracket = '\x0';
            RightBracket = '\x0';
            LeftBracket2 = '\x0';
            RightBracket2 = '\x0';

            // select the first two pair of braces with the length of exactly one char (FCTB restrictions)
            var braces = parser.Language.Grammar.KeyTerms
              .Select(pair => pair.Value)
              .Where(term => term.Flags.IsSet(TermFlags.IsOpenBrace))
              .Where(term => term.IsPairFor != null && term.IsPairFor is KeyTerm)
              .Where(term => term.Text.Length == 1)
              .Where(term => ((KeyTerm)term.IsPairFor).Text.Length == 1)
              .Take(2);
            if (braces.Any())
            {
                // first pair
                var brace = braces.First();
                LeftBracket = brace.Text.First();
                RightBracket = ((KeyTerm)brace.IsPairFor).Text.First();
                // second pair
                if (braces.Count() > 1)
                {
                    brace = braces.Last();
                    LeftBracket2 = brace.Text.First();
                    RightBracket2 = ((KeyTerm)brace.IsPairFor).Text.First();
                }
            }
            else
            {
                LeftBracket = '(';
                RightBracket = ')';
            }
        }

        private void InitializeComponent()
        {
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // IronyFCTB
            // 
            this.AutoScrollMinSize = new System.Drawing.Size(27, 14);
            this.BackColor = System.Drawing.SystemColors.Control;
            
            this.Name = "IronyFCTB";
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }
    }

    /// <summary>
    /// Manages arguments for StyleNeeded Event.
    /// </summary>
    public class StyleNeededEventArgs : EventArgs
    {
        public readonly Token Token;
        public bool Cancel { get; set; }
        public Style Style { get; set; }

        /// <summary>
        /// Default constructor for StyleNeededEventArg
        /// </summary>
        /// <param name="t">Token</param>
        public StyleNeededEventArgs(Token t)
        {
            Token = t;
        }
    }
}