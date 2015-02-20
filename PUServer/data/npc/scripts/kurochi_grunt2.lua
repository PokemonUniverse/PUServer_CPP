function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then

		if answer == 0 then
			sendDialogue("Ho-Oh watches over our tribe, there is no way we will loose here today.", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Ho-oh watches over our tribe, there is no way we will loose here today.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("I couldn't possibly, Ho-oh is a gentle, yet vengeful spirit, and to do such a thing would make it angry", cid)
			endDialogue(cid)
		elseif answer == 2 then
			sendDialogue("Not a problem.", cid)
			endDialogue(cid)
		end
	end
end