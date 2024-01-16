using Godot;
using System;

public partial class bsod : Node2D
{
	public override void _Ready()
	{
		var bsodStopStr = GetNode<Label>("BSOD_StopTxt");
		bsodStopStr.Text = Daisy.Globals.BSoD.stopStrings[Daisy.Globals.BSoD.stopcode];
		var bsodCauseStr = GetNode<Label>("BSOD_CauseTxt");
		bsodCauseStr.Text = "The problem seems to be caused by the following:\n" + Daisy.Globals.BSoD.crashOrigin;
		var bsodHexStr = GetNode<Label>("BSOD_HexTxt");
		bsodHexStr.Text = "Technical Information:\n***** STOP: 0x" + Convert.ToString(Daisy.Globals.BSoD.stopcode,16);
	}
	private void bsodExit()
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
	private void handleInput()
	{
		if (Input.IsActionJustPressed(Daisy.InputMap.bsodExit))
		{
			bsodExit();
		}
	}
	public override void _Process(double delta)
	{
		handleInput();
	}
}
