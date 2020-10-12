using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Yabe
{
    public interface IYabePlugin
    {
        void Init(YabeMainDialog yabeFrm);
    }
}
