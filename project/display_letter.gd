extends RichTextLabel

var alph = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

@onready var a = PoseGroup.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	a.load_model("res://poses/model.gkposegroup")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	text = "Current semaphore: " + alph[a.check_for_poses(0)]
