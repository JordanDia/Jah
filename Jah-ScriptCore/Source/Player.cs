using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Jah;

namespace Sandbox
{
    public class Player : Entity
    {

        new void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {UUID}");
        }

        new void OnUpdate(float timestep)
        {
            //Console.WriteLine($"Player.OnUpdate: {timestep}");

            float speed = 3.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed * timestep;

            Vector3 translation = Translation;

            translation += velocity;
            Translation = translation;
        }

        new void OnDestroy()
        {
            Console.WriteLine("Player.OnDestroy");
        }

    }
}
