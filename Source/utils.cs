using Godot;
using System;

namespace Daisy
{
    class Utilities
    {
        public static void BSODVarInit(stopCodes stopcode = stopCodes.generic, string origin = "printerr(uint,string)")
        {
            Globals.BSoD.stopcode = (uint)stopcode;
            Globals.BSoD.crashOrigin = origin;
        }
    }
}