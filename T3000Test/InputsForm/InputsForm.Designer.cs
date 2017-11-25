namespace T3000.Forms
{
    partial class InputsForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(InputsForm));
            this.saveButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.clearSelectedRowButton = new System.Windows.Forms.Button();
            this.view = new T3000.Controls.TView();
            this.InputColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PanelColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.DescriptionColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.AutoManualColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.ValueColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.UnitColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.RangeColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.RangeTextColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CalibrationColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.SignColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.FilterColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.StatusColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.JumperColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.LabelColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.view)).BeginInit();
            this.SuspendLayout();
            // 
            // saveButton
            // 
            resources.ApplyResources(this.saveButton, "saveButton");
            this.saveButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.saveButton.Name = "saveButton";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.Save);
            // 
            // cancelButton
            // 
            resources.ApplyResources(this.cancelButton, "cancelButton");
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.Cancel);
            // 
            // clearSelectedRowButton
            // 
            resources.ApplyResources(this.clearSelectedRowButton, "clearSelectedRowButton");
            this.clearSelectedRowButton.Name = "clearSelectedRowButton";
            this.clearSelectedRowButton.UseVisualStyleBackColor = true;
            this.clearSelectedRowButton.Click += new System.EventHandler(this.ClearSelectedRow);
            // 
            // view
            // 
            this.view.AllowUserToAddRows = false;
            this.view.AllowUserToDeleteRows = false;
            resources.ApplyResources(this.view, "view");
            this.view.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.view.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.view.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.InputColumn,
            this.PanelColumn,
            this.DescriptionColumn,
            this.AutoManualColumn,
            this.ValueColumn,
            this.UnitColumn,
            this.RangeColumn,
            this.RangeTextColumn,
            this.CalibrationColumn,
            this.SignColumn,
            this.FilterColumn,
            this.StatusColumn,
            this.JumperColumn,
            this.LabelColumn});
            this.view.MultiSelect = false;
            this.view.Name = "view";
            // 
            // InputColumn
            // 
            this.InputColumn.FillWeight = 55F;
            resources.ApplyResources(this.InputColumn, "InputColumn");
            this.InputColumn.Name = "InputColumn";
            this.InputColumn.ReadOnly = true;
            // 
            // PanelColumn
            // 
            resources.ApplyResources(this.PanelColumn, "PanelColumn");
            this.PanelColumn.Name = "PanelColumn";
            // 
            // DescriptionColumn
            // 
            this.DescriptionColumn.FillWeight = 150F;
            resources.ApplyResources(this.DescriptionColumn, "DescriptionColumn");
            this.DescriptionColumn.Name = "DescriptionColumn";
            // 
            // AutoManualColumn
            // 
            resources.ApplyResources(this.AutoManualColumn, "AutoManualColumn");
            this.AutoManualColumn.Name = "AutoManualColumn";
            this.AutoManualColumn.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // ValueColumn
            // 
            resources.ApplyResources(this.ValueColumn, "ValueColumn");
            this.ValueColumn.Name = "ValueColumn";
            // 
            // UnitColumn
            // 
            resources.ApplyResources(this.UnitColumn, "UnitColumn");
            this.UnitColumn.Name = "UnitColumn";
            this.UnitColumn.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // RangeColumn
            // 
            resources.ApplyResources(this.RangeColumn, "RangeColumn");
            this.RangeColumn.Name = "RangeColumn";
            // 
            // RangeTextColumn
            // 
            resources.ApplyResources(this.RangeTextColumn, "RangeTextColumn");
            this.RangeTextColumn.Name = "RangeTextColumn";
            this.RangeTextColumn.ReadOnly = true;
            // 
            // CalibrationColumn
            // 
            resources.ApplyResources(this.CalibrationColumn, "CalibrationColumn");
            this.CalibrationColumn.Name = "CalibrationColumn";
            // 
            // SignColumn
            // 
            this.SignColumn.FillWeight = 60F;
            resources.ApplyResources(this.SignColumn, "SignColumn");
            this.SignColumn.Name = "SignColumn";
            // 
            // FilterColumn
            // 
            resources.ApplyResources(this.FilterColumn, "FilterColumn");
            this.FilterColumn.Name = "FilterColumn";
            // 
            // StatusColumn
            // 
            resources.ApplyResources(this.StatusColumn, "StatusColumn");
            this.StatusColumn.Name = "StatusColumn";
            this.StatusColumn.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.StatusColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // JumperColumn
            // 
            this.JumperColumn.FillWeight = 150F;
            resources.ApplyResources(this.JumperColumn, "JumperColumn");
            this.JumperColumn.Name = "JumperColumn";
            // 
            // LabelColumn
            // 
            resources.ApplyResources(this.LabelColumn, "LabelColumn");
            this.LabelColumn.Name = "LabelColumn";
            // 
            // InputsForm
            // 
            this.AcceptButton = this.saveButton;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.Controls.Add(this.clearSelectedRowButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.view);
            this.Name = "InputsForm";
            ((System.ComponentModel.ISupportInitialize)(this.view)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private T3000.Controls.TView view;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button clearSelectedRowButton;
        private System.Windows.Forms.DataGridViewTextBoxColumn InputColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn PanelColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn DescriptionColumn;
        private System.Windows.Forms.DataGridViewButtonColumn AutoManualColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn ValueColumn;
        private System.Windows.Forms.DataGridViewButtonColumn UnitColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn RangeColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn RangeTextColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn CalibrationColumn;
        private System.Windows.Forms.DataGridViewButtonColumn SignColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn FilterColumn;
        private System.Windows.Forms.DataGridViewButtonColumn StatusColumn;
        private System.Windows.Forms.DataGridViewButtonColumn JumperColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn LabelColumn;
    }
}

