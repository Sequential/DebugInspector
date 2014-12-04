using System;
using System.Collections.Generic;
using System.Text;


namespace DebugInspector.Controls
{
    public interface INavItem
    {
        void NavigateTo(IEnvironment env);
    }
}
