function onAnswer(cid, answer)
	if answer == 0 then
			sendDialogue("Hello " .. getCreatureName(cid) .. ". It is good to see you",
					"1-It's good to see you too",
					"2-Hello Jerry!",
					cid)
		elseif answer == 1 then
			sendDialogue("Yeah, a lot of thing's seem to be going good today: the sun is shining, there is a slight breaze, and my coffee is extra soothing today.",
					"11-Well I got to get going",
					"12-I would prefer if there was no wind",
					cid)
		elseif answer == 11 then
				sendDialogue("Are you sure? You could stay a while. I have so much I want to tell you.",
						"15-Yes, I really need to go",
						"16-Well, I suppose I could stick around",
						cid)
		elseif answer == 15 then
					sendDialogue("Well, I won't keep you then. One question though, where are you going?",
							"21-Home",
							"22-To the next gym",
							"23-Away from you",
							"24-Nowhere",
							cid)
		elseif answer == 21 then
						sendDialogue("Please come back and talk again, this has been fun.", cid)
						endDialogue(cid)
		elseif answer == 22 then
						sendDialogue("I hear that can be pretty tough, are you sure you're up for it?",
								"25-Yup",
								"26-GOODBYE!",
								cid)
		elseif answer == 25 then
							sendDialogue("Well that's good, I hope you are correct. Good luck.", cid)
							endDialogue(cid)
		elseif answer == 26 then
						sendDialogue("You don't have to be rude about it! Fine, goodbye!", cid)
							endDialogue(cid)
		elseif answer == 23 then
						sendDialogue("That's pretty veague! Are you going North? South? East? West?",
								"31-Yup",
								"26-GOODBYE!",
								cid)
		elseif answer == 31 then
							sendDialogue("You're confusing.", cid)
							endDialogue(cid)
		elseif answer == 24 then
						sendDialogue("You're going nowhere? How do you get nowhere?",
								"35-It's five minutes east of somewhere",
								"26-GOODBYE!",
								cid)
		elseif answer == 35 then
							sendDialogue(". . .", cid)
							endDialogue(cid)
		elseif answer == 16 then
					sendDialogue("That's terrific; I'm glad you can stay! Did you know I'm a clone?",
							"40-No",
							"40-You're a clone?",
							"41-Yes",
							cid)
		elseif answer == 40 then
						sendDialogue("Yes, I am a clone of this Jerry guy. He won some quest contest, and here I am.",
								"45-What was his quest about?",
								"46-That's pretty cool!",
								cid)
		elseif answer == 45 then
							sendDialogue("It's about this guy named Byron, I don't want to say a whole lot more.",
									"50-Why were you chosen to win?",
									"51-That's pretty cool, I'm excited",
									"52-That's a stupid quest!",
									cid)
		elseif answer == 50 then
								sendDialogue("Spirit felt that the story delved into the human psychy pretty well, he likes that sort of thing. You should keep that in mind, if you decide to write a quest!",
										"55-I will, thanks",
										"52-That's stupid!",
										"56-goodbye",
										cid)
		elseif answer == 55 then
									sendDialogue("That's good, I look foreword to reading a submission of yours on the forum.", cid)
									endDialogue(cid)
		elseif answer == 56 then
									sendDialogue("Fairwell traveller.", cid)
									endDialogue(cid)
		elseif answer == 51 then
								sendDialogue("Thank's, me too!", cid)
								endDialogue(cid)
		elseif answer == 52 then
								sendDialogue("Yeah? Well I'm the one with the 1st place win, aren't I?", cid)
								endDialogue(cid)
		elseif answer == 41 then
						sendDialogue("You did? Well, I have run out of things to say. Goodbye. Please stop by again!", cid)
						endDialogue(cid)
		elseif answer == 12 then
				sendDialogue("I suppose that would be nice! It might be cool if there were some Buneary and Ledyba around outside too.",
						"61-That would be cool",
						"62-Nah, then it would be too cluttered",
						"63-And the occasional light rain",
						cid)
		elseif answer == 61 then
					sendDialogue("Yup!", cid)
					endDialogue(cid)
		elseif answer == 62 then
					sendDialogue("I suppose you're right, but not if it only happened once in a while.",
							"65-Perhaps",
							"66-No, I still don't think so",
							cid)
		elseif answer == 65 then
						sendDialogue("Yes, perhaps.", cid)
						endDialogue(cid)
		elseif answer == 66 then
						sendDialogue("Well, that is your oppinion", cid)
						endDialogue(cid)
		elseif answer == 63 then
					sendDialogue("Oh yeah, that would be awesome! And some water pokemon would come out and frolic in it too.",
							"71-Mmmm, yeah!",
							"72-Now we're just pushing it",
							cid)
		elseif answer == 71 then
						sendDialogue("Mmmm indeed!", cid)
						endDialogue(cid)
		elseif answer == 72 then
						sendDialogue("Maybe.", cid)
						endDialogue(cid)
		elseif answer == 2 then
			sendDialogue("What can I do for you?",
					"5-Ah, nothing, just came to chat",
					"40-Wait...Jerry? Are you a clone?",
					"6-Nothing, i'll see you later.",
					cid)
		elseif answer == 5 then
				sendDialogue("Oh, ok! Plus 1 comment!",cid)
				endDialogue(cid)
		elseif answer == 6 then
				sendDialogue("Alright, goodbye!",cid)
				endDialogue(cid)
	end
end