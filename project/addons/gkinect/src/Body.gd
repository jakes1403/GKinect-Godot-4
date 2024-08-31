@tool
extends Node2D
@onready var g = preload("res://addons/gkinect/point.tscn")

var l = []

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in range(25):
		l.push_back(g.instantiate())
		add_child(l[i])


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in range(25):
		GKinect.get_bodies()
		l[i].position = GKinect.get_color_point(i) * 0.5
