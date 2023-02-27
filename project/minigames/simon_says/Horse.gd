extends AnimatedSprite2D

signal doneSpeaking

var timed = 0

func playFax():
	$Fax.connect("finished", done)
	$FaxBackwards.connect("finished", done)
	timed+=1
	frame = 0
	var backwards = timed % 2 == 0
	if not backwards:
		play("fax")
		$Fax.play()
	else:
		play_backwards("fax")
		$FaxBackwards.play()
	

func stopFax():
	frame = 0
	stop()
	$Fax.stop()
	$FaxBackwards.stop()

func playInstruct():
	frame = 0
	play("simoninstructs")
	$Instruct.connect("finished", done)
	$Instruct.play(0.5)

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



