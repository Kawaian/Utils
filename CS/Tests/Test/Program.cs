using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Test
{
    class Program
    {
        static Util.Random RNG = new Util.Random();
        static Random sysRNG = new Random();

        static List<long> elapsedTimes = new List<long>();
        

        static void Main(string[] args)
        {
            Console.Write("-------------------------------------   -------------------------------------\n" +
                          "|        XORSHIFT INTEGER GEN       |   |        XORSHIFT DOUBLE GEN        |\n" +
                          "-------------------------------------   -------------------------------------\n");
            Console.ForegroundColor = ConsoleColor.Gray;

            string GenRandomInt() => RNG.NextInt(0, 99).ToString();
            string GenRandomDouble() => RNG.NextDouble(0, 1).ToString("N1");

            for (int i = 0; i < 10; i++)
            {
                Console.WriteLine(GetRNGLine(GenRandomInt, 10, 3) + "   " + GetRNGLine(GenRandomDouble, 10, 3));
                Console.WriteLine("-------------------------------------   -------------------------------------");
            }

            Console.ForegroundColor = ConsoleColor.White;
            Console.Write("\nNow benchmarking - Press ENTER to continue");
            Console.Read();
            Console.WriteLine();

            // Benchmark System.Random
            const ushort IterCount = 100;
            for (var i = 0; i < IterCount+1; i++)
            {
                RenderProgressBar("Benchmarking System.Random", i, IterCount);

                Stopwatch timer = new Stopwatch();
                timer.Start();
                {
                    for (var j = 0; j < 100_000_000 / 10; j++)
                    {
                        _ = sysRNG.Next(0, 1000);
                        _ = sysRNG.Next(0, 1000);
                    }
                } timer.Stop();

                elapsedTimes.Add(timer.ElapsedMilliseconds);
            } 

            Console.WriteLine($"\nAverage computation time: {elapsedTimes.Average()} miliseconds");
            elapsedTimes.Clear();

            // Benchmark Util.Random
            for (var i = 0; i < IterCount + 1; i++)
            {
                RenderProgressBar("Benchmarking Util.Random", i, IterCount);

                Stopwatch timer = new Stopwatch();
                timer.Start();
                {
                    for (var j = 0; j < 100_000_000 / 10; j++)
                    {
                        _ = RNG.NextInt(0, 1000);
                        _ = RNG.NextInt(0, 1000);
                    }
                }
                timer.Stop();

                elapsedTimes.Add(timer.ElapsedMilliseconds);
            }
            Console.WriteLine($"\nAverage computation time: {elapsedTimes.Average()} miliseconds");
            Console.WriteLine();
        }

        static string GetRNGLine(Func<String> generator, int count, int pad)
        {
            StringBuilder str = new StringBuilder("|");
            for (int i = 1; i < count; i++)
                str.Append(generator().PadLeft(pad) + "|");

            return str.ToString();
        }

        public static void RenderProgressBar(string stepDescription, int progress, int total)
        {
            int totalChunks = 30;

            Console.CursorLeft = 0;
            Console.CursorLeft = totalChunks + 1;
            Console.CursorLeft = 1;

            double pctComplete = Convert.ToDouble(progress) / total;
            int numChunksComplete = Convert.ToInt16(totalChunks * pctComplete);

            Console.BackgroundColor = ConsoleColor.DarkGreen;
            Console.Write("".PadRight(numChunksComplete));

            Console.BackgroundColor = ConsoleColor.DarkGray;
            Console.Write("".PadRight(totalChunks - numChunksComplete));
            
            Console.CursorLeft = totalChunks + 5;
            Console.BackgroundColor = ConsoleColor.Black;

            string output = progress.ToString() + " of " + total.ToString();
            Console.Write(output.PadRight(15) + stepDescription);
        }
    }
}
