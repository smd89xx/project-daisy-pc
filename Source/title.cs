using Godot;
using System;
using System.Threading;
using System.Threading.Tasks;

public partial class title : Node2D
{
	GDScript fadeScript = GD.Load<GDScript>(Daisy.Resources.fadeScript);
	public override void _Ready()
	{
		fadeScript.Call("fade_in");
	}
	private async void HandleInput()
	{
		if (Input.IsActionJustPressed("title_start"))
		{
			var selectSFX = GetNode<AudioStreamPlayer>("selectSFX");
			selectSFX.Play();
			fadeScript.Call("fade_out");
			await ToSignal(GetTree().CreateTimer(1.0f),SceneTreeTimer.SignalName.Timeout);
			Daisy.Utilities.BSODVarInit(Daisy.stopCodes.generic,"title.HandleInput()");
			GetTree().ChangeSceneToFile(Daisy.Resources.bsodScreen);
		}
		else if (Input.IsActionJustPressed("title_exit"))
		{
			var backSFX = GetNode<AudioStreamPlayer>("backSFX");
			backSFX.Play();
			fadeScript.Call("fade_out");
			await ToSignal(GetTree().CreateTimer(1.0f),SceneTreeTimer.SignalName.Timeout);
			System.Environment.Exit(0);
		}
	}
	public override void _Process(double delta)
	{
		HandleInput();
	}
}
