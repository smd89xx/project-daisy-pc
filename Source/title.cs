using Godot;
using System;

public partial class title : Node2D
{
	public override void _Ready()
	{
		
	}
	private void handleInput()
	{
		if (Input.IsActionJustPressed(Daisy.InputMap.titleStart))
		{
			Daisy.Utilities.BSODVarInit(Daisy.stopCodes.generic,"title.handleInput()");
			GetTree().ChangeSceneToFile(Daisy.Resources.bsodScreen);
		}
		else if (Input.IsActionJustPressed(Daisy.InputMap.titleExit))
		{
			System.Environment.Exit(0);
		}
	}
	public override void _Process(double delta)
	{
		handleInput();
	}
}
