using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WFA_PH_CurveDemo
{
    public static class MeshGridClass
    {
        //--Now for the meshgrid() section...
        //returning tuple because it helps to return multiple values.


        public static Tuple<T[,], T[,]> MeshGrid<T>(this T[] x, T[] y)
        {
            var X = new T[x.Length, y.Length];
            var Y = new T[x.Length, y.Length];
            for (int i = 0; i < x.Length; i++)
            {
                for (int j = 0; j < y.Length; j++)
                {
                    X[i, j] = x[i];
                    Y[i, j] = y[j];
                }
            }

            return Tuple.Create(X, Y);
        }

    }
}
