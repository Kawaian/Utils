using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Util
{
    //ref. https://tinyurl.com/y9ay5kpy
    public class KalmanFilter
    {
        double A = 1;
        double H = 1;
        double P = 0.1;
        double Q = 0.125;
        double R = 1;
        double K;
        double z;
        double x;
        bool initialized = false;

        public double Calculate(double d)
        {
            if (!initialized)
            {
                x = d;
                initialized = true;
            }

            z = d;

            x = A * x;
            P = A * P * A + Q;

            K = P * H / (H * P * H + R);
            x = x + K * (z - H * x);
            P = (1 - K * H) * P;

            return x;
        }

        public KalmanFilter Clone()
        {
            KalmanFilter filter = new KalmanFilter
            {
                x = x,
                z = z,
                P = P,
                K = K,
                initialized = initialized
            };

            return filter;
        }
    }

    public class PointKalmanFilter
    {
        KalmanFilter filterX = new KalmanFilter();
        KalmanFilter filterY = new KalmanFilter();

        public Point Calculate(Point pt)
        {
            Point ret = new Point(0, 0)
            {
                X = filterX.Calculate(pt.X),
                Y = filterY.Calculate(pt.Y)
            };
            return ret;
        }
    }

    public class ArrayKalmanFilter
    {
        KalmanFilter[] filters;

        public ArrayKalmanFilter(int length)
        {
            filters = new KalmanFilter[length];
            for (int i = 0; i < length; i++)
                filters[i] = new KalmanFilter();
        }

        public double[] Calculate(double[] array)
        {
            if (filters == null)
            {
                filters = new KalmanFilter[array.Length];
                for (int i = 0; i < filters.Length; i++)
                {
                    filters[i] = new KalmanFilter();
                }
            }

            if (array.Length != filters.Length)
                throw new ArgumentOutOfRangeException();

            double[] ret = new double[array.Length];
            for (int i = 0; i < array.Length; i++)
            {
                ret[i] = filters[i].Calculate(array[i]);
            }
            return ret;
        }

        public ArrayKalmanFilter Clone()
        {
            ArrayKalmanFilter filter = new ArrayKalmanFilter(filters.Length);
            for (int i = 0; i < filters.Length; i++)
                filter.filters[i] = filters[i].Clone();
            return filter;
        }
    }
}
