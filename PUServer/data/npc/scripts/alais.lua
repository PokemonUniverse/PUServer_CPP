function onAnswer(cid, answer)
	if answer == 0 then
		sendDialogue("I own at cooking!",
					"1-All hail Alais",
					cid)
	elseif answer == 1 then
		sendDialogue("Yes all hail me!", cid)
		endDialogue(cid)
	end
end