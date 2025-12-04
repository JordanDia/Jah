using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Jah
{

    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0, 0, 0);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator*(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator+(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 vec, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorDot(ref Vector3 vec);



        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetTranslation(ulong uuid, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetTranslation(ulong uuid, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
    }

    public class Entity
    {
        protected Entity()
        {
            UUID = 0;
        }

        internal Entity(ulong uuid)
        {
            UUID = uuid;
        }

        public readonly ulong UUID;

        public float FloatVar { get; set; }

       

        public void OnCreate()
        {
            Console.WriteLine("OnCreate");
        }

        public void OnUpdate(float timestep)
        {
            Console.WriteLine("OnUpdate");
        }


        public void OnDestroy()
        {
            Console.WriteLine("OnDestroy");
        }


        public Vector3 Translation
        { 
            get
            {
                InternalCalls.Entity_GetTranslation(UUID, out Vector3 translation);
                return translation;
            }

            set
            {
                InternalCalls.Entity_SetTranslation(UUID, ref value);
            }
        }
    }
}