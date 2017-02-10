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

namespace WFA_PH_CurveDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {





        }

       
        private void PlotPHChart(string fluidName)
        {

            /*
             Lets do it step by step as the way that has been done in the Matlab

             */
            //--1. press_rng = logspace(-2,2,300); % [MPa] pressure (p) range

            PH_ChartClass PH = new PH_ChartClass();
            double[] press_rng = PH.LogSpace(-2, 2, 300,true,10).ToArray();
            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s = null;       
            for (int i = 0; i < press_rng.Length; i++)
            {
                s += press_rng[i].ToString() + "\t \t";
            }
            //MessageBox.Show("press_rng values = \n" + s);


            //--For line spacing values
            //2. temp_rng = 273.15+linspace(1,800,300); 
            double[] temp_rng =  PH.LinSpace(1, 800, 300, true).ToArray();
            

            //ArrayList press_rng = PH.logSpaceMyFunction(-2, 2, 300);//I think this will print thearray
            //ArrayList press_rng = PH.logSpace(1, 5, 7);
            //--Check
            string s1 = null;
            for (int i = 0; i < temp_rng.Length; i++)
            {
                temp_rng[i] += 273.15;
                s1 += temp_rng[i].ToString() + "\t \t";
            }
            // MessageBox.Show("temp_rng values = \n" + s1);



            //--Now for meshgrid we have created truple

            //MeshGrid_Class ms1 = new MeshGrid_Class();
            //3.[p,T] = meshgrid(press_rng,temp_rng); % [MPa,K] mesh p & T

            MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            Tuple<double[,], double[,]> tuple_x = MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            double[,] p = tuple_x.Item1;
            double[,] T = tuple_x.Item2;

            // MessageBox.Show("Tuple =\n" + tuple_x.Item1.Length);
            // MessageBox.Show("Tuple =\n" + p[1,1]);
            //  MeshGrid_Class.MeshGrid(press_rng, temp_rng);

            // MessageBox.Show("Check row = " + p.GetLength(0) + ",column = " + p.GetLength(1)+"pvalue check= "+p[0,0]+", last = "+p[299,299]);

            //string pTest1 = null;
            //string hum = null;
            //for (int i = 290; i < 300; i++)//Row length 
            //{
            //    // pTest += item + "\t";
            //    for (int j = 290; j <300; j++)//column length 
            //    {
            //        pTest1 ="V="+ p[i, j].ToString()+ "\t\t";
            //        hum += pTest1;
            //    }
            //}

            //MessageBox.Show("Pressure value check = \n" + hum);


            //--Now lets do the calculation for others 
            //4.h = IAPWS_IF97('h_pT',p,T); % [kJ/kg] enthalpy = f(p,T)

            double[,] h = new double[p.GetLength(0), p.GetLength(1)];

            for(int i = 0; i < h.GetLength(0); i++)
            {
                for(int j = 0; j < h.GetLength(1); j++)
                {
                    if (T[i, j] <= 22.064)
                    { 
                    //--Now calling is done for the enthalpy function
                    double enthalpy = PH.IAPWS_IF97_TowParameterEquivalentFxn("H", "P", p[i, j], "T", T[i, j], fluidName);
                    h[i, j] = enthalpy;
                    }

                }
            }



            //--Use dynamic list to trim rather than array

            //double[,] h_trimed = new double[0,0];

            //for (int i = 0; i < h.GetLength(0); i++)
            //{
            //    for (int j = 0; j < h.GetLength(1); j++)
            //    {
            //         if(Convert.ToString(h[i, j])  != "")
            //        {
            //            h_trimed[i, j] = h[i, j];
            //        }
                    
            //    }
            //}



           // MessageBox.Show("Enthalpy row  = " + h.GetLength(0) + ",col = " + h.GetLength(1) +"Trimmed value row= "+h_trimed.GetLength(0)+",col = "+h_trimed.GetLength(1));

            MessageBox.Show("end \n");

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Lets call the actual function
            string fluidNAME = "Water";
            PlotPHChart(fluidNAME);
        }
    }
}
