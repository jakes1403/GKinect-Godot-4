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

signal playInstruct

signal playFax
signal stopFax

var playGame = true

var rng = RandomNumberGenerator.new()

var counter = 0

# Dont use nae nae at all

var poseNames = ["attention_l", "attention_r", "dab_l", "dab_r", "gangnam", "naenae_l", "naenae_r", "tpose", "whip_l", "whip_r"]

@onready var a = PoseGroup.new()

var checkingForPose = false

var doneChecked = false

var bodies

func get_current_pose():
	if bodies.size() == 0 and not doneChecked:
		print("eq")
		$TimeoutTimer.start(5)
		doneChecked = true
	if bodies.size() > 0:
		doneChecked = false
		$TimeoutTimer.stop()
	return poseNames[a.check_for_poses(0)]

func askForPose():
	rng.randomize()
	var value = rng.randi_range(0, poseDict.size() - 1)
	currentPose = poseDict[value]
	emit_signal(currentPose)
	checkingForPose = true

var instruct = false

func doIntro():
	$Music.stop()
	emit_signal("playInstruct")
	instruct = true
	

func _ready():
	a.load_model("res://poses/simon_says_model.gkposegroup")
	doIntro()
	#askForPose()

var speakingGoodBad = false

func correctPose():
	checkingForPose = false
	$SlackTimer.stop()
	emit_signal("playGoodJob")
	print("Correct pose: " + currentPose)
	speakingGoodBad = true

func wrongPose():
	checkingForPose = false
	emit_signal("playWrong")
	speakingGoodBad = true

func _process(delta):
	if Input.is_action_just_pressed("run"):
		#emit_signal("playInstruct")
		pass
	
	bodies = GKinect.get_bodies()
	if bodies.size() > 0 and playGame == false:
		emit_signal("stopFax")
		playGame = true
		doIntro()
		
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
	if speakingGoodBad:
		speakingGoodBad = false
		if playGame == true:
			askForPose()
	if instruct:
		instruct = false
		$Music.play()
		askForPose()


func _on_slack_timer_timeout():
	if checkingForPose:
		wrongPose()




func _on_timeout_timer_timeout():
	playGame = false
	$Music.stop()
	$FaxTimer.start(30)


func _on_fax_timer_timeout():
	if playGame == false:
		emit_signal("playFax")
