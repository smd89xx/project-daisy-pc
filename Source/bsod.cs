using Godot;
using System;

public partial class bsod : Node2D
{
	GDScript fadeScript = GD.Load<GDScript>(Daisy.Resources.fadeScript);
	public override void _Ready()
	{
		var bsodStopStr = GetNode<Label>("BSOD_StopTxt");
		bsodStopStr.Text = Daisy.Globals.BSoD.stopStrings[Daisy.Globals.BSoD.stopcode];
		var bsodCauseStr = GetNode<Label>("BSOD_CauseTxt");
		bsodCauseStr.Text = "The problem seems to be caused by the following:\n" + Daisy.Globals.BSoD.crashOrigin;
		var bsodHexStr = GetNode<Label>("BSOD_HexTxt");
		bsodHexStr.Text = "Technical Information:\n***** STOP: 0x" + Convert.ToString(Daisy.Globals.BSoD.stopcode,16);
		fadeScript.Call("fade_in",0.1);
	}
	private void BSODExit()
	{
		switch (Daisy.Globals.BSoD.stopcode)
		{
			default:
			{
				GetTree().ChangeSceneToFile(Daisy.Resources.titleScreen);
				break;
			}
		}
	}
	private void HandleInput()
	{
		if (Input.IsActionJustPressed("bsod_exit"))
		{
			BSODExit();
		}
	}
	public override void _Process(double delta)
	{
		HandleInput();
	}
}
