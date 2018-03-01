namespace T3000.Controls
{
    using System;
    using System.Windows.Forms;
    using System.ComponentModel;
    using System.Collections.Generic;
    using MultipleMonthCalendarControls;

    public partial class MultipleMonthCalendar : UserControl
    {
        #region Properties
        
        [Description("Dimension X value"), Category("MultipleMonthCalendar")]
        public int DimensionX {
            get { return monthsControl.DimensionX; }
            set { monthsControl.DimensionX = value; }
        }

        [Description("Dimension Y value"), Category("MultipleMonthCalendar")]
        public int DimensionY {
            get { return monthsControl.DimensionY; }
            set { monthsControl.DimensionY = value; }
        }

        [Description("Start date"), Category("MultipleMonthCalendar")]
        public DateTime StartDate {
            get { return monthsControl.StartDate; }
            set { monthsControl.StartDate = value; }
        }
        /*
        [Description("Selected dates"), Category("MultipleMonthCalendar")]
        public List<DateTime> SelectedDates {
            get { return monthsControl.SelectedDates; }
            set { monthsControl.SelectedDates = value; }
        }
        */
        #endregion

        public MultipleMonthCalendar()
        {
            InitializeComponent();
            
            nextButton.Click += (sender, args) => 
                monthsControl.StartDate = MonthUtilities.GetNextMonth(monthsControl.StartDate);

            prevButton.Click += (sender, args) =>
                monthsControl.StartDate = MonthUtilities.GetPrevMonth(monthsControl.StartDate);
        }
    }
}
