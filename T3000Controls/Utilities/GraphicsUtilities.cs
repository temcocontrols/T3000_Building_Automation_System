namespace T3000Controls
{
    using System;
    using System.Drawing;
    using System.Drawing.Drawing2D;

    internal static class GraphicsUtilities
    {
        /// <summary>
        /// Returns rounded rectangle path for selected rect and radius.
        /// </summary>
        /// <param name="rect">Input rect</param>
        /// <param name="radius">Rounding radius</param>
        /// <returns></returns>
        public static GraphicsPath CreateRoundedRectanglePath(RectangleF rect, int radius)
        {   
            var path = new GraphicsPath();
            path.AddArc(rect.X, rect.Y, radius, radius, 180, 90);
            path.AddArc(rect.X + rect.Width - radius - 1, rect.Y, radius, radius, 270, 90);
            path.AddArc(rect.X + rect.Width - radius - 1, rect.Y + rect.Height - radius - 1, radius, radius, 0, 90);
            path.AddArc(rect.X, rect.Y + rect.Height - radius - 1, radius, radius, 90, 90);
            path.CloseAllFigures();

            return path;
        }

        /// <summary>
        /// Returns region with pen offset for borders.
        /// </summary>
        /// <param name="path">Input path</param>
        /// <param name="pen">Input pen</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <returns></returns>
        public static Region GetRegionForPath(GraphicsPath path, Pen pen)
        {
            if (path == null) 
                throw new ArgumentNullException(nameof(path));

            if (pen == null)
                throw new ArgumentNullException(nameof(pen));

            var region = new Region(path);
            path.Widen(pen);
            region.Union(path);

            return region;
        }

        /// <summary>
        /// Returns region with SystemPens.ActiveBorder offset for borders.
        /// </summary>
        /// <param name="path">Input path</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <returns></returns>
        public static Region GetRegionForPath(GraphicsPath path)
        {
            if (path == null)
                throw new ArgumentNullException(nameof(path));

            return GetRegionForPath(path, SystemPens.ActiveBorder);
        }
    }
}