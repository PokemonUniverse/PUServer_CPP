function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then

		if answer == 0 then
			sendDialogue("When is the boss going to let us fight? I'm getting so pumped!", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("When is the boss going to let us fight? I'm getting so pumped!",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("No, but I bet you it was one of them Kurochi guys who did it.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	end
end