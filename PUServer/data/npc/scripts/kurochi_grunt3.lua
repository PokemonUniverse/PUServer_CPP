function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then

		if answer == 0 then
			sendDialogue("Those Masukara don't stand a chance! I can't wait to finish them off for good", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Those Masukara don't stand a chance! I can't wait to finish them off for good",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("No, I haven't done anything like that, but if it was from a Masukara, I would have taken the potion, and everything else he had.",
					"3-Erm, ok",
					cid)
			endDialogue(cid)
		elseif answer == 2 then
			sendDialogue("Not a problem.", cid)
		elseif answer == 3 then
			endDialogue(cid)
		end
		
	end
end