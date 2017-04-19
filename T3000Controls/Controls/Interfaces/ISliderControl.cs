namespace T3000Controls
{
    using System.Drawing;

    public interface ISliderControl
    {
        float TopValue { get; set; }
        float BottomValue { get; set; }
        float CurrentValue { get; set; }
        bool TopZone { get; set; }
        bool BottomZone { get; set; }
        float TopZoneValue { get; set; }
        float BottomZoneValue { get; set; }
        float MiddleZoneValue { get; set; }
        float StepValue { get; set; }
        bool TwoSliderMode { get; set; }
        string AdditionalText { get; set; }
        Color LinesColor { get; set; }
        Color BorderColor { get; set; }
        Color TopZoneColor { get; set; }
        Color BottomZoneColor { get; set; }
        Color CurrentValueColor { get; set; }
        int BackgroundWidth { get; set; }
        int HandlesHeight { get; set; }
        Color HandlesBorderColor { get; set; }
        Color MiddleHandleColor { get; set; }
        bool LowEventMode { get; set; }
        bool MiddleZoneValueAsAverage { get; set; }
        Color IndicatorColor { get; set; }
        Color IndicatorBorderColor { get; set; }
        int IndicatorWidth { get; set; }
        int IndicatorHeight { get; set; }
        bool EnableIndicator { get; set; }
        bool IsSimpleIndicator { get; set; }
        string IndicatorText { get; set; }

        //Public methods
        bool IsInverse { get; }
        bool InTopZone { get; }
        bool InBottomZone { get; }
        string ValueToText(float value);
        string TopZoneText { get; }
        string BottomZoneText { get; }
        string MiddleZoneText { get; }
        string TopValueText { get; }
        string BottomValueText { get; }
        string CurrentValueText { get; }
        void SetRange(float topValue, float bottomValue);
        void SetZoneValues(float topZoneValue, float bottomZoneValue);
        void SetZoneValues(float topZoneValue, float middleZoneValue, float bottomZoneValue);

        //User control properties
        Color ForeColor { get; set; }
        bool Visible { get; set; }
        bool Enabled { get; set; }
        int Top { get; set; }
        int Left { get; set; }
        int Width { get; set; }
        int Height { get; set; }

        //User control methods
        void Dispose();
    }
}