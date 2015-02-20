function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		
			if answer == 0 then
			sendDialogue("Those Kurochi, how I hate them.",
					"1-What did they do?",
					"2-I don't",
					"3-Me too",
					cid)
			elseif answer == 1 then
				sendDialogue("I don't know all the details, but our teams have always hated eachother.",
						"4-Sounds like a stupid reason",
						"5-Fair enough",
						cid)
			elseif answer == 4 then
				sendDialogue("Get lost kid!", cid)
				endDialogue(cid)
			elseif answer == 5 then
				endDialogue(cid)
			elseif Answer == 2 then
				sendDialogue("If it weren't for the boss telling us not to attack, I would beat you. Get out of here before I am unable to hold back.", cid)
				endDialogue(cid)
			elseif Answer == 3 then
				sendDialogue("Yeah? You should talk to the boss about joining us than.", cid)
				endDialogue(cid)
			end
	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Those Kurochi, how I hate them.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("How dare you accuse me of such an act. Get out of my sight.", cid)
			endDialogue(cid)
		end

	end
end