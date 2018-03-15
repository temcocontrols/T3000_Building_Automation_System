namespace BroveApp
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
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.getProductsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.webBrowser_Description = new System.Windows.Forms.WebBrowser();
            this.pictureBox_Image = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_ID = new System.Windows.Forms.TextBox();
            this.textBox_Name = new System.Windows.Forms.TextBox();
            this.textBox_Price = new System.Windows.Forms.TextBox();
            this.textBox_Weight = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.button_UP = new System.Windows.Forms.Button();
            this.button_Down = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Image)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(0, 28);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(651, 534);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.getProductsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1258, 25);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // getProductsToolStripMenuItem
            // 
            this.getProductsToolStripMenuItem.Name = "getProductsToolStripMenuItem";
            this.getProductsToolStripMenuItem.Size = new System.Drawing.Size(91, 21);
            this.getProductsToolStripMenuItem.Text = "GetProducts";
            this.getProductsToolStripMenuItem.Click += new System.EventHandler(this.getProductsToolStripMenuItem_Click);
            // 
            // webBrowser_Description
            // 
            this.webBrowser_Description.Location = new System.Drawing.Point(657, 305);
            this.webBrowser_Description.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser_Description.Name = "webBrowser_Description";
            this.webBrowser_Description.Size = new System.Drawing.Size(589, 272);
            this.webBrowser_Description.TabIndex = 2;
            // 
            // pictureBox_Image
            // 
            this.pictureBox_Image.Location = new System.Drawing.Point(858, 28);
            this.pictureBox_Image.Name = "pictureBox_Image";
            this.pictureBox_Image.Size = new System.Drawing.Size(372, 271);
            this.pictureBox_Image.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox_Image.TabIndex = 3;
            this.pictureBox_Image.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(657, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 19);
            this.label1.TabIndex = 4;
            this.label1.Text = "ID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(657, 94);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 19);
            this.label2.TabIndex = 5;
            this.label2.Text = "Name";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(652, 141);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(64, 19);
            this.label3.TabIndex = 6;
            this.label3.Text = "Price";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(651, 199);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 19);
            this.label4.TabIndex = 7;
            this.label4.Text = "Weight";
            // 
            // textBox_ID
            // 
            this.textBox_ID.Location = new System.Drawing.Point(716, 33);
            this.textBox_ID.Name = "textBox_ID";
            this.textBox_ID.Size = new System.Drawing.Size(136, 21);
            this.textBox_ID.TabIndex = 8;
            // 
            // textBox_Name
            // 
            this.textBox_Name.Location = new System.Drawing.Point(716, 92);
            this.textBox_Name.Name = "textBox_Name";
            this.textBox_Name.Size = new System.Drawing.Size(136, 21);
            this.textBox_Name.TabIndex = 9;
            // 
            // textBox_Price
            // 
            this.textBox_Price.Location = new System.Drawing.Point(722, 141);
            this.textBox_Price.Name = "textBox_Price";
            this.textBox_Price.Size = new System.Drawing.Size(120, 21);
            this.textBox_Price.TabIndex = 10;
            // 
            // textBox_Weight
            // 
            this.textBox_Weight.Location = new System.Drawing.Point(732, 199);
            this.textBox_Weight.Name = "textBox_Weight";
            this.textBox_Weight.Size = new System.Drawing.Size(120, 21);
            this.textBox_Weight.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(657, 280);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(130, 19);
            this.label5.TabIndex = 12;
            this.label5.Text = "Description";
            // 
            // button_UP
            // 
            this.button_UP.Location = new System.Drawing.Point(119, 568);
            this.button_UP.Name = "button_UP";
            this.button_UP.Size = new System.Drawing.Size(75, 23);
            this.button_UP.TabIndex = 14;
            this.button_UP.Text = "Up Page";
            this.button_UP.UseVisualStyleBackColor = true;
            this.button_UP.Click += new System.EventHandler(this.button_UP_Click);
            // 
            // button_Down
            // 
            this.button_Down.Location = new System.Drawing.Point(328, 568);
            this.button_Down.Name = "button_Down";
            this.button_Down.Size = new System.Drawing.Size(75, 23);
            this.button_Down.TabIndex = 15;
            this.button_Down.Text = "Down Page";
            this.button_Down.UseVisualStyleBackColor = true;
            this.button_Down.Click += new System.EventHandler(this.button_Down_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(661, 226);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(181, 43);
            this.Save.TabIndex = 16;
            this.Save.Text = "Save";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1258, 589);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.button_Down);
            this.Controls.Add(this.button_UP);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_Weight);
            this.Controls.Add(this.textBox_Price);
            this.Controls.Add(this.textBox_Name);
            this.Controls.Add(this.textBox_ID);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox_Image);
            this.Controls.Add(this.webBrowser_Description);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "BroProductsList";
            this.SizeChanged += new System.EventHandler(this.Form1_SizeChanged);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Image)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem getProductsToolStripMenuItem;
        private System.Windows.Forms.WebBrowser webBrowser_Description;
        private System.Windows.Forms.PictureBox pictureBox_Image;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_ID;
        private System.Windows.Forms.TextBox textBox_Name;
        private System.Windows.Forms.TextBox textBox_Price;
        private System.Windows.Forms.TextBox textBox_Weight;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button button_UP;
        private System.Windows.Forms.Button button_Down;
        private System.Windows.Forms.Button Save;
    }
}

