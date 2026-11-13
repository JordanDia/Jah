using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jah
{
    public class Setup : WorldScript
    {

        private Entity player;
        private float spawnTimer;

        public override void OnCreate()
        {
            Console.WriteLine("Setting up game");

            player = World.Find("Player");
            Console.WriteLine(player != null ? "Found player!" : "Player not found!");

            var newQuad = World.CreateQuad();
        }

        public override void OnUpdate(float timestep)
        {
            spawnTimer += timestep;
            if (spawnTimer >= 1.0f) // every 1 second
            {
                // spawn a new quad
                var newQuad = World.CreateQuad();
                spawnTimer = 0f; // reset timer
            }
        }

    }
}
