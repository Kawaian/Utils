using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Util
{
    public class Size
    {
        public static Size operator +(Size s, double d)
        {
            return new Size(s.Width + d, s.Height + d);
        }

        public double Width;
        public double Height;

        public Point Center => new Point(Width / 2, Height / 2);

        public Size(double all)
        {
            Width = Height = all;
        }

        /// <summary>
        /// Size
        /// </summary>
        /// <param name="width">rows</param>
        /// <param name="height">cols</param>
        public Size(double width, double height)
        {
            Width = width;
            Height = height;
        }

        public Size(Point leftop, Point rightbot)
        {
            Width = rightbot.X - leftop.X;
            Height = rightbot.Y - leftop.Y;
        }

#if UTILS_OPENCVSHARP_ENABLED
        public OpenCvSharp.Size ToCvSize()
        {
            return new OpenCvSharp.Size((int)Width, (int)Height);
        }
#endif

#if UTILS_GDI_ENABLED
        public System.Drawing.Size ToGdiSize()
        {
            return new System.Drawing.Size((int)Width, (int)Height);
        }
#endif

        public Size Clone()
        {
            return new Size(Width, Height);
        }
    }
}
