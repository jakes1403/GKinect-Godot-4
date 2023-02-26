extends Node3D

@export var body_part = "head"

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	# print(alph[GKinect.run_model()])
	var bodies = GKinect.get_bodies()
	if bodies.size() > 0:
		transform.origin = bodies[0][body_part] * 5
		transform.origin.x *= -1
