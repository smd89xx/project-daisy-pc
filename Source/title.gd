extends Node

# Called when the node enters the scene tree for the first time.
func _ready():
	$FadeScene/FadeRect/FadeAnim.play_backwards("Fade");
	
func handle_input():
	if (Input.is_action_just_pressed("title_start")):
		$selectSFX.play()
		$FadeScene/FadeRect/FadeAnim.play("Fade")
		$titleMus/MusFade.play("Fade")
		await get_tree().create_timer(1.85).timeout
		get_tree().change_scene_to_file("res://Scenes/test_level.tscn")
	elif (Input.is_action_just_pressed("title_exit")):
		$selectSFX.play()
		$FadeScene/FadeRect/FadeAnim.play("Fade")
		$titleMus/MusFade.play("Fade")
		await get_tree().create_timer(1.85).timeout
		get_tree().quit(0);
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	handle_input()
