﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Util
{
    public class Point
    {
        public double X;
        public double Y;

        public Point(double X, double Y)
        {
            this.X = X;
            this.Y = Y;
        }

        public Point()
        {

        }

        public double EucludianLength()
        {
            return Math.Sqrt(X*X+Y*Y);
        }

        public static Point operator +(Point a, double b)
        {
            return new Point(a.X + b, a.Y + b);
        }

        public static Point operator *(Point a, double b)
        {
            return new Point(a.X * b, a.Y * b);
        }

        public static Point operator *(double a, Point b)
        {
            return b * a;
        }

        public static Point operator /(Point a, double b)
        {
            return new Point(a.X / b, a.Y / b);
        }

        public static Point operator +(Point a, Point b)
        {
            return new Point(a.X + b.X, a.Y + b.Y);
        }

        public static Point operator - (Point a, Point b)
        {
            return new Point(a.X - b.X, a.Y - b.Y);
        }

        public static Point Rotate(Point pt, double alpha)
        {
            throw new NotImplementedException();
        }

        public static double EucludianDistance(Point a, Point b)
        {
            return (a - b).EucludianLength();
        }

        public override string ToString()
        {
            return $"({X}, {Y})";
        }

        public Point Clone()
        {
            return new Point(X, Y);
        }

#if UTILS_OPENCVSHARP_ENABLED

        public OpenCvSharp.Point ToCvPoint()
        {
            return new OpenCvSharp.Point(X, Y);
        }

        public static explicit operator OpenCvSharp.Point(Point t)
        {
            return t.ToCvPoint();
        }

        public static implicit operator Point(OpenCvSharp.Point t)
        {
            return new Point(t.X, t.Y);
        }

#endif

    }

    public class Point3D
    {
        public double X;
        public double Y;
        public double Z;

        public Point3D(double X, double Y, double Z)
        {
            this.X = X;
            this.Y = Y;
            this.Z = Z;
        }

        public Point3D(double[] array)
        {
            if (array == null)
                throw new ArgumentNullException(nameof(array));
            if (array.Length != 3)
                throw new ArgumentOutOfRangeException(nameof(array));

            X = array[0];
            Y = array[1];
            Z = array[2];
        }

        public Point3D()
        {

        }

        public static Point3D operator +(Point3D a, Point3D b)
        {
            return new Point3D(a.X+b.X, a.Y+b.Y, a.Z+b.Z);
        }

        public static Point3D operator -(Point3D a, Point3D b)
        {
            return new Point3D(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        public static Point3D operator *(Point3D a, double d)
        {
            return new Point3D(a.X * d, a.Y * d, a.Z * d);
        }

        public static Point3D operator *(double d, Point3D a)
        {
            return a * d;
        }

        public double[] ToArray()
        {
            return new double[] { X, Y, Z };
        }

        public override string ToString()
        {
            return $"({X}, {Y}, {Z})";
        }
    }
}
