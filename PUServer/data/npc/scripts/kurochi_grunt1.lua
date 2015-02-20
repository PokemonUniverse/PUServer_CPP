function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then

		if answer == 0 then
			sendDialogue("Why are the Masukara attacking us?", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Why are the Masukara attacking us?",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("No, it wasn't me.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	end
end