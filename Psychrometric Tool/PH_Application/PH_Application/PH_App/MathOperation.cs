using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;

namespace PH_App
{
    class MathOperation
    {

        //==Relevant functions in this section of the moment
        //These are the parameters which will be used later in plotting
        ArrayList press_rng = new ArrayList();
        ArrayList temp_rng = new ArrayList();

        /// <summary>
        /// This function has same property as logspace(start,end,count) in matlab
        /// </summary>
        /// <param name="start">start value</param>
        /// <param name="end">end value</param>
        /// <param name="count">count value</param>
        /// <returns></returns>
        public IEnumerable<double> logspace(double start, double end, int count)
        {
            double d = (double)count, p = end / start;

            return Enumerable.Range(0, count).Select(i => start * Math.Pow(p, i / d));
        }

        //==Lograthmically spacing of numbers
        public ArrayList logarithmicSpace(double start, double end, int count)
        {

            ArrayList ArrList = new ArrayList();

            for (int i = 0; i <= count; i++)
            {
                double f = start * Math.Pow(((float)end) / start, ((float)i) / count);

                ArrList.Add(f);
            }

            return ArrList;
        }

        //Another logrithmic fxn
        //Create logarithmically spaced vector
        public ArrayList logSpace(int start, int end, int count)
        {
            double delta = (end - start) / (double)count;

            ArrayList ArrList = new ArrayList();

            for (double i = start; i < end; i = i + Math.Log(i))
            {
                ArrList.Add(i);
            }

            return ArrList;
        }

        //==Lograthmically spacing of numbers
        public ArrayList logSpaceMyFunction(double start, double end, int count)
        {

            ArrayList ArrList = new ArrayList();

            for (int i = 0; i <= count; i++)
            {
                double diff = ((Math.Pow(10, end) - Math.Pow(10, start)) / count);
                double f = start + i * diff;

                ArrList.Add(f);
            }

            return ArrList;
        }

        /// <summary>
        /// function similar to matlab function of linespace
        /// </summary>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <param name="count"></param>
        /// <returns></returns>
        public IEnumerable<double> linespace(double start, double end, int count)
        {
            double n = (double)count, diff = (end - start) / (n - 1);
            return Enumerable.Range(0, count).Select(i => start + ((i) * diff));
        }

        //--This is used and it works
        //------------------------------------Log spacing in c# --------------------------------------------//

        public static IEnumerable<double> Arange(double start, int count)
        {
            return Enumerable.Range((int)start, count).Select(v => (double)v);
        }

        public static IEnumerable<double> Power(IEnumerable<double> exponents, double baseValue = 10.0d)
        {
            return exponents.Select(v => Math.Pow(baseValue, v));
        }

        public IEnumerable<double> LinSpace(double start, double stop, int num, bool endpoint = true)
        {
            var result = new List<double>();
            if (num <= 0)
            {
                return result;
            }

            if (endpoint)
            {
                if (num == 1)
                {
                    return new List<double>() { start };
                }

                var step = (stop - start) / ((double)num - 1.0d);
                result = Arange(0, num).Select(v => (v * step) + start).ToList();
            }
            else
            {
                var step = (stop - start) / (double)num;
                result = Arange(0, num).Select(v => (v * step) + start).ToList();
            }

            return result;
        }


        public IEnumerable<double> LogSpace(double start, double stop, int num, bool endpoint = true, double numericBase = 10.0d)
        {
            var y = LinSpace(start, stop, num: num, endpoint: endpoint);
            return Power(y, numericBase);
        }

        //------------------------End of logspace---------------------------------------------------//

        /// <summary>
        /// Equivalent of matlab function IAPWS_IF97() 
        /// returns parameters based on the outputName 
        /// </summary>
        /// <param name="outputName">output we want</param>
        /// <param name="prop1">proerty1(like T(tempr), P(Pressure),h(enthalpy),etc)</param>
        /// <param name="prop1Value">Value of prop1</param>
        /// <param name="prop2">proerty2(like T(tempr), P(Pressure),h(enthalpy),etc)</param>
        /// <param name="prop2Value">Value of prop2</param>
        /// <param name="fluidName">Fluid Name(eg. Water,n-Propane,etc)</param>
        /// <returns>values based on pased parameter generally, temperature, pressure and enthalpy</returns>
        public double IAPWS_IF97_TowParameterEquivalentFxn(string outputParamName, string prop1, double prop1Value, string prop2, double prop2Value, string fluidName)
        {
        
           return CoolProp.PropsSI(outputParamName, prop1, prop1Value, prop2, prop2Value, fluidName);
        }



    }
}
