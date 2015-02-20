function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then
		
		if answer == 0 then
			sendDialogue("Get out of here kid, this doesn't concern you.", cid)
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then
	
		if answer == 0 then
			sendDialogue("Get out of here kid, this doesn't concern you.",
					"1-Did you take a little girl's potion?",
					"2-Sorry to bother you",
					cid)
			elseif answer == 1 then
				sendDialogue("No, go away! *drop* oops. That potion is mine.",
						"3-It has a star on it",
						cid)
			elseif answer == 3 then
				sendDialogue("Ok, yes, I stole it from that little girl. What are you going to do about it?",
						"4-*Punch*",
						"5-Nothing",
						cid)
				setQuestProgress(cid, 1, 99)
				addItem(cid, 17, 1)
			elseif answer == 4 then
				sendDialogue("OUCH! Fine, here you go. Now get out of here.",
						"7-Thank you very much",
						"8-*Punch*",
						cid)
			elseif answer == 7 then
				endDialogue(cid)
			elseif answer == 8 then
				sendDialogue("Arrg! GO AWAY!", cid)
				endDialogue(cid)

			elseif answer == 5 then
				sendDialogue("That's right, run away kid.", cid)
				endDialogue(cid)
			
			elseif answer == 2 then
				sendDialogue("I bet you are.", cid)
				endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 99 then
		
		if answer == 0 then
			sendDialogue("Go away before I get beat you.",
					"10-You and what army?",
					"11-Alright, I'm going",
					cid)
			elseif answer == 10 then
				sendDialogue("The one standing around me.",
						"11-Right...Bye",
						cid)
			elseif answer == 11 then
				endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 100 then

		if answer == 0 then
			sendDialogue("Go away before I get beat you.",
					"10-You and what army?",
					"11-Alright, I'm going",
					cid)
			elseif answer == 10 then
				sendDialogue("The one standing around me.",
							"11-Right...Bye",
							cid)
			elseif answer == 11 then
				endDialogue(cid)
		end
		
	end
end