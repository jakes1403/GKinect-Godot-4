extends AnimatedSprite2D

signal doneSpeaking

func playWhip():
	frame = 0
	play("whip")
	$Whip.connect("finished", done)
	$Whip.play()

func playDab():
	frame = 0
	play("dab")
	$Dab.connect("finished", done)
	$Dab.play()

func playWrong():
	frame = 0
	play("wrong")
	$Wrong.connect("finished", done)
	$Wrong.play()

func playAttention():
	frame = 0
	play("attention")
	$Attention.connect("finished", done)
	$Attention.play()

func playGoodJob():
	frame = 0
	play("goodjob")
	$GoodJob.connect("finished", done)
	$GoodJob.play()



func playGangnam():
	frame = 0
	play("gangnam")
	$Gangnam.connect("finished", done)
	$Gangnam.play()

func playTpose():
	frame = 0
	play("tpose")
	$TPose.connect("finished", done)
	$TPose.play()

func done():
	emit_signal("doneSpeaking")
