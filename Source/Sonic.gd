extends CharacterBody2D

const gravity = 980
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func HandleGfx():
	if (is_on_floor()):
		$SonicSpr.rotation = get_floor_normal().angle() + 1.5708
		$CollisionShape2D.rotation = get_floor_normal().angle() + 1.5708
		if (velocity.x > 0):
			$SonicSpr/SncAnim.play("Walking")
			$SonicSpr.flip_h = 0
		elif (velocity.x < 0):
			$SonicSpr/SncAnim.play("Walking")
			$SonicSpr.flip_h = 1
		else:
			$SonicSpr/SncAnim.play("Idle")
	else:
		$SonicSpr/SncAnim.play("Jump")

func UpdatePos(delta):
	velocity.y += gravity * delta
	move_and_slide()

func HandleInput():
	if (Input.is_action_pressed("player_left")):
		velocity.x = -200
	elif (Input.is_action_pressed("player_right")):
		velocity.x = 200
	else:
		velocity.x = 0
	if (Input.is_action_just_pressed("player_jump") and is_on_floor()):
		velocity.y -= 490
		$JumpSFX.play()
	
		
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	HandleInput()
	UpdatePos(delta)
	HandleGfx()
