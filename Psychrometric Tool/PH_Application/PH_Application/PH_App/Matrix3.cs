using System;
using System.Collections.Generic;
using System.Text;

namespace PH_App
{
    public class Matrix3
    {
        public float[,] M = new float[4, 4];

        public Matrix3()
        {
            Identity3();
        }

        public Matrix3(float m00, float m01, float m02, float m03,
                       float m10, float m11, float m12, float m13,
                       float m20, float m21, float m22, float m23,
                       float m30, float m31, float m32, float m33)
        {
            M[0, 0] = m00;
            M[0, 1] = m01;
            M[0, 2] = m02;
            M[0, 3] = m03;
            M[1, 0] = m10;
            M[1, 1] = m11;
            M[1, 2] = m12;
            M[1, 3] = m13;
            M[2, 0] = m20;
            M[2, 1] = m21;
            M[2, 2] = m22;
            M[2, 3] = m23;
            M[3, 0] = m30;
            M[3, 1] = m31;
            M[3, 2] = m32;
            M[3, 3] = m33;
        }

        // Define a Identity matrix:
        public void Identity3()
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (i == j)
                    {
                        M[i, j] = 1;
                    }
                    else
                    {
                        M[i, j] = 0;
                    }
                }
            }
        }

        // Multiply two matrices together:
        public static Matrix3 operator *(Matrix3 m1, Matrix3 m2)
        {
            Matrix3 result = new Matrix3();
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    float element = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        element += m1.M[i, k] * m2.M[k, j];
                    }
                    result.M[i, j] = element;
                }
            }
            return result;
        }
        // Apply a transformation to a vector (point):
        public float[] VectorMultiply(float[] vector)
        {
            float[] result = new float[4];
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    result[i] += M[i, j] * vector[j];
                }
            }
            return result;
        }      

        // Create a scaling matrix:
        public static Matrix3 Scale3(float sx, float sy, float sz)
        {
            Matrix3 result = new Matrix3();
            result.M[0, 0] = sx;
            result.M[1, 1] = sy;
            result.M[2, 2] = sz;
            return result;
        }

        // Create a translation matrix
        public static Matrix3 Translate3(float dx, float dy, float dz)
        {
            Matrix3 result = new Matrix3();
            result.M[0, 3] = dx;
            result.M[1, 3] = dy;
            result.M[2, 3] = dz;
            return result;
        }

        // Create a rotation matrix around the x axis:
        public static Matrix3 Rotate3X(float theta)
        {
            theta = theta * (float)Math.PI / 180.0f;
            float sn = (float)Math.Sin(theta);
            float cn = (float)Math.Cos(theta);
            Matrix3 result = new Matrix3();
            result.M[1, 1] = cn;
            result.M[1, 2] = -sn;
            result.M[2, 1] = sn;
            result.M[2, 2] = cn;
            return result;
        }

        // Create a rotation matrix around the y axis:
        public static Matrix3 Rotate3Y(float theta)
        {
            theta = theta * (float)Math.PI / 180.0f;
            float sn = (float)Math.Sin(theta);
            float cn = (float)Math.Cos(theta);
            Matrix3 result = new Matrix3();
            result.M[0, 0] = cn;
            result.M[0, 2] = sn;
            result.M[2, 0] = -sn;
            result.M[2, 2] = cn;
            return result;
        }

        // Create a rotation matrix around the z axis:
        public static Matrix3 Rotate3Z(float theta)
        {
            theta = theta * (float)Math.PI / 180.0f;
            float sn = (float)Math.Sin(theta);
            float cn = (float)Math.Cos(theta);
            Matrix3 result = new Matrix3();
            result.M[0, 0] = cn;
            result.M[0, 1] = -sn;
            result.M[1, 0] = sn;
            result.M[1, 1] = cn;
            return result;
        }

        // Front view projection matrix:
        public static Matrix3 FrontView()
        {
            Matrix3 result = new Matrix3();
            result.M[2, 2] = 0;
            return result;
        }

        // Side view projection matrix:
        public static Matrix3 SideView()
        {
            Matrix3 result = new Matrix3();
            result.M[0, 0] = 0;
            result.M[2, 2] = 0;
            result.M[0, 2] = -1;
            return result;
        }

        // Top view projection matrix:
        public static Matrix3 TopView()
        {
            Matrix3 result = new Matrix3();
            result.M[1, 1] = 0;
            result.M[2, 2] = 0;
            result.M[1, 2] = -1;
            return result;
        }

        // Axonometric projection matrix:
        public static Matrix3 Axonometric(float alpha, float beta)
        {
            Matrix3 result = new Matrix3();
            float sna = (float)Math.Sin(alpha * Math.PI / 180);
            float cna = (float)Math.Cos(alpha * Math.PI / 180);
            float snb = (float)Math.Sin(beta * Math.PI / 180);
            float cnb = (float)Math.Cos(beta * Math.PI / 180);
            result.M[0, 0] = cnb;
            result.M[0, 2] = snb;
            result.M[1, 0] = sna * snb;
            result.M[1, 1] = cna;
            result.M[1, 2] = -sna * cnb;
            result.M[2, 2] = 0;
            return result;
        }

        // Oblique projection matrix:
        public static Matrix3 Oblique(float alpha, float theta)
        {
            Matrix3 result = new Matrix3();
            float ta = (float)Math.Tan(alpha * Math.PI / 180);
            float snt = (float)Math.Sin(theta * Math.PI / 180);
            float cnt = (float)Math.Cos(theta * Math.PI / 180);
            result.M[0, 2] = -cnt / ta;
            result.M[1, 2] = -snt / ta;
            result.M[2, 2] = 0;
            return result;
        }

        // Perspective projection matrix:
        public static Matrix3 Perspective(float d)
        {
            Matrix3 result = new Matrix3();
            result.M[3, 2] = -1 / d;
            return result;
        }

        public Point3 Cylindrical(float r, float theta, float y)
        {
            Point3 pt = new Point3();
            float sn = (float)Math.Sin(theta * Math.PI / 180);
            float cn = (float)Math.Cos(theta * Math.PI / 180);
            pt.X = r * cn;
            pt.Y = y;
            pt.Z = -r * sn;
            pt.W = 1;
            return pt;
        }

        public Point3 Spherical(float r, float theta, float phi)
        {
            Point3 pt = new Point3();
            float snt = (float)Math.Sin(theta * Math.PI / 180);
            float cnt = (float)Math.Cos(theta * Math.PI / 180);
            float snp = (float)Math.Sin(phi * Math.PI / 180);
            float cnp = (float)Math.Cos(phi * Math.PI / 180);
            pt.X = r * snt * cnp;
            pt.Y = r * cnt;
            pt.Z = -r * snt * snp;
            pt.W = 1;
            return pt;
        }

        public static Matrix3 Euler(float alpha, float beta, float gamma)
        {
            Matrix3 result = new Matrix3();
            alpha = alpha * (float)Math.PI / 180.0f;
            float sna = (float)Math.Sin(alpha);
            float cna = (float)Math.Cos(alpha);
            beta = beta * (float)Math.PI / 180.0f;
            float snb = (float)Math.Sin(beta);
            float cnb = (float)Math.Cos(beta);
            gamma = gamma * (float)Math.PI / 180.0f;
            float sng = (float)Math.Sin(gamma);
            float cng = (float)Math.Cos(gamma);
            result.M[0, 0] = cna * cng - sna * snb * sng;
            result.M[0, 1] = -snb * sng;
            result.M[0, 2] = sna * cng - cna * cnb * sng;
            result.M[1, 0] = -sna * snb;
            result.M[1, 1] = cnb;
            result.M[1, 2] = cna * snb;
            result.M[2, 0] = -cna * sng - sna * cnb * cng;
            result.M[2, 1] = -snb * cng;
            result.M[2, 2] = cna * cnb * cng - sna * snb;
            return result;
        }

        public static Matrix3 AzimuthElevation(float elevation, float azimuth)
        {
            Matrix3 result = new Matrix3();
            // make sure elevation in the range of [-90, 90]:
            if (elevation > 90)
                elevation = 90;
            else if (elevation < -90)
                elevation = -90;
            // Make sure azimuth in the range of [-180, 180]:
            if (azimuth > 180)
                azimuth = 180;
            else if (azimuth < -180)
                azimuth = -180;

            elevation = elevation * (float)Math.PI / 180.0f;
            float sne = (float)Math.Sin(elevation);
            float cne = (float)Math.Cos(elevation);
            azimuth = azimuth * (float)Math.PI / 180.0f;
            float sna = (float)Math.Sin(azimuth);
            float cna = (float)Math.Cos(azimuth);
            result.M[0, 0] = cna;
            result.M[0, 1] = sna;
            result.M[0, 2] = 0;
            result.M[1, 0] = -sne * sna;
            result.M[1, 1] = sne * cna;
            result.M[1, 2] = cne;
            result.M[2, 0] = cne * sna;
            result.M[2, 1] = -cne * cna;
            result.M[2, 2] = sne;
            return result;
        }
    }
}
