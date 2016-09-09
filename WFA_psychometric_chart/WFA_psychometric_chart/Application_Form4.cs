using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.Windows.Forms.DataVisualization.Charting;
namespace WFA_psychometric_chart
{
    public partial class Application_Form4 : Form
    {
        private Form1_main form1;
        public Application_Form4(Form1_main form1)
            //:this()
        {
            this.form1 = form1;//defined in top..
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.Application_Form4_Disposed );
        }

        //public void ShowDialog(ref Chart chart1)
        //{
        //    //Assign received parameter(s) to local context
        //    //txtBoxForm2.Text = txtBoxForm1.Text;

        //    this.ShowDialog(); //Display and activate this form (Form2)

        //    //Return parameter(s)
        //    //txtBoxForm1.Text = txtBoxForm2.Text;
            
        //}


        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        //lets create a class that is use for inserting in the dynamic list
        public class list_class
        {
            public double value1 { get; set; }
            public double value2
            {
                get;
                set;
            }

        }//close of class.my lsit

        private void button1_Click(object sender, EventArgs e)
        {
            //this part let the user plot the value on the graph..
            /*Lets do follwing steps form now.
             * check the value are empty or not if empty message error...
             * check for the values are correct or not...
             * push the data in array list and pass the value to the form1.
             */
            if (tb_ref_temp_ten.Text == "" || tb_ref_temp_twentyfive.Text =="" || tb_ref_temp_fourty.Text=="")
            {
                MessageBox.Show("Enter a valid refrence temperature value in degree celcius");
            }
            else
            {
                //do some operation.
                if (tb_ref_hum_twentyfive.Text == "" || tb_ref_hum_fifty.Text == "" || tb_ref_hum_seventyfive.Text == "")
                {
                    MessageBox.Show("Enter a valid refrence humidity value in %");
                }
                else
                {
                    //do something...
                    //check for input boxex.
                    if(tb_one.Text=="" || tb_two.Text=="" ||tb_three.Text==""||tb_four.Text==""||tb_five.Text==""||tb_six.Text==""||tb_seven.Text==""||tb_eight.Text==""||tb_nine.Text==""){
                    MessageBox.Show("Enter a valid temperature value in the input boxex in degree celcius");
                      } else{
                    //do something...
                          try
                          {
                              //refrence temp and humidity
                              double temp_ref1 = double.Parse(tb_ref_temp_ten.Text);
                              double temp_ref2 = double.Parse(tb_ref_temp_twentyfive.Text);
                              double temp_ref3 = double.Parse(tb_ref_temp_fourty.Text);
                              double hum_ref1 = double.Parse(tb_ref_hum_twentyfive.Text);
                              double hum_ref2 = double.Parse(tb_ref_hum_fifty.Text);
                              double hum_ref3 = double.Parse(tb_ref_hum_seventyfive.Text);
                              //the actual input temperature...
                              double temp_one = double.Parse(tb_one.Text);
                              double temp_two = double.Parse(tb_two.Text);
                              double temp_three = double.Parse(tb_three.Text);
                              double temp_four = double.Parse(tb_four.Text);
                              double temp_five = double.Parse(tb_five.Text);
                              double temp_six = double.Parse(tb_six.Text);
                              double temp_seven = double.Parse(tb_seven.Text);
                              double temp_eight = double.Parse(tb_eight.Text);
                              double temp_nine = double.Parse(tb_nine.Text);
                              //now lets throw this value in an array list and pass it to the other form using dynamic datatype....

                              //lets put this values in an list..
                              List<list_class> list_temp = new List<list_class>();
                              //lets add to this list..
                              list_temp.Add(new list_class{ value1= temp_ref1,value2= hum_ref1});
                              list_temp.Add(new list_class {value1= temp_ref2,value2= hum_ref2 });
                              list_temp.Add(new list_class {value1= temp_ref3,value2= hum_ref3 });

                              //lets add the nine values as well..
                              list_temp.Add(new list_class { value1= temp_ref1,value2= temp_one});
                              list_temp.Add(new list_class { value1 = temp_ref2, value2 = temp_two });
                              list_temp.Add(new list_class { value1 = temp_ref3, value2 = temp_three });

                              list_temp.Add(new list_class { value1 = temp_ref1, value2 = temp_four });
                              list_temp.Add(new list_class { value1 = temp_ref2, value2 = temp_five });
                              list_temp.Add(new list_class { value1 = temp_ref3, value2 = temp_six });


                              list_temp.Add(new list_class { value1 = temp_ref1, value2 = temp_seven });
                              list_temp.Add(new list_class { value1 = temp_ref2, value2 = temp_eight });
                              list_temp.Add(new list_class { value1 = temp_ref3, value2 = temp_nine });

                              form1.plot_new_graph();//this one is working..
                              //now lets call the appropriate function to plot this values..
                              for (int i = 0; i < list_temp.Count; i++)
                              {
                                 
                                  //form1.plot_by_DBT_HR((double)list_temp[i].value1, (double)list_temp[i].value2);
                                  form1.plot_by_DBT_HR((double)list_temp[i].value1, (double)(list_temp[i].value2/100));
                                  //MessageBox.Show("temp =" + (double)list_temp[i].value1 + " , hum= " + (double)list_temp[i].value2);
                              }


                          }
                          catch (Exception ex)
                          {
                              MessageBox.Show(ex.Message);
                          }



                    }
                }
             
            }

            MessageBox.Show("success");

        }

        private void Application_Form4_Load(object sender, EventArgs e)
        {
            //lets set the default value first..
            tb_ref_temp_ten.Text = "10".ToString();
            tb_ref_temp_twentyfive.Text = "25".ToString();
            tb_ref_temp_fourty.Text = "40".ToString();
            tb_ref_hum_twentyfive.Text = "25".ToString();
            tb_ref_hum_fifty.Text = "50".ToString();
            tb_ref_hum_seventyfive.Text = "75".ToString();
            label2.Text = "Humidity(in %)" + char.ConvertFromUtf32(8595);

        }

        //lets do passing Form1 instance in the constructor of application_form4

        //public Application_Form4(Form1 form1)
        //    :this()
        //{
        //    //complete the memeber initialization..
        //    this.form1 = form1;//defined in top..
        //    InitializeComponent();

        //} 




      public void Application_Form4_Disposed ( object sender, System.EventArgs e )
      {
      }

      
    }//close of application_Form4 class

    


}
