namespace T3000Controls
{
    using System;
    using System.Windows.Forms;
    using System.Runtime.InteropServices;

    [ComVisible(false)]
    public delegate void ValueChangedEventHandler(object sender, float newValue);

    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    public interface ISliderControlEvents
    {
        [DispId(1)]
        void TopZoneValueChanged(object sender, float newValue);

        [DispId(2)]
        void BottomZoneValueChanged(object sender, float newValue);

        [DispId(3)]
        void CurrentValueChanged(object sender, float newValue);

        [DispId(4)]
        void TopValueChanged(object sender, float newValue);

        [DispId(5)]
        void BottomValueChanged(object sender, float newValue);

        [DispId(6)]
        void MiddleZoneValueChanged(object sender, float newValue);

        [DispId(7)]
        void TopHandleMoved(object sender, float newValue);

        [DispId(8)]
        void MiddleHandleMoved(object sender, float newValue);

        [DispId(9)]
        void BottomHandleMoved(object sender, float newValue);

        #region User control events

        [DispId(-610)]
        void Click(object sender, EventArgs e);

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

        #endregion
    }
}
