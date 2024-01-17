using Godot;
using System;

namespace Daisy
{
    class Resources
    {
        public const string bsodScreen = "res://Scenes/bsod.tscn";
        public const string titleScreen = "res://Scenes/title.tscn";
        public const string fadeScript = "res://addons/UniversalFade/Fade.gd";
    }
    class Settings
    {
        public const string viewportScale = "display/window/stretch/scale";
        public const string viewportWidth = "display/window/size/viewport_width";
        public const string viewportHeight = "display/window/size/viewport_height";
    }
    class Globals
    {
        public class BSoD
        {
            public static uint stopcode = 0;
            public static readonly string[] stopStrings = {"ERROR_GENERIC"};
            public static string crashOrigin = "vars.cs";
        }
    }
}