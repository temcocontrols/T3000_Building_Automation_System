namespace WFA_psychometric_chart
{
    partial class form_app_timer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(form_app_timer));
            this.checkedListBox1 = new System.Windows.Forms.CheckedListBox();
            this.label9 = new System.Windows.Forms.Label();
            this.dtp2 = new System.Windows.Forms.DateTimePicker();
            this.dtp1 = new System.Windows.Forms.DateTimePicker();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.gb_select_time_and_date = new System.Windows.Forms.GroupBox();
            this.gb_select_time_and_date.SuspendLayout();
            this.SuspendLayout();
            // 
            // checkedListBox1
            // 
            this.checkedListBox1.FormattingEnabled = true;
            this.checkedListBox1.Items.AddRange(new object[] {
            resources.GetString("checkedListBox1.Items"),
            resources.GetString("checkedListBox1.Items1"),
            resources.GetString("checkedListBox1.Items2"),
            resources.GetString("checkedListBox1.Items3"),
            resources.GetString("checkedListBox1.Items4"),
            resources.GetString("checkedListBox1.Items5"),
            resources.GetString("checkedListBox1.Items6"),
            resources.GetString("checkedListBox1.Items7"),
            resources.GetString("checkedListBox1.Items8"),
            resources.GetString("checkedListBox1.Items9"),
            resources.GetString("checkedListBox1.Items10"),
            resources.GetString("checkedListBox1.Items11"),
            resources.GetString("checkedListBox1.Items12"),
            resources.GetString("checkedListBox1.Items13"),
            resources.GetString("checkedListBox1.Items14"),
            resources.GetString("checkedListBox1.Items15"),
            resources.GetString("checkedListBox1.Items16"),
            resources.GetString("checkedListBox1.Items17"),
            resources.GetString("checkedListBox1.Items18"),
            resources.GetString("checkedListBox1.Items19"),
            resources.GetString("checkedListBox1.Items20"),
            resources.GetString("checkedListBox1.Items21"),
            resources.GetString("checkedListBox1.Items22"),
            resources.GetString("checkedListBox1.Items23"),
            resources.GetString("checkedListBox1.Items24")});
            resources.ApplyResources(this.checkedListBox1, "checkedListBox1");
            this.checkedListBox1.Name = "checkedListBox1";
            this.checkedListBox1.SelectedIndexChanged += new System.EventHandler(this.checkedListBox1_SelectedIndexChanged);
            // 
            // label9
            // 
            resources.ApplyResources(this.label9, "label9");
            this.label9.Name = "label9";
            // 
            // dtp2
            // 
            this.dtp2.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            resources.ApplyResources(this.dtp2, "dtp2");
            this.dtp2.Name = "dtp2";
            this.dtp2.Value = new System.DateTime(2016, 3, 17, 23, 14, 2, 0);
            this.dtp2.ValueChanged += new System.EventHandler(this.dtp2_event_onValue_Change);
            // 
            // dtp1
            // 
            resources.ApplyResources(this.dtp1, "dtp1");
            this.dtp1.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtp1.Name = "dtp1";
            this.dtp1.Value = new System.DateTime(2016, 1, 1, 0, 0, 0, 0);
            this.dtp1.ValueChanged += new System.EventHandler(this.dtp1_event_onValue_Change);
            // 
            // label8
            // 
            resources.ApplyResources(this.label8, "label8");
            this.label8.Name = "label8";
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // gb_select_time_and_date
            // 
            this.gb_select_time_and_date.Controls.Add(this.checkedListBox1);
            this.gb_select_time_and_date.Controls.Add(this.label7);
            this.gb_select_time_and_date.Controls.Add(this.label8);
            this.gb_select_time_and_date.Controls.Add(this.dtp1);
            this.gb_select_time_and_date.Controls.Add(this.label9);
            this.gb_select_time_and_date.Controls.Add(this.dtp2);
            resources.ApplyResources(this.gb_select_time_and_date, "gb_select_time_and_date");
            this.gb_select_time_and_date.Name = "gb_select_time_and_date";
            this.gb_select_time_and_date.TabStop = false;
            // 
            // form_app_timer
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gb_select_time_and_date);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "form_app_timer";
            this.Load += new System.EventHandler(this.form_app_timer_Load);
            this.gb_select_time_and_date.ResumeLayout(false);
            this.gb_select_time_and_date.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.DateTimePicker dtp1;
        private System.Windows.Forms.DateTimePicker dtp2;
        private System.Windows.Forms.CheckedListBox checkedListBox1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox gb_select_time_and_date;
    }
}