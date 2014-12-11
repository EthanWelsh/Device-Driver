#include<stdio.h>
FILE *cardDeck;

/////////////////////////////////////////////
/*
    Function: cardDraw
    output: 1 card value between 1-11.
    input: an address to aces, to update it if there is an ace.
*/
////////////////////////////////////////////
int cardDraw(int *aces){
    char card[1]; //Is an array so I don't have to deal with &address
	int cardValue; //Return value
	fread(card, 1, 1, cardDeck);
	card[0] = (card[0] % 13);//Mods by 13, so Ace will be 0, 2 will be 1, etc.
	switch(card[0]){
	case 0:
		*aces = *aces + 1;
		cardValue = 11;
		break;
	case 10:
		cardValue = 10;  //Case 10 is a jack
		break;
	case 11:             //Case 11 is a queen
		cardValue = 10;
		break;
	case 12:            //Case 12 is a king
		cardValue = 10;
		break;
	default:            //Case 1-9 will be turned into 2-10 by adding 1.
		cardValue = card[0]+1;
		break;
	}
	return cardValue;
}
//////////////////////////////////
/*
    Function: changeAce
    purpose: After getting the card, to display a
    1 if the number is too large(and an ace) that it will go over 21.
    input: total, aceTotal.

    reasons: total to check if it goes over 21, aceTotal because ace was
    already added in cardDraw, so we must remove it.
*/
/////////////////////////////////
int changeAce(int *total, int *aceTotal){
	int newTotal = *total + 11;
	if(newTotal > 21){
		*aceTotal = *aceTotal - 1;
		return 1;
	}
	else
		return 11;
}

//////////////////////////////////
/*
    Function: checkTotal
    Purpose:If the total is too large, and you have an ace in your hand,
    this will continue the game without you busting. Otherwise, the state
    of the game changes to exit the loop.
    Inputs: total, acetotal.

    Reason: To update total if ace count is higher than 0 and to update
    acecount accordingly.
*/
/////////////////////////////////
int checkTotal(int *total, int *aceTotal){
	if(*total > 21){
		if(*aceTotal > 0){
			*(total) -= 10;
			*aceTotal=*aceTotal-1;
			printf("\nThere was an Ace, so the actual value is: %d", *total);
		}
		else{
			return -1;
		}
	}
	return 1;
}
//////////////////////////////////
/*
    Function: checkComp
    Purpose: About the same as Check Total except it adds
    a computer-check to it so that the computer will stop automatically
    if it is over 16.
    inputs: total, acetotal.

    Reason: To update total if ace count is higher than 0 and to update
    acecount accordingly.
*/
/////////////////////////////////
int checkComp(int *total, int *aceTotal){
	if(*total > 21){
		if(*aceTotal > 0){
			*(total) -= 10;
			*aceTotal=*aceTotal-1;
		}
		else{
			return -1;
		}
	}
	else if(*total >= 17){
		return 0;
	}
	return 1;
}

//////////////////////////////////
/*
    Function: playGame

    purpose: To play the game.
*/
/////////////////////////////////
void playGame(){
	cardDeck = fopen("/dev/cards", "r");
	int stateOfGame = 1; //A one means to keep playing the current player
	int compCurrCard, currCard;//==
	char input[10];
	int playerHandTotal = 0;
	int playerAceTotal = 0;
	int compHandTotal = 0;
	int compAceTotal = 0;
	compHandTotal = cardDraw(&compAceTotal); //Draws first 2 cards for computer.
	compCurrCard = cardDraw(&compAceTotal);
	playerHandTotal = cardDraw(&playerAceTotal); //Draws first card for player.

	while(stateOfGame == 1){
		printf("The dealer:\n? + %d\n\n", compCurrCard);
		currCard = cardDraw(&playerAceTotal);
		//Changes card to Ace if needed
		if(currCard == 11){
			currCard=changeAce(&playerHandTotal, &playerAceTotal);
		}
		//Checks the total here to update total.
		checkTotal(&playerHandTotal, &playerAceTotal);
		printf("You:\n%d + %d = %d ", playerHandTotal, currCard, playerHandTotal+currCard, playerAceTotal);
		playerHandTotal+=currCard;

		//Updates the state of the game.
		stateOfGame = checkTotal(&playerHandTotal, &playerAceTotal);
		if(stateOfGame == -1){
			printf("BUSTED!");
		}
		printf("\n\n");

		//Asks user to continue, 0 state means to leave.
		if(stateOfGame == 1){
			printf("Do you want to \"hit\" or \"stand\"?");
			scanf("%s", input);
			if(strcmp(input, "hit"))
				stateOfGame = 0;
		}

	}

	if(stateOfGame == -1){
		printf("You busted, dealer wins\n");
	}
    //Computers turn to go:
	else{
		printf("The dealer:\n%d + %d = %d\n\n",compHandTotal, compCurrCard, compHandTotal+compCurrCard);
		compHandTotal += compCurrCard;
		do{
			compCurrCard = cardDraw(&compAceTotal);
			if(compCurrCard == 11){
				compCurrCard=changeAce(&compHandTotal, &compAceTotal);
			}
			printf("The dealer:\n%d + %d = %d\n\n",compHandTotal, compCurrCard, compHandTotal+compCurrCard);
			compHandTotal += compCurrCard;
			stateOfGame = checkComp(&compHandTotal, &compAceTotal);
		}
		while(stateOfGame == 1);

		//If both the computer and human got to state 0, it means that both have valid numbers, check.
		if(stateOfGame == 0){
			(playerHandTotal > compHandTotal)? printf("You won, the dealer drew %d and you drew %d", compHandTotal, playerHandTotal):printf("You Lost, the dealer drew %d and you drew %d", compHandTotal, playerHandTotal);
		}
		//State of -1 for the computer, he failed.
		else{
		checkTotal(&playerHandTotal, &playerAceTotal);
			printf("Dealer Busted, you win!");
		}
	}
}
int main(){
	playGame();
	return 0;
}



