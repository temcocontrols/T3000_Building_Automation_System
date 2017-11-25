namespace T3000.Forms
{
    using System;
    using System.Collections.Generic;

    public static class StringExtensions
    {
        public static IList<string> ToLines(this string text, 
            StringSplitOptions options = StringSplitOptions.None) =>
            text.Split(new[] { "\r\n", "\r", "\n" }, options);
    }
}
