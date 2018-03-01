namespace T3000.Controls.MultipleMonthCalendarControls
{
    using System;
    using System.Drawing;
    using System.Windows.Forms;
    using System.ComponentModel;

    internal class DayControl : Button
    {
        #region Properties

        private bool _isSelected = false;
        [Description("Is selected"), Category("DayControl")]
        public bool IsSelected {
            get { return _isSelected; }
            set {
                if (Date.Equals(new DateTime()))
                {
                    return;
                }

                _isSelected = value;
                BackColor = value
                    ? Color.FromArgb(152, 194, 206)
                    : Color.White;
                ForeColor = value
                    ? Color.FromArgb(10, 65, 122)
                    : Color.Black;

                Invalidate();
            }
        }

        private DateTime _date = DateTime.Today;
        [Description("Date"), Category("DayControl")]
        public DateTime Date {
            get { return _date; }
            set {
                _date = value;

                var isEmpty = Date.Equals(new DateTime());
                Text = isEmpty ? "" : value.Day.ToString();
            }
        }

        #endregion

        public DayControl()
        {
            FlatStyle = FlatStyle.Flat;
            FlatAppearance.BorderSize = 0;
            Font = new Font(FontFamily.GenericSansSerif, 8);
            TextAlign = ContentAlignment.MiddleCenter;
            UseVisualStyleBackColor = false;
            Margin = new Padding(0);
            Padding = new Padding(0);
            
            ResizeRedraw = true;

            GotFocus += (sender, args) => BackColor = Color.Silver;
            LostFocus += (sender, args) => BackColor = IsSelected
                ? Color.FromArgb(152, 194, 206)
                : Color.White;
        }
    }
}
