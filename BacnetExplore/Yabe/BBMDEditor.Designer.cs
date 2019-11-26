namespace Yabe
{
    partial class BBMDEditor
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BBMDEditor));
            this.bpWrite = new System.Windows.Forms.Button();
            this.lbBBMDlInfo = new System.Windows.Forms.Label();
            this.tabCtrl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.BBMDTable = new System.Windows.Forms.DataGridView();
            this.IPAddress = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Port = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Mask = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.bpRefresh = new System.Windows.Forms.Button();
            this.FDRTable = new System.Windows.Forms.DataGridView();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.lbFDRInfo = new System.Windows.Forms.Label();
            this.IPendpoint = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TTL = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.RemainTTL = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tabCtrl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BBMDTable)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FDRTable)).BeginInit();
            this.SuspendLayout();
            // 
            // bpWrite
            // 
            this.bpWrite.Location = new System.Drawing.Point(107, 155);
            this.bpWrite.Name = "bpWrite";
            this.bpWrite.Size = new System.Drawing.Size(124, 23);
            this.bpWrite.TabIndex = 1;
            this.bpWrite.Text = "Write && Read back";
            this.bpWrite.UseVisualStyleBackColor = true;
            this.bpWrite.Click += new System.EventHandler(this.bpWrite_Click);
            // 
            // lbBBMDlInfo
            // 
            this.lbBBMDlInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbBBMDlInfo.ForeColor = System.Drawing.Color.Red;
            this.lbBBMDlInfo.Location = new System.Drawing.Point(31, 246);
            this.lbBBMDlInfo.Name = "lbBBMDlInfo";
            this.lbBBMDlInfo.Size = new System.Drawing.Size(333, 23);
            this.lbBBMDlInfo.TabIndex = 3;
            this.lbBBMDlInfo.Text = "Read Broadcast Table without response";
            this.lbBBMDlInfo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tabCtrl
            // 
            this.tabCtrl.Controls.Add(this.tabPage1);
            this.tabCtrl.Controls.Add(this.tabPage2);
            this.tabCtrl.Location = new System.Drawing.Point(21, 27);
            this.tabCtrl.Name = "tabCtrl";
            this.tabCtrl.SelectedIndex = 0;
            this.tabCtrl.Size = new System.Drawing.Size(343, 210);
            this.tabCtrl.TabIndex = 6;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.BBMDTable);
            this.tabPage1.Controls.Add(this.bpWrite);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(335, 184);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "BroadCast Table";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // BBMDTable
            // 
            this.BBMDTable.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.BBMDTable.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IPAddress,
            this.Port,
            this.Mask});
            this.BBMDTable.Location = new System.Drawing.Point(0, 0);
            this.BBMDTable.Name = "BBMDTable";
            this.BBMDTable.Size = new System.Drawing.Size(333, 149);
            this.BBMDTable.TabIndex = 1;
            // 
            // IPAddress
            // 
            this.IPAddress.HeaderText = "IP Address";
            this.IPAddress.Name = "IPAddress";
            this.IPAddress.Width = 120;
            // 
            // Port
            // 
            this.Port.HeaderText = "Port";
            this.Port.Name = "Port";
            this.Port.Width = 50;
            // 
            // Mask
            // 
            this.Mask.HeaderText = "Mask";
            this.Mask.Name = "Mask";
            this.Mask.Width = 120;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.bpRefresh);
            this.tabPage2.Controls.Add(this.FDRTable);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(335, 184);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "FDR Table";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // bpRefresh
            // 
            this.bpRefresh.Location = new System.Drawing.Point(128, 155);
            this.bpRefresh.Name = "bpRefresh";
            this.bpRefresh.Size = new System.Drawing.Size(75, 23);
            this.bpRefresh.TabIndex = 7;
            this.bpRefresh.Text = "Refresh";
            this.bpRefresh.UseVisualStyleBackColor = true;
            this.bpRefresh.Click += new System.EventHandler(this.bpRefresh_Click);
            // 
            // FDRTable
            // 
            this.FDRTable.AllowUserToAddRows = false;
            this.FDRTable.AllowUserToDeleteRows = false;
            this.FDRTable.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.FDRTable.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IPendpoint,
            this.TTL,
            this.RemainTTL});
            this.FDRTable.Location = new System.Drawing.Point(3, 3);
            this.FDRTable.Name = "FDRTable";
            this.FDRTable.ReadOnly = true;
            this.FDRTable.Size = new System.Drawing.Size(336, 149);
            this.FDRTable.TabIndex = 6;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // ldlFDRInfo
            // 
            this.lbFDRInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbFDRInfo.ForeColor = System.Drawing.Color.Red;
            this.lbFDRInfo.Location = new System.Drawing.Point(31, 269);
            this.lbFDRInfo.Name = "ldlFDRInfo";
            this.lbFDRInfo.Size = new System.Drawing.Size(333, 23);
            this.lbFDRInfo.TabIndex = 7;
            this.lbFDRInfo.Text = "Read FDR Table without response";
            this.lbFDRInfo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // IPendpoint
            // 
            this.IPendpoint.HeaderText = "EndPoint";
            this.IPendpoint.Name = "IPendpoint";
            this.IPendpoint.ReadOnly = true;
            this.IPendpoint.Width = 150;
            // 
            // TTL
            // 
            this.TTL.HeaderText = "TTL";
            this.TTL.Name = "TTL";
            this.TTL.ReadOnly = true;
            this.TTL.Width = 70;
            // 
            // RemainTTL
            // 
            this.RemainTTL.HeaderText = "Remain";
            this.RemainTTL.Name = "RemainTTL";
            this.RemainTTL.ReadOnly = true;
            this.RemainTTL.Width = 70;
            // 
            // BBMDEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 306);
            this.Controls.Add(this.lbFDRInfo);
            this.Controls.Add(this.tabCtrl);
            this.Controls.Add(this.lbBBMDlInfo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "BBMDEditor";
            this.Text = "BBMD Tables Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BBMDEditor_FormClosing);
            this.Load += new System.EventHandler(this.BBMDEditor_Load);
            this.tabCtrl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.BBMDTable)).EndInit();
            this.tabPage2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.FDRTable)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button bpWrite;
        private System.Windows.Forms.Label lbBBMDlInfo;
        private System.Windows.Forms.TabControl tabCtrl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.DataGridView BBMDTable;
        private System.Windows.Forms.DataGridViewTextBoxColumn IPAddress;
        private System.Windows.Forms.DataGridViewTextBoxColumn Port;
        private System.Windows.Forms.DataGridViewTextBoxColumn Mask;
        private System.Windows.Forms.DataGridView FDRTable;
        private System.Windows.Forms.Button bpRefresh;
        private System.Windows.Forms.Label lbFDRInfo;
        private System.Windows.Forms.DataGridViewTextBoxColumn IPendpoint;
        private System.Windows.Forms.DataGridViewTextBoxColumn TTL;
        private System.Windows.Forms.DataGridViewTextBoxColumn RemainTTL;
    }
}