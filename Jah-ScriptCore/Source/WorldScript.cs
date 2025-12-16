using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jah
{
    public abstract class WorldScript
    {
        public virtual void OnCreate() { }

        public virtual void OnUpdate(float timestep) { }

        public virtual void OnDestroy() { }
    }
}
