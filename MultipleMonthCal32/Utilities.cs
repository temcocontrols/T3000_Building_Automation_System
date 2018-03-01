namespace T3000.Controls.MultipleMonthCalendarControls
{
    using System;

    public static class MonthUtilities
    {
        public static int MonthDiff(DateTime time1, DateTime time2) =>
            (time2.Year - time1.Year) * 12 + time2.Month - time1.Month;

        public static int MonthLength(int year, int month) =>
            DateTime.DaysInMonth(year, month);

        public static bool IsDateEquals(DateTime time1, DateTime time2) =>
            time1.Year == time2.Year &&
            time1.Month == time2.Month &&
            time1.Day == time2.Day;

        /// <summary>
        /// Copies timestamp part only
        /// </summary>
        public static DateTime CopyTime(DateTime from, DateTime to) =>
            new DateTime(to.Year, to.Month, to.Day, from.Hour, from.Minute, from.Second);

        /// <summary>
        /// Copies date part only
        /// </summary>
        public static DateTime CopyDate(DateTime from, DateTime to) =>
            new DateTime(from.Year, from.Month, from.Day, to.Hour, to.Minute, to.Second);

        public static int CompareMonths(DateTime date1, DateTime date2)
        {
            var monthDate1 = new DateTime(date1.Year, date1.Month, 1);
            var monthDate2 = new DateTime(date2.Year, date2.Month, 1);

            return DateTime.Compare(monthDate1, monthDate2);
        }

        public static int CompareDate(DateTime date1, DateTime date2)
        {
            var monthDate1 = new DateTime(date1.Year, date1.Month, date1.Day);
            var monthDate2 = new DateTime(date2.Year, date2.Month, date2.Day);

            return DateTime.Compare(monthDate1, monthDate2);
        }

        /// <summary>
        /// Adds/subtracts 'months' from date
        /// </summary>
        /// <param name="date"></param>
        /// <param name="months"></param>
        /// <returns></returns>
        public static DateTime GetMonth(DateTime date, int months)
        {
            var m = date.Month + months;

            var newYear = date.Year + (m > 0 ? (m - 1) / 12 : m / 12 - 1);
            var newMonth = m > 0 ? (m - 1) % 12 + 1 : 12 + m % 12;

            /* fix moving from last day in a month */
            var length = MonthLength(newYear, newMonth);

            return new DateTime(newYear, newMonth, date.Day > length ? length : date.Day);
        }

        public static DateTime GetNextMonth(DateTime date) => GetMonth(date, 1);
        public static DateTime GetPrevMonth(DateTime date) => GetMonth(date, -1);

    }
}
