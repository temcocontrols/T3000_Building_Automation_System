using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PH_App
{
    public partial class TrashBox : Form
    {
        Form_Main_PH_Application PH;
        public TrashBox(Form_Main_PH_Application F)
        {
            PH = F;
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //--Cancel btn
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //--Delete btn
            try
            {
                if (MessageBox.Show("Are you sure you want to delete this chart Permanently?", "Delete chart Permanently", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)

                {

                    if (dataGridView1.Rows.Count <= 0)
                    {
                        //--If there is no data don't need to do futher processing
                        return;
                    }

                    //--Delete function 
                    //--This is the delete operation for the handler.....
                    /*
                    Steps: 1. delete the row  of the table using the id portion.
                    2. Delete the corresponding tables related to the row.             
                    */

                    //=========================================================This code is commented for not deleting the values===============//

                    // MessageBox.Show("Here we are 293ln");
                    ArrayList arrList = new ArrayList();
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        //--In rows we are looping .
                        if (Convert.ToBoolean(dataGridView1.Rows[i].Cells[3].Value)) //Will be true if checkedd  
                        {
                            arrList.Add(i);//Lets add the index value

                        }
                    }


                    for (int i = 0; i < arrList.Count; i++)
                    {
                        //Now lets delete all the chart values
                        int indexVal = int.Parse(arrList[i].ToString());
                        var mc = new MainController();
                        mc.CheckSelectedBuilding();
                        string buildingName = mc.selectedBuildingList[0].BuildingName;
                        mc.deleteChartAndItsContent(dataGridView1.Rows[indexVal].Cells[1].Value.ToString(), dataGridView1.Rows[indexVal].Cells[4].Value.ToString(), dataGridView1.Rows[indexVal].Cells[5].Value.ToString(), buildingName);
                    }

                    // MessageBox.Show("Here we are  end 310ln");
                    this.Close();
                }//close of the chart delete

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        //List to store the id and name of the chart
        ArrayList arrayList = new ArrayList();
        private void button1_Click(object sender, EventArgs e)
        {
            //--btn Restore

            try
            {
                //--After applying ok we will restore all the data that has been 
                //--triggered as false as to be true and load the value again 
                //--Also close the window as soon as  it is set
                /*
               Steps : 
               1.Check for all the check boxes and restore
               */
                if (dataGridView1.Rows.Count <= 0)
                {
                    //--If there is no data don't need to do futher processing
                    return;
                }



                arrayList.Clear();//Empty the arraylist 
                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    //--In rows we are looping .
                    if (Convert.ToBoolean(dataGridView1.Rows[i].Cells[3].Value)) //Will be true if checkedd  
                    {
                        arrayList.Add(dataGridView1.Rows[i].Cells[1].Value.ToString());//Insert the chart id values
                                                                                       //storeRestoreList.Add(chartDetailListForDissabledValue[i].chartID);
                                                                                       // MessageBox.Show("Value = " + dataGridView1.Rows[i].Cells[1].Value.ToString());
                    }
                }

                //MessageBox.Show("StoreRestoreList count =" + arrayList.Count);
                //--Now lets remove the checked values form the database sections
                var mc = new MainController();
                mc.CheckSelectedBuilding();
                string buildingName = mc.selectedBuildingList[0].BuildingName;
                for (int i = 0; i < arrayList.Count; i++)
                {
                    mc.updateValueInDatabaseToTrue(buildingName, arrayList[i].ToString());
                }

                //--Now after updating call the datagridview_show_Data function in main chart and close the 

                //--We are claling the dataGridView_Show_Data is called
                //  MessageBox.Show("DataGridView_show_Data Calls");


                //f1.RefreshingEverythingChartAndContent(sender);//Calling the function for refresh
               // var ph = new Form_Main_PH_Application();
                PH.RefreshChartListForTrashBoxRestore(PH);

                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void TrashBox_Load(object sender, EventArgs e)
        {
            var v =new MainController();
            v.LoadDataFromDB(this);
        }
    }
}
