using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jah
{
    public static class World
    {
        public static Entity Find(string name)

        {
            // internal call to C++
            ulong uuid = InternalCalls.World_FindEntityByName(name);
            if (uuid == 0)
                return null;

            return new Entity(uuid);
        }
        public static Entity CreateQuad()
        {
            ulong uuid = InternalCalls.World_CreateQuad();

            return new Entity(uuid);
        }
    }
}
