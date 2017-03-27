using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Excel = Microsoft.Office.Interop.Excel;

namespace Example6_3
{
    class ExcellDataPull
    {

      public  double[,] h_list_Value = new double[300, 300];

        public void ExcellData()
        {
            //--Now lets do some reading activity

            //  double[,] h_list_Value = new double[300, 300];

            
            //--Now lets do some reading activity
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(@"C:\Users\ASUS\Desktop\Matlab_H_data.xlsx");
            Microsoft.Office.Interop.Excel.Sheets sheets = xlWorkbook.Worksheets;
            Microsoft.Office.Interop.Excel.Worksheet worksheet = (Microsoft.Office.Interop.Excel.Worksheet)xlWorkbook.Worksheets.get_Item(1);
            // Excel._Worksheet xlWorksheet = (Excel.Worksheet)worksheet.get_Item(1);//xlWorkbook.Sheets[0];//"Sheet1"; //(Excel._Worksheet)xlWorkbook.Sheets[1];
            Excel.Range xlRange = worksheet.UsedRange;
            object[,] worksheetValuesArray = xlRange.get_Value(Type.Missing);

            for (int i = 0; i < 300; i++)
            {
                for (int j = 0; j < 300; j++)
                {

                    h_list_Value[i, j] = (double)worksheetValuesArray[i + 1, j + 1]; //worksheet.Cells[i + 1, j + 1].Value; //xlRange.Cells[i+1,j+1].Value;

                }
            }
            //MessageBox.Show("Firstdata = " + h_list_Value[0, 0] + ",count Row= " + h_list_Value.GetLength(0) + ",cell count" + xlRange.Count + ",val" + xlRange.Cells[1, 1].Value + ",another value = " + xlRange.Cells[300, 300].Value);
            xlWorkbook.Close(0);
            xlApp.Quit();

        }//Close of the function


    }
}
