namespace Yabe
{
    partial class CreateObject
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CreateObject));
            this.ObjectType = new System.Windows.Forms.ComboBox();
            this.Create = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.ObjectId = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.ObjectName = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.ObjectId)).BeginInit();
            this.SuspendLayout();
            // 
            // ObjectType
            // 
            this.ObjectType.FormattingEnabled = true;
            this.ObjectType.Location = new System.Drawing.Point(18, 24);
            this.ObjectType.Name = "ObjectType";
            this.ObjectType.Size = new System.Drawing.Size(227, 21);
            this.ObjectType.TabIndex = 0;
            // 
            // Create
            // 
            this.Create.Location = new System.Drawing.Point(16, 103);
            this.Create.Name = "Create";
            this.Create.Size = new System.Drawing.Size(75, 23);
            this.Create.TabIndex = 1;
            this.Create.Text = "Create";
            this.Create.UseVisualStyleBackColor = true;
            this.Create.Click += new System.EventHandler(this.Create_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(245, 103);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 2;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // ObjectId
            // 
            this.ObjectId.Location = new System.Drawing.Point(256, 25);
            this.ObjectId.Maximum = new decimal(new int[] {
            4194303,
            0,
            0,
            0});
            this.ObjectId.Name = "ObjectId";
            this.ObjectId.Size = new System.Drawing.Size(64, 20);
            this.ObjectId.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 67);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Object_Name :";
            // 
            // ObjectName
            // 
            this.ObjectName.Location = new System.Drawing.Point(102, 64);
            this.ObjectName.Name = "ObjectName";
            this.ObjectName.Size = new System.Drawing.Size(218, 20);
            this.ObjectName.TabIndex = 5;
            // 
            // CreateObject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 157);
            this.Controls.Add(this.ObjectName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ObjectId);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.Create);
            this.Controls.Add(this.ObjectType);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CreateObject";
            this.Text = "CreateObject";
            this.Load += new System.EventHandler(this.CreateObject_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ObjectId)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Create;
        private System.Windows.Forms.Button Cancel;
        public System.Windows.Forms.ComboBox ObjectType;
        public System.Windows.Forms.NumericUpDown ObjectId;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.TextBox ObjectName;
    }
}