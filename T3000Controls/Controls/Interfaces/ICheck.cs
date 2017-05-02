using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;

//[ComVisible(false)]
//public delegate void ValueChangedEventHandler(object sender, float newValue);
//[InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
namespace T3000Controls
{
  
    public interface ICheck
    {
        //Text for Label
       string TCaption { get; set; }
        Color TFontColor { get; set; }
        Color TBackColor { get; set; }
        //Text Font;
        string TFontName { get; set; }
        int TFontSize { get; set; }
        bool TBold { get; set; }
        bool TItalic { get; set; }
        bool TStrikeout { get; set; }
        bool TUnderline { get; set; }
        //Label's background Color
       bool TChecked { get; set; }
        //Label's background Image
       bool TShowPicture { get; set; }


        void SetText(string strText);
        string GetText();
       
        void SetFont(string FontName, int FontSize,
                     bool IsBold, bool Italic,
                     bool Strikeout, bool Underline);
 

        void Dispose();
        //Text Color
       

    }
}
