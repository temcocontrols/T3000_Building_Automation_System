namespace Enthalpy_calculation
{
    partial class Form1
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.btnCalculate = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radio_eu = new System.Windows.Forms.RadioButton();
            this.radio_si = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tb_b_unit = new System.Windows.Forms.TextBox();
            this.tb_x_unit = new System.Windows.Forms.TextBox();
            this.tb_h_unit = new System.Windows.Forms.TextBox();
            this.tb_pws_unit = new System.Windows.Forms.TextBox();
            this.tb_tn_unit = new System.Windows.Forms.TextBox();
            this.tb_m_unit = new System.Windows.Forms.TextBox();
            this.tb_a_unit = new System.Windows.Forms.TextBox();
            this.tb_td_unit = new System.Windows.Forms.TextBox();
            this.tb_atm_unit = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.tb_temp_unit = new System.Windows.Forms.TextBox();
            this.tb_h = new System.Windows.Forms.TextBox();
            this.lb_h = new System.Windows.Forms.Label();
            this.tb_pws = new System.Windows.Forms.TextBox();
            this.tb_tn = new System.Windows.Forms.TextBox();
            this.tb_m = new System.Windows.Forms.TextBox();
            this.tb_x = new System.Windows.Forms.TextBox();
            this.tb_b = new System.Windows.Forms.TextBox();
            this.tb_a = new System.Windows.Forms.TextBox();
            this.tb_td = new System.Windows.Forms.TextBox();
            this.tb_atm = new System.Windows.Forms.TextBox();
            this.tb_humi = new System.Windows.Forms.TextBox();
            this.tb_temp = new System.Windows.Forms.TextBox();
            this.lb_x = new System.Windows.Forms.Label();
            this.lb_pws = new System.Windows.Forms.Label();
            this.lb_b = new System.Windows.Forms.Label();
            this.lb_tn = new System.Windows.Forms.Label();
            this.lb_m = new System.Windows.Forms.Label();
            this.lb_a = new System.Windows.Forms.Label();
            this.lb_td = new System.Windows.Forms.Label();
            this.lb_atm = new System.Windows.Forms.Label();
            this.lb_humi = new System.Windows.Forms.Label();
            this.lb_temp = new System.Windows.Forms.Label();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCalculate
            // 
            this.btnCalculate.Location = new System.Drawing.Point(208, 438);
            this.btnCalculate.Name = "btnCalculate";
            this.btnCalculate.Size = new System.Drawing.Size(111, 36);
            this.btnCalculate.TabIndex = 20;
            this.btnCalculate.Text = "&Calculate";
            this.btnCalculate.UseVisualStyleBackColor = true;
            this.btnCalculate.Click += new System.EventHandler(this.btnCalculate_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radio_eu);
            this.groupBox1.Controls.Add(this.radio_si);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(293, 57);
            this.groupBox1.TabIndex = 39;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Unit Selection";
            // 
            // radio_eu
            // 
            this.radio_eu.AutoSize = true;
            this.radio_eu.Location = new System.Drawing.Point(172, 20);
            this.radio_eu.Name = "radio_eu";
            this.radio_eu.Size = new System.Drawing.Size(40, 17);
            this.radio_eu.TabIndex = 38;
            this.radio_eu.TabStop = true;
            this.radio_eu.Text = "EU";
            this.radio_eu.UseVisualStyleBackColor = true;
            // 
            // radio_si
            // 
            this.radio_si.AutoSize = true;
            this.radio_si.Checked = true;
            this.radio_si.Location = new System.Drawing.Point(46, 20);
            this.radio_si.Name = "radio_si";
            this.radio_si.Size = new System.Drawing.Size(35, 17);
            this.radio_si.TabIndex = 37;
            this.radio_si.TabStop = true;
            this.radio_si.Text = "SI";
            this.radio_si.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tb_b_unit);
            this.groupBox2.Controls.Add(this.tb_x_unit);
            this.groupBox2.Controls.Add(this.tb_h_unit);
            this.groupBox2.Controls.Add(this.tb_pws_unit);
            this.groupBox2.Controls.Add(this.tb_tn_unit);
            this.groupBox2.Controls.Add(this.tb_m_unit);
            this.groupBox2.Controls.Add(this.tb_a_unit);
            this.groupBox2.Controls.Add(this.tb_td_unit);
            this.groupBox2.Controls.Add(this.tb_atm_unit);
            this.groupBox2.Controls.Add(this.textBox2);
            this.groupBox2.Controls.Add(this.tb_temp_unit);
            this.groupBox2.Controls.Add(this.tb_h);
            this.groupBox2.Controls.Add(this.lb_h);
            this.groupBox2.Controls.Add(this.tb_pws);
            this.groupBox2.Controls.Add(this.tb_tn);
            this.groupBox2.Controls.Add(this.tb_m);
            this.groupBox2.Controls.Add(this.tb_x);
            this.groupBox2.Controls.Add(this.tb_b);
            this.groupBox2.Controls.Add(this.tb_a);
            this.groupBox2.Controls.Add(this.tb_td);
            this.groupBox2.Controls.Add(this.tb_atm);
            this.groupBox2.Controls.Add(this.tb_humi);
            this.groupBox2.Controls.Add(this.tb_temp);
            this.groupBox2.Controls.Add(this.lb_x);
            this.groupBox2.Controls.Add(this.lb_pws);
            this.groupBox2.Controls.Add(this.lb_b);
            this.groupBox2.Controls.Add(this.lb_tn);
            this.groupBox2.Controls.Add(this.lb_m);
            this.groupBox2.Controls.Add(this.lb_a);
            this.groupBox2.Controls.Add(this.lb_td);
            this.groupBox2.Controls.Add(this.lb_atm);
            this.groupBox2.Controls.Add(this.lb_humi);
            this.groupBox2.Controls.Add(this.lb_temp);
            this.groupBox2.Location = new System.Drawing.Point(12, 75);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(451, 351);
            this.groupBox2.TabIndex = 40;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Calculation";
            // 
            // tb_b_unit
            // 
            this.tb_b_unit.Location = new System.Drawing.Point(332, 248);
            this.tb_b_unit.Name = "tb_b_unit";
            this.tb_b_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_b_unit.TabIndex = 66;
            this.tb_b_unit.Text = "g/kg";
            this.tb_b_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_x_unit
            // 
            this.tb_x_unit.Location = new System.Drawing.Point(332, 281);
            this.tb_x_unit.Name = "tb_x_unit";
            this.tb_x_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_x_unit.TabIndex = 65;
            this.tb_x_unit.Text = "g/kg";
            this.tb_x_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_h_unit
            // 
            this.tb_h_unit.Location = new System.Drawing.Point(332, 313);
            this.tb_h_unit.Name = "tb_h_unit";
            this.tb_h_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_h_unit.TabIndex = 64;
            this.tb_h_unit.Text = "kJ/kg";
            this.tb_h_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_pws_unit
            // 
            this.tb_pws_unit.Location = new System.Drawing.Point(332, 211);
            this.tb_pws_unit.Name = "tb_pws_unit";
            this.tb_pws_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_pws_unit.TabIndex = 63;
            this.tb_pws_unit.Text = "hPa";
            this.tb_pws_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_tn_unit
            // 
            this.tb_tn_unit.Location = new System.Drawing.Point(332, 185);
            this.tb_tn_unit.Name = "tb_tn_unit";
            this.tb_tn_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_tn_unit.TabIndex = 62;
            this.tb_tn_unit.Text = "unit less";
            this.tb_tn_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_m_unit
            // 
            this.tb_m_unit.Location = new System.Drawing.Point(332, 158);
            this.tb_m_unit.Name = "tb_m_unit";
            this.tb_m_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_m_unit.TabIndex = 61;
            this.tb_m_unit.Text = "unit less";
            this.tb_m_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_a_unit
            // 
            this.tb_a_unit.Location = new System.Drawing.Point(332, 131);
            this.tb_a_unit.Name = "tb_a_unit";
            this.tb_a_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_a_unit.TabIndex = 60;
            this.tb_a_unit.Text = "unit less ";
            this.tb_a_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_td_unit
            // 
            this.tb_td_unit.Location = new System.Drawing.Point(332, 106);
            this.tb_td_unit.Name = "tb_td_unit";
            this.tb_td_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_td_unit.TabIndex = 59;
            this.tb_td_unit.Text = "DegC";
            this.tb_td_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_atm_unit
            // 
            this.tb_atm_unit.Location = new System.Drawing.Point(332, 81);
            this.tb_atm_unit.Name = "tb_atm_unit";
            this.tb_atm_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_atm_unit.TabIndex = 58;
            this.tb_atm_unit.Text = "Hpa";
            this.tb_atm_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(332, 58);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 20);
            this.textBox2.TabIndex = 57;
            this.textBox2.Text = "%";
            this.textBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_temp_unit
            // 
            this.tb_temp_unit.Location = new System.Drawing.Point(332, 29);
            this.tb_temp_unit.Name = "tb_temp_unit";
            this.tb_temp_unit.Size = new System.Drawing.Size(100, 20);
            this.tb_temp_unit.TabIndex = 56;
            this.tb_temp_unit.Text = "C";
            this.tb_temp_unit.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tb_h
            // 
            this.tb_h.Location = new System.Drawing.Point(206, 310);
            this.tb_h.Name = "tb_h";
            this.tb_h.Size = new System.Drawing.Size(100, 20);
            this.tb_h.TabIndex = 55;
            // 
            // lb_h
            // 
            this.lb_h.AutoSize = true;
            this.lb_h.Location = new System.Drawing.Point(8, 313);
            this.lb_h.Name = "lb_h";
            this.lb_h.Size = new System.Drawing.Size(48, 13);
            this.lb_h.TabIndex = 54;
            this.lb_h.Text = "Enthalpy";
            // 
            // tb_pws
            // 
            this.tb_pws.Location = new System.Drawing.Point(207, 215);
            this.tb_pws.Name = "tb_pws";
            this.tb_pws.Size = new System.Drawing.Size(100, 20);
            this.tb_pws.TabIndex = 53;
            // 
            // tb_tn
            // 
            this.tb_tn.Location = new System.Drawing.Point(206, 184);
            this.tb_tn.Name = "tb_tn";
            this.tb_tn.Size = new System.Drawing.Size(100, 20);
            this.tb_tn.TabIndex = 52;
            this.tb_tn.Text = "240.7263";
            // 
            // tb_m
            // 
            this.tb_m.Location = new System.Drawing.Point(206, 156);
            this.tb_m.Name = "tb_m";
            this.tb_m.Size = new System.Drawing.Size(100, 20);
            this.tb_m.TabIndex = 51;
            this.tb_m.Text = "7.591386";
            // 
            // tb_x
            // 
            this.tb_x.Location = new System.Drawing.Point(206, 280);
            this.tb_x.Name = "tb_x";
            this.tb_x.Size = new System.Drawing.Size(100, 20);
            this.tb_x.TabIndex = 50;
            this.tb_x.Text = "621.9907";
            // 
            // tb_b
            // 
            this.tb_b.Location = new System.Drawing.Point(206, 246);
            this.tb_b.Name = "tb_b";
            this.tb_b.Size = new System.Drawing.Size(100, 20);
            this.tb_b.TabIndex = 49;
            // 
            // tb_a
            // 
            this.tb_a.Location = new System.Drawing.Point(206, 133);
            this.tb_a.Name = "tb_a";
            this.tb_a.Size = new System.Drawing.Size(100, 20);
            this.tb_a.TabIndex = 48;
            this.tb_a.Text = "6.116441";
            // 
            // tb_td
            // 
            this.tb_td.Location = new System.Drawing.Point(206, 112);
            this.tb_td.Name = "tb_td";
            this.tb_td.Size = new System.Drawing.Size(100, 20);
            this.tb_td.TabIndex = 47;
            // 
            // tb_atm
            // 
            this.tb_atm.Location = new System.Drawing.Point(207, 88);
            this.tb_atm.Name = "tb_atm";
            this.tb_atm.Size = new System.Drawing.Size(100, 20);
            this.tb_atm.TabIndex = 46;
            this.tb_atm.Text = "1013";
            // 
            // tb_humi
            // 
            this.tb_humi.Location = new System.Drawing.Point(207, 56);
            this.tb_humi.Name = "tb_humi";
            this.tb_humi.Size = new System.Drawing.Size(100, 20);
            this.tb_humi.TabIndex = 45;
            // 
            // tb_temp
            // 
            this.tb_temp.Location = new System.Drawing.Point(206, 22);
            this.tb_temp.Name = "tb_temp";
            this.tb_temp.Size = new System.Drawing.Size(100, 20);
            this.tb_temp.TabIndex = 44;
            // 
            // lb_x
            // 
            this.lb_x.AutoSize = true;
            this.lb_x.Location = new System.Drawing.Point(8, 278);
            this.lb_x.Name = "lb_x";
            this.lb_x.Size = new System.Drawing.Size(91, 13);
            this.lb_x.TabIndex = 43;
            this.lb_x.Text = "Mixed Gas Ratio :";
            // 
            // lb_pws
            // 
            this.lb_pws.AutoSize = true;
            this.lb_pws.Location = new System.Drawing.Point(8, 218);
            this.lb_pws.Name = "lb_pws";
            this.lb_pws.Size = new System.Drawing.Size(184, 13);
            this.lb_pws.TabIndex = 42;
            this.lb_pws.Text = "Partial Pressure of water at Saturation";
            // 
            // lb_b
            // 
            this.lb_b.AutoSize = true;
            this.lb_b.Location = new System.Drawing.Point(7, 249);
            this.lb_b.Name = "lb_b";
            this.lb_b.Size = new System.Drawing.Size(187, 13);
            this.lb_b.TabIndex = 41;
            this.lb_b.Text = "Molecular mass of water over mol wt. :";
            // 
            // lb_tn
            // 
            this.lb_tn.AutoSize = true;
            this.lb_tn.Location = new System.Drawing.Point(7, 188);
            this.lb_tn.Name = "lb_tn";
            this.lb_tn.Size = new System.Drawing.Size(26, 13);
            this.lb_tn.TabIndex = 40;
            this.lb_tn.Text = "Tn :";
            // 
            // lb_m
            // 
            this.lb_m.AutoSize = true;
            this.lb_m.Location = new System.Drawing.Point(7, 161);
            this.lb_m.Name = "lb_m";
            this.lb_m.Size = new System.Drawing.Size(16, 13);
            this.lb_m.TabIndex = 39;
            this.lb_m.Text = "M";
            // 
            // lb_a
            // 
            this.lb_a.AutoSize = true;
            this.lb_a.Location = new System.Drawing.Point(8, 136);
            this.lb_a.Name = "lb_a";
            this.lb_a.Size = new System.Drawing.Size(20, 13);
            this.lb_a.TabIndex = 38;
            this.lb_a.Text = "A :";
            // 
            // lb_td
            // 
            this.lb_td.AutoSize = true;
            this.lb_td.Location = new System.Drawing.Point(7, 115);
            this.lb_td.Name = "lb_td";
            this.lb_td.Size = new System.Drawing.Size(125, 13);
            this.lb_td.TabIndex = 37;
            this.lb_td.Text = "Dew Point Temperature :";
            // 
            // lb_atm
            // 
            this.lb_atm.AutoSize = true;
            this.lb_atm.Location = new System.Drawing.Point(7, 91);
            this.lb_atm.Name = "lb_atm";
            this.lb_atm.Size = new System.Drawing.Size(115, 13);
            this.lb_atm.TabIndex = 36;
            this.lb_atm.Text = "Atmospheric Pressure :";
            // 
            // lb_humi
            // 
            this.lb_humi.AutoSize = true;
            this.lb_humi.Location = new System.Drawing.Point(8, 59);
            this.lb_humi.Name = "lb_humi";
            this.lb_humi.Size = new System.Drawing.Size(53, 13);
            this.lb_humi.TabIndex = 35;
            this.lb_humi.Text = "Humidity :";
            // 
            // lb_temp
            // 
            this.lb_temp.AutoSize = true;
            this.lb_temp.Location = new System.Drawing.Point(8, 25);
            this.lb_temp.Name = "lb_temp";
            this.lb_temp.Size = new System.Drawing.Size(73, 13);
            this.lb_temp.TabIndex = 34;
            this.lb_temp.Text = "Temperature :";
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(50, 438);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(110, 36);
            this.btnLoad.TabIndex = 41;
            this.btnLoad.Text = "&Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(344, 438);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(111, 36);
            this.btnReset.TabIndex = 42;
            this.btnReset.Text = "&Reset";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // chart1
            // 
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(499, 43);
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series1.IsXValueIndexed = true;
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            series1.XValueMember = "1";
            series1.YValueMembers = "10";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(714, 552);
            this.chart1.TabIndex = 43;
            this.chart1.Text = "chart1";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(50, 490);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(110, 36);
            this.button1.TabIndex = 44;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1300, 750);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnCalculate);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Enthalpy calculation";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCalculate;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radio_eu;
        private System.Windows.Forms.RadioButton radio_si;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tb_b_unit;
        private System.Windows.Forms.TextBox tb_x_unit;
        private System.Windows.Forms.TextBox tb_h_unit;
        private System.Windows.Forms.TextBox tb_pws_unit;
        private System.Windows.Forms.TextBox tb_tn_unit;
        private System.Windows.Forms.TextBox tb_m_unit;
        private System.Windows.Forms.TextBox tb_a_unit;
        private System.Windows.Forms.TextBox tb_td_unit;
        private System.Windows.Forms.TextBox tb_atm_unit;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox tb_temp_unit;
        private System.Windows.Forms.TextBox tb_h;
        private System.Windows.Forms.Label lb_h;
        private System.Windows.Forms.TextBox tb_pws;
        private System.Windows.Forms.TextBox tb_tn;
        private System.Windows.Forms.TextBox tb_m;
        private System.Windows.Forms.TextBox tb_x;
        private System.Windows.Forms.TextBox tb_b;
        private System.Windows.Forms.TextBox tb_a;
        private System.Windows.Forms.TextBox tb_td;
        private System.Windows.Forms.TextBox tb_atm;
        private System.Windows.Forms.TextBox tb_humi;
        private System.Windows.Forms.TextBox tb_temp;
        private System.Windows.Forms.Label lb_x;
        private System.Windows.Forms.Label lb_pws;
        private System.Windows.Forms.Label lb_b;
        private System.Windows.Forms.Label lb_tn;
        private System.Windows.Forms.Label lb_m;
        private System.Windows.Forms.Label lb_a;
        private System.Windows.Forms.Label lb_td;
        private System.Windows.Forms.Label lb_atm;
        private System.Windows.Forms.Label lb_humi;
        private System.Windows.Forms.Label lb_temp;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Button button1;
    }
}

