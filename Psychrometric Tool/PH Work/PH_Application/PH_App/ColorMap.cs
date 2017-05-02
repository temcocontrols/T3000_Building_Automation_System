using System;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace PH_App
{
    public class ColorMap
    {
        private int colormapLength = 64;
        private int alphaValue = 255;
        
        public ColorMap()
        {
        }

        public ColorMap(int colorLength)
        {
            colormapLength = colorLength;
        }

        public ColorMap(int colorLength, int alpha)
        {
            colormapLength = colorLength;
            alphaValue = alpha;
        }

        public int[,] Spring()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[] spring = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                spring[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = 255;
                cmap[i, 2] = (int)(255 * spring[i]);
                cmap[i, 3] = 255 - cmap[i, 1];
            }
            return cmap;
        }

        public int[,] Summer()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[] summer = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                summer[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = (int)(255 * summer[i]);
                cmap[i, 2] = (int)(255 * 0.5f * (1 + summer[i]));
                cmap[i, 3] = (int)(255 * 0.4f);
            }
            return cmap;
        }

        public int[,] Autumn()
        {   
            int[,] cmap = new int[colormapLength, 4];
            float[] autumn = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                autumn[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = 255;
                cmap[i, 2] = (int)(255 * autumn[i]);
                cmap[i, 3] = 0;
            }
            return cmap;
        }

        public int[,] Winter()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[] winter = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                winter[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = 0;
                cmap[i, 2] = (int)(255 * winter[i]);
                cmap[i, 3] = (int)(255 * (1.0f - 0.5f * winter[i]));
            }
            return cmap;
        }

        public int[,] Gray()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[] gray = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                gray[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = (int)(255 * gray[i]);
                cmap[i, 2] = (int)(255 * gray[i]);
                cmap[i, 3] = (int)(255 * gray[i]);
            }
            return cmap;
        }

        public int[,] Jet()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[,] cMatrix = new float[colormapLength, 3];
            int n = (int)Math.Ceiling(colormapLength / 4.0f);
            int nMod = 0;
            float[] fArray = new float[3 * n - 1];
            int[] red = new int[fArray.Length];
            int[] green = new int[fArray.Length];
            int[] blue = new int[fArray.Length];

            if (colormapLength % 4 == 1)
            {
                nMod = 1;
            }

            for (int i = 0; i <fArray.Length; i++)
            {
                if (i < n)
                    fArray[i] = (float)(i + 1) / n;
                else if (i >= n && i < 2 * n - 1)
                    fArray[i] = 1.0f;
                else if (i >= 2 * n - 1)
                    fArray[i] = (float)(3 * n - 1 - i) / n;
                green[i] = (int)Math.Ceiling(n / 2.0f) - nMod + i;
                red[i] = green[i] + n;
                blue[i] = green[i] - n;
            }

            int nb = 0;
            for (int i = 0; i < blue.Length; i++)
            {
                if (blue[i] > 0)
                    nb++;
            }

            for (int i = 0; i < colormapLength; i++)
            {
                for (int j = 0; j < red.Length; j++)
                {
                    if (i == red[j] && red[j] < colormapLength)
                    {
                        cMatrix[i, 0] = fArray[i - red[0]];
                    }
                }
                for (int j = 0; j < green.Length; j++)
                {
                    if (i == green[j] && green[j] < colormapLength)
                        cMatrix[i, 1] = fArray[i - (int)green[0]];
                }
                for (int j = 0; j < blue.Length; j++)
                {
                    if (i == blue[j] && blue[j] >= 0)
                        cMatrix[i, 2] = fArray[fArray.Length - 1 - nb + i];
                }
            }

            for (int i = 0; i < colormapLength; i++)
            {
                cmap[i, 0] = alphaValue;
                for (int j = 0; j < 3; j++)
                {
                    cmap[i, j+1] = (int)(cMatrix[i, j] * 255);
                }
            }
            return cmap;
        }

        public int[,] Hot()
        {
            int[,] cmap = new int[colormapLength, 4];
            int n = 3 * colormapLength / 8;
            float[] red = new float[colormapLength];
            float[] green = new float[colormapLength];
            float[] blue = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                if (i < n)
                    red[i] = 1.0f*(i+1) / n;
                else
                    red[i] = 1.0f;
                if (i < n)
                    green[i] = 0f;
                else if (i >= n && i < 2 * n)
                    green[i] = 1.0f * (i+1 - n) / n;
                else
                    green[i] = 1f;
                if (i < 2 * n)
                    blue[i] = 0f;
                else
                    blue[i] = 1.0f * (i + 1 - 2 * n) / (colormapLength - 2 * n);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = (int)(255 * red[i]);
                cmap[i, 2] = (int)(255 * green[i]);
                cmap[i, 3] = (int)(255 * blue[i]);
            }
            return cmap;
        }

        public int[,] Cool()
        {
            int[,] cmap = new int[colormapLength, 4];
            float[] cool = new float[colormapLength];
            for (int i = 0; i < colormapLength; i++)
            {
                cool[i] = 1.0f * i / (colormapLength - 1);
                cmap[i, 0] = alphaValue;
                cmap[i, 1] = (int)(255 * cool[i]);
                cmap[i, 2] = (int)(255 * (1 - cool[i]));
                cmap[i, 3] = 255;
            }
            return cmap;
        }
    }
}
