namespace TemcoStandardBacnetTool
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
            ((System.ComponentModel.ISupportInitialize)(this.ObjectId)).BeginInit();
            this.SuspendLayout();
            // 
            // ObjectType
            // 
            this.ObjectType.FormattingEnabled = true;
            this.ObjectType.Location = new System.Drawing.Point(28, 24);
            this.ObjectType.Name = "ObjectType";
            this.ObjectType.Size = new System.Drawing.Size(217, 21);
            this.ObjectType.TabIndex = 0;
            // 
            // Create
            // 
            this.Create.Location = new System.Drawing.Point(28, 68);
            this.Create.Name = "Create";
            this.Create.Size = new System.Drawing.Size(75, 23);
            this.Create.TabIndex = 1;
            this.Create.Text = "Create";
            this.Create.UseVisualStyleBackColor = true;
            this.Create.Click += new System.EventHandler(this.Create_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(240, 68);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 2;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // ObjectId
            // 
            this.ObjectId.Location = new System.Drawing.Point(251, 25);
            this.ObjectId.Maximum = new decimal(new int[] {
            4194303,
            0,
            0,
            0});
            this.ObjectId.Name = "ObjectId";
            this.ObjectId.Size = new System.Drawing.Size(64, 20);
            this.ObjectId.TabIndex = 3;
            // 
            // CreateObject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 116);
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

        }

        #endregion

        private System.Windows.Forms.Button Create;
        private System.Windows.Forms.Button Cancel;
        public System.Windows.Forms.ComboBox ObjectType;
        public System.Windows.Forms.NumericUpDown ObjectId;
    }
}