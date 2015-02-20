function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 and getQuestProgress(2, cid) == 0 then
			if answer == 0 then
				sendDialogue("Who are you?",
					"1-" .. getCreatureName(cid) .."",
					"2-Nobody",
					cid)
			elseif answer == 1 then
				setQuestProgress(cid, 2, 1)
				sendDialogue("Well ..getCreatureName(cid).., This is no place for a kid, go home.",
						"3-I want to join your team",
						"4-Yes sir",
						"5-What is this about?",
						cid)
			elseif answer == 3 then
				sendDialogue("I see. Unfortunately we are not recruiting right now, check back later.",
						"6-Thank you",
						"6-Too bad",
						cid)
			elseif answer == 6 then
				sendDialogue("Mmhmm. Now please, get out of here before you get hurt.", cid)
				endDialogue(cid)
			elseif answer == 4 then
				endDialogue(cid)
			elseif answer == 5 then
				sendDialogue("Us and the Kurochi are at war. They say we intruded on their land, but they haven't claimed it yet. Soon Lugia will be ours and the Kurochi will be no more.",
						"7-That sounds a little cliche",
						"6-Wow, well I got to go",
						cid)
			elseif answer == 7 then
				sendDialogue("Trust me, it goes further than just two rival teams, but I cannot give any more information to some insolent brat. Now beat it.", cid)
				endDialogue(cid)
			elseif answer == 2 then
				sendDialogue("If you are not going to tell me who you are, then kindly leave.", cid)
				endDialogue(cid)
			end
	elseif getQuestProgress(2, cid) == 1 then
			if answer == 0 then
				sendDialogue("" .. getCreatureName(cid) .. ", This is no place for a kid, go home.",
						"23-I want to join your team",
						"24-Yes sir",
						"25-What is this about?",
						cid)
			elseif answer == 23 then
				sendDialogue("I see. Unfortunately we are not recruiting right now, check back later.",
						"6-Thank you",
						"6-Too bad",
						cid)
			elseif answer == 26 then
				sendDialogue("Mmhmm. Now please, get out of here before you get hurt.", cid)
				endDialogue(cid)
			elseif answer == 24 then
				endDialogue(cid)
			elseif answer == 25 then
				sendDialogue("Us and the Kurochi are at war. They say we intruded on their land, but they haven't claimed it yet. Soon Lugia will be ours and the Kurochi will be no more.",
						"7-That sounds a little cliche",
						"6-Wow, well I got to go",
						cid)
			elseif answer == 27 then
				sendDialogue("Trust me, it goes further than just two rival teams, but I cannot give any more information to some insolent brat. Now beat it.", cid)
				endDialogue(cid)
			end

	elseif getQuestProgress(1, cid) == 3 and getQuestProgress(2, cid) == 0 then
		if answer == 0 then
			sendDialogue("Who are you?",
					"10-" .. getCreatureName(cid) .. "",
					cid)
			elseif answer == 1 then
				setQuestProgress(cid, 2, 1)
				sendDialogue("Well " ..getCreatureName(cid) .. ", This is no place for a kid, go home.",
					"11-Did you take a little girl's potion?",
					"12-Sorry to bother you",
					cid)
		elseif answer == 11 then
			sendDialogue("I cannot believe you would think a Masukara would do such a disgusting thing. Go ask some of the Kurochi.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end
	elseif getQuestProgress(1, cid) == 3 and getQuestProgress(2, cid) == 1 then
		if answer == 0 then
				sendDialogue("" .. getCreatureName(cid) .. ", This is no place for a kid, go home.",
					"11-Did you take a little girl's potion?",
					"12-Sorry to bother you",
					cid)
		elseif answer == 11 then
			sendDialogue("I cannot believe you would think a Masukara would do such a disgusting thing. Go ask some of the Kurochi.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			endDialogue(cid)
		end

	end
end