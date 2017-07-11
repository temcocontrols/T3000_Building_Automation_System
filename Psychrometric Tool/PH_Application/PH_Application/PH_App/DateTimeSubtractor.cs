using System;
using System.Globalization;

namespace PH_App
{
    public class DateTimeSubtractor
    {
        public string SubtractTwoTime(DateTime first,DateTime Now)
        {
            TimeSpan diff =  Now- first ; //DateTime.Now - DateTime.Today;
            string formatted = "";
            //if (diff.Days != 0 && diff.Hours !=0)
            //{
                formatted = string.Format(
                                   CultureInfo.CurrentCulture,
                                   "{0}D:{1}H:{2}M:{3}S",
                                   diff.Days,
                                   diff.Hours,
                                   diff.Minutes,               
                                   diff.Seconds);
            /*
            }
            else if (diff.Days == 0 && diff.Hours == 0 && diff.Minutes == 0)
            {
                formatted = string.Format(
                                   CultureInfo.CurrentCulture,
                                   "{3} seconds",                                   
                                   diff.Seconds);
            }
            else if (diff.Days == 0 && diff.Hours == 0)
            {
                formatted = string.Format(
                                   CultureInfo.CurrentCulture,
                                   "{2} minutes, {3} seconds",                                  
                                   diff.Minutes,
                                   diff.Seconds);
            }else if(diff.Days == 0)
            {
                formatted = string.Format(
                                   CultureInfo.CurrentCulture,
                                   "{1} hours, {2} minutes, {3} seconds",
                                   
                                   diff.Hours,
                                   diff.Minutes,
                                   diff.Seconds);

            }
            */
                return formatted;
        }
    }
}
