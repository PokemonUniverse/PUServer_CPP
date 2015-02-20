function onAnswer(cid, answer)
	if getQuestProgress(1, cid) == 0 then 
	
		if answer == 0 then
			sendDialogue("Oh deer!",
						"1-What's wrong?",
						"2-Umm....bye",
						cid)
		elseif answer == 1 then
			sendDialogue("My Jigglypuff is hurt, and I don't know what to do.",
						"3-Go to a pokemon center",
						"2-That's too bad",
						cid)
		elseif answer == 3 then
			sendDialogue("I can't, I have to wait for my brother to get here, otherwise he might think I was kidnapped!",
						"4-Do you have any potions?",
						"2-Well, good luck",
						cid)
		elseif answer == 4 then
			sendDialogue("A man bumped into me earlier and stole my only potion. What am I going to do?",
						"5-Have you told the police?",
						"6-I'll get you that potion",
						"7-You have really bad luck",
						cid)
		elseif answer == 5 then
			sendDialogue("I haven't seen any police here on Miryus yet!",
					"2-Hmm...too bad, well good luck to you",
					"6-What if I get that potion back?",
				cid)
		elseif answer == 6 then
			sendDialogue("Would you really?",
						"8-Yes",
						"9-No",
				cid)
		elseif answer == 8 then
			setQuestProgress(cid, 1, 3)
			sendDialogue("Oh, Thank you so much, I don't remember what the man looked like, but if you look around, I'm sure you will find him.", cid)
			endDialogue(cid)
		elseif answer == 9 then
			setQuestProgress(cid, 1, 2)
			sendDialogue("Oh...bye...", cid)
			endDialogue(cid)
		elseif answer == 7 then
			sendDialogue("I know. Thank you for your concern. I'll let you go now.", cid)
			endDialogue(cid)
		elseif answer == 2 then
			sendDialogue("Bye trainer, I hope you do better than I am", cid)
			endDialogue(cid)
		end
		
	elseif getQuestProgress(1, cid) == 2 then
	
		if answer == 0 then
			sendDialogue("I knew you would come back! Are you going to help me get my potion for my Jigglypuff?",
				"10-Yes",
				"11-No",
				cid) -- MISSED A ) here
		elseif answer == 10 then
			setQuestProgress(cid, 1, 3)
			sendDialogue("Oh, Thank you so much, I don't remember what the man looked like, but if you look around, I'm sure you will find him. I put a star sticker on my potion, so you will know when you find it.", cid)
			endDialogue(cid)
		elseif answer == 11 then
			sendDialogue("Oh...bye...", cid)
			endDialogue(cid)
		end
		
	elseif getQuestProgress(1, cid) == 99 then
	
		if answer == 0 then -- FORGOT answer == 0, otherwise it's always showing this dialog
			removeItem(cid, 17, 1)
			sendDialogue("Oh, thank you trainer! I knew you would do it! Unfortunately my brother isn't back yet, but at least my Jigglypuff is safe now! Thank you so much!",
						"21-What, no reward?",
						"22-No problem",
						cid)
						setQuestProgress(cid, 1, 100)
		elseif answer == 21 then -- PUT THE == at the wrong side
			sendDialogue("I'm sorry, I don't have anything to offer you. If I ever think of a way to repay you, I will.", cid)
			endDialogue(cid)
		elseif answer == 22 then
			endDialogue(cid)
		end

	elseif getQuestProgress(1, cid) == 3 then
		
		if answer == 0 then
			sendDialogue("Please hurry, my Jigglypuff is really hurt.",
					"15-How will I know it's your potion?",
					"16-What did the guy look like?",
					"17-I'll find it",
					cid)
		elseif answer == 15 then
			sendDialogue("I put a sticker of a star on the bottle.",
					"18-Why did you do that?",
					"19-Got it",
					cid)
		elseif answer == 18 then
			sendDialogue("I put star stickers on all my stuff.", cid)
			endDialogue(cid)
		elseif answer == 19 then
			sendDialogue("Please hurry.", cid)
			endDialogue(cid)
		elseif answer == 16 then
			sendDialogue("I'm sorry, I don't remember.", cid)
			endDialogue(cid)
		elseif answer == 17 then
			sendDialogue("Thank you.", cid)
			endDialogue(cid)
		end
		
	elseif getQuestProgress(1, cid) == 100 then
		sendDialogue("Thank you once again Trainer! Now I just wish my brother would show up so we can go home.", cid)
		endDialogue(cid)
	end
end