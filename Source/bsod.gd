extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	pass

func handle_input():
	if (Input.is_action_just_pressed("bsod_exit")):
		get_tree().change_scene_to_file("res://Scenes/title.tscn")
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	handle_input()
