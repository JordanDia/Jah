using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jah
{
    public class Setup : WorldScript
    {

        public override void OnCreate()
        {
            Console.WriteLine("Setting up game");

            var player = World.Find("Player");
            Console.WriteLine(player != null ? "Found player!" : "Player not found!");


            player.Translation += new Vector3(0, 2, 0);

            var newQuad = World.CreateQuad();
        }

        public override void OnUpdate(float timestep)
        {
            Console.WriteLine("World ticking");
        }

    }
}
