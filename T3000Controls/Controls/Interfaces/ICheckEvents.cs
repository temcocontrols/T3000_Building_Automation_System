

namespace T3000Controls
{
   
    using System;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;
    //[Guid("A6EAAC3E-C8EF-483B-8498-5B5BB2B3A045")]
   // [ComVisible(false)]
    //public delegate void ValueChangedEventHandler(object sender, float newValue);

    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
   public interface ICheckEvents
    {
        #region User control events
        [DispId(1)]
        void OnCheckBoxClick(object sender, EventArgs e);

        [DispId(-610)]
        void OnClick(object sender, EventArgs e);

        [DispId(-601)]
        void DblClick(object sender, EventArgs e);

        [DispId(-602)]
        void KeyDown(object sender, KeyEventArgs e);

        [DispId(-603)]
        void KeyPress(object sender, KeyEventArgs e);

        [DispId(-604)]
        void KeyUp(object sender, KeyEventArgs e);

        [DispId(-605)]
        void MouseDown(object sender, MouseEventArgs e);

        [DispId(-606)]
        void MouseMove(object sender, MouseEventArgs e);

        [DispId(-607)]
        void MouseUp(object sender, MouseEventArgs e);

        [DispId(-620)]
        void CheckedChanged(object sender, EventArgs e);

        #endregion
    }
}
