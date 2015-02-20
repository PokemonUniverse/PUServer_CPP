function onAnswer(cid, answer)
	if answer == 0 then
			sendDialogue("That guy over there is way too drunk! The sad thing is he has only had 1 beer.", cid)
			endDialogue(cid)
	end
end