using System;
using System.Drawing;

namespace PH_App
{
    public class BarStyle
    {
        private float xLength = 0.5f;
        private float yLength = 0.5f;
        private float zOrigin = 0;
        private bool isBarSingleColor = false;

        public BarStyle()
        {

        }

        public bool IsBarSingleColor
        {
            get { return isBarSingleColor; }
            set { isBarSingleColor = value; }
        }

        public float ZOrigin
        {
            get { return zOrigin; }
            set { zOrigin = value; }
        }

        public float YLength
        {
            get { return yLength; }
            set { yLength = value; }
        }

        public float XLength
        {
            get { return xLength; }
            set { xLength = value; }
        }
    }
}
