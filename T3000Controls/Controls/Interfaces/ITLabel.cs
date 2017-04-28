using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace T3000Controls
{

    public interface ITLabel
    {
        //Text for Label
        string TCaption { get; set; }
        void SetText(string strText);
        string GetText();
        void Dispose();
        //Text Color
        Color TTextColor { get; set; }
        //Text Font;
        string TFontName { get; set; }
        int TFontSize { get; set; }
        bool TIsBold { get; set; }
        bool TIsItalic { get; set; }
        bool TIsStrikeout { get; set; }
        bool TIsUnderline { get; set; }
        //Label's background Color
        Color TBackColor { get; set; }
        //Label's background Image
        string TImgPath { get; set; }
        //Label's Size
        float TBorderSize { get; set; }
        Color TBorderColor { get; set; }
        int TWidth { get; set; }
        int THeight { get; set; }
        bool TVisible { get; set; }
        bool TEnabled { get; set; }
        bool TAutoSize { get; set; }
        bool TShowBorder { get; set; }
         
    }
}
