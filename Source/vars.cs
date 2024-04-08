using Godot;
using System;
using System.Data;
using System.Reflection.Metadata;

namespace Daisy
{
	class Resources
	{
		public const string bsodScreen = "res://Scenes/bsod.tscn";
		public const string titleScreen = "res://Scenes/title.tscn";
		public const string fadeScript = "res://addons/UniversalFade/Fade.gd";
		public const string saveScreen = "res://Scenes/savescrn.tscn";
		public static readonly string[] levels = {"res://Scenes/test_level.tscn","extra test string"};
		public const string savePath = "save.bin";
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
		public static SaveMData[] SaveSlots = new SaveMData[9];

		public static byte saveSlot = 0;
		public static short brightness = 255;
		public const byte fadeSpeed = 4;
		public static FadeStates fadeState = FadeStates.off;
	}
}
