using System;

namespace Jah {

    public class Main
    {

        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main consturctor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello from C#!");
        }

        public void PrintInt(int x)
        {
            Console.WriteLine($"C# says { x }");
        }

        public void PrintInts(int x, int y)
        {
            Console.WriteLine($"{x} and {y}");
        }

    }

}