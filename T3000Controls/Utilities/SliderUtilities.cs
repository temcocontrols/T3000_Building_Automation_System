namespace T3000Controls
{
    using System;

    internal static class SliderUtilities
    {
        public static float YToValue(float y, float topValue, float bottomValue, int height)
        {
            var delta = Math.Abs(topValue - bottomValue);
            var oneValue = delta/Math.Max(height, 1);
            var value = y*oneValue;
            
            return topValue > bottomValue ? (topValue - value) : (topValue + value);
        }

        public static float ValueToY(float value, float topValue, float bottomValue, int height)
        {
            var delta = Math.Max(Math.Abs(topValue - bottomValue), 1);
            var oneValueHeight = height/delta;
            var valueFromTop = topValue > bottomValue ? (value - bottomValue) : (value - topValue);
            var y = valueFromTop*oneValueHeight;

            return topValue > bottomValue ? height - y : y;
        }

        public static float ValueToHeight(float value, float topValue, float bottomValue, int height)
        {
            var delta = Math.Abs(topValue - bottomValue);
            var y = ValueToY(value, 0, delta, height);

            return y;
        }

        public static float GetOffsetValueForValue(float value, float topValue, float bottomValue)
        {
            if (topValue > bottomValue)
            {
                var prevValue = (float) Math.Floor(topValue/value)*value;

                return topValue - prevValue;
            }

            var nextValue = (float) Math.Ceiling(topValue/value)*value;

            return nextValue - topValue;
        }

        public static float GetOffsetForValue(float value, float topValue, float bottomValue, int height)
        {
            return ValueToHeight(
                GetOffsetValueForValue(value, topValue, bottomValue), 
                topValue, bottomValue, height);
        }

        public static float RoundUp(float value, float delta, float multiplier, int divider = 10)
        {
            return ((float)Math.Ceiling((value + delta * multiplier) / divider)) * divider;
        }

        public static float RoundDown(float value, float delta, float multiplier, int divider = 10)
        {
            return ((float)Math.Floor((value - delta * multiplier) / divider)) * divider;
        }
    }
}
