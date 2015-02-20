function onAnswer(cid, answer)
	
	if getQuestProgress(1, cid) == 0 then

		if answer == 0 then
				sendDialogue("Hello traveller. This is not a safe place, please finish your business here, and find somewhere else to play.",
					"1-Why are you fighting?",
					"2-I want to join you",
					cid)
			elseif answer == 1 then
				sendDialogue("The Masukara invaded our land, and are after a relic of ours. We cannot allow them to get what they're after!",
						"3-You are outnumbered",
						"4-Why are they after it?",
						"2-I want to join you",
						"5-I got to go",
						cid)
			elseif answer == 3 then
				sendDialogue("Perhaps, but we are stronger, we can take them.",
						"5-Good luck",
						"6-Why don't you attack?",
						"2-I want to join you",
						cid)
			elseif answer == 6 then
				sendDialogue("We are waiting to see if they will make the first move.", cid)
				endDialogue(cid)
			elseif answer == 4 then
				sendDialogue("They wish to use it against Ho-oh: our tribes guardian, so that he cannot protect us anymore.",
						"7-I feel as if there is more to it",
						"8-That's terrible",
						cid)
			elseif answer == 7 then
				sendDialogue("That is all you need to know, now run along.", cid)
				endDialogue(cid)
			elseif answer == 8 then
				sendDialogue("Yes, it is indeed.", cid)
				endDialogue(cid)
			elseif answer == 2 then
				sendDialogue("I'm sorry, I only accept those worthy enough to be in our elite tribe. Come back when you have proven yourself.", cid)
				endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then

		if answer == 0 then
			sendDialogue("Hello traveller. This is not a safe place, please finish your business here, and find somewhere else to play.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
		elseif answer == 1 then
			sendDialogue("Not I. That sounds like something a Masukara would do.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	end
	
end