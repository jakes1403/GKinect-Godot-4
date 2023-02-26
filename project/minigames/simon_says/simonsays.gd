extends Node2D

var poseDict = ["playTpose", "playGangnam", "playAttention", "playDab", "playWhip"]

var currentPose = ""

signal playGangnam
signal playTpose

signal playGoodJob
signal playAttention
signal playWrong
signal playDab
signal playWhip

var rng = RandomNumberGenerator.new()

var counter = 0

# Dont use nae nae at all

var poseNames = ["attention_l", "attention_r", "dab_l", "dab_r", "gangnam", "naenae_l", "naenae_r", "tpose", "whip_l", "whip_r"]

@onready var a = PoseGroup.new()

var checkingForPose = false

func get_current_pose():
	GKinect.get_bodies()
	return poseNames[a.check_for_poses(0)]

func askForPose():
	rng.randomize()
	var value = rng.randi_range(0, poseDict.size() - 1)
	currentPose = poseDict[value]
	emit_signal(currentPose)
	checkingForPose = true

func _ready():
	a.load_model("res://poses/simon_says_model.gkposegroup")

func correctPose():
	checkingForPose = false
	$SlackTimer.stop()
	emit_signal("playGoodJob")
	print("Correct pose: " + currentPose)

func wrongPose():
	checkingForPose = false
	emit_signal("playWrong")

func _process(delta):
	if Input.is_action_just_pressed("run"):
		askForPose()
		
	if (not $SlackTimer.is_stopped()) and checkingForPose:
		if get_current_pose() == "tpose" and currentPose == "playTpose":
			correctPose()
		elif get_current_pose() == "gangnam" and currentPose == "playGangnam":
			correctPose()
		elif (get_current_pose() == "attention_l" or get_current_pose() == "attention_r") and currentPose == "playAttention":
			correctPose()
		elif (get_current_pose() == "dab_l" or get_current_pose() == "dab_r") and currentPose == "playDab":
			correctPose()
		elif (get_current_pose() == "whip_l" or get_current_pose() == "whip_r") and currentPose == "playWhip":
			correctPose()


func _on_horse_done_speaking():
	if checkingForPose:
		$SlackTimer.start()


func _on_slack_timer_timeout():
	if checkingForPose:
		wrongPose()
