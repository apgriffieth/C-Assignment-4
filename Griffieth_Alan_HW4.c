#include <stdio.h>
#include <stdlib.h>

typedef struct Card {
  char suit;
  int value;
}Card;

typedef struct BagNode {
 struct  Card* card;
 struct BagNode* next;
}BagNode;

typedef struct GrabBag {
  BagNode* head;
  BagNode* tail;
  int size;//keeps track of how many elements are in a bag
}GrabBag;

void initBag(GrabBag* bag) {
  bag->head = NULL;
  bag->tail = NULL;
  bag->size = 0;
}

BagNode* createNode(Card* card) {
  BagNode* newNode = (BagNode*)malloc(sizeof(BagNode));
  newNode->card = card;
  newNode->next = NULL;
  return(newNode);
}

void add(GrabBag* bag, Card* card) {
  BagNode* newNode = createNode(card);
  if(bag->head == NULL) {//The bag is empty
    bag->head = newNode;
    bag->tail = newNode;
    bag->size += 1;
  }
  else {//adds to tail
    bag->tail->next = newNode;
    bag->tail = bag->tail->next;
    bag->size += 1;
  }
}

int getCurrentSize(GrabBag* bag) {//this function is pretty useless if the size
                                  //is updated accurately
  return(bag->size); 
}

int isEmpty(GrabBag* bag) {
  if(bag->size  == 0) {
    return(1);
  }
  else {
    return(0);
  }
}

void empty(GrabBag* bag) {
  //this function empties the bag by deleting from the head until head is NULL
  BagNode* temp;
  while(bag->head != NULL) {
    temp = bag->head;
    bag->head = bag->head->next;
    free(temp);
  }
  bag->size = 0;
}

int getFrequencyOf(GrabBag* bag, int value) {
  BagNode* current = bag->head;
  int count = 0;
  int i;
  for(i = 0; i < bag->size; i++) {//This checks every value in the bag
    if(current->card->value == value) {
      count++;
    }
    current = current->next;
  }
  return(count);
}

void fillWithCards(GrabBag* bag) {
  if(!isEmpty(bag)) {
    empty(bag);//The bag should only have 52 cards
  }
  int i, j;
  for(i = 0; i < 4; i++) {
    for(j = 1; j <=13; j++) {//double for loop because we need 13 of each suit
      Card* newCard = (Card*)malloc(sizeof(Card));
      if(i == 0) {
	newCard->suit = 'C';//Clubs
      }
      else if(i == 1) {
	newCard->suit = 'D';//Diamonds
      }
      else if(i == 2) {
	newCard->suit = 'S';//Spades
      }
      else if(i == 3) {
	newCard->suit = 'H';//Hearts
      }
      newCard->value = j;
      add(bag, newCard);
    }
  }
  bag->size = 52;
}

Card* grab(GrabBag* bag) {
  if(isEmpty(bag)) {
    printf("The bag is empty!\n");
    return(NULL);
  }
  else {
    Card* card;
    int size = bag->size;
    int remove = (rand() % (size));//gives us a random number from 0 to size-1
    BagNode* current = bag->head;
    BagNode* previous;
    int i;
    for(i = 0; i < remove; i++) {//iterates through the random number 
      previous = current;        //of elements
      current = current->next;
    }
      card = current->card;
      previous->next = current->next;//gets rid of the element
      bag->size--;
    return(card);
  }
}
 
void printBag(GrabBag* bag) {
  BagNode* node;
  if(bag->head == NULL) {
    printf("The deck is empty\n");
  }
  else {
    node = bag->head;
    while(node != NULL) {
      if(node->card->suit == 'C') {
	printf("[%d-C]\t", node->card->value);
      }
      else if(node->card->suit == 'D') {
	printf("[%d-D]\t", node->card->value);
      }
      else if(node->card->suit == 'S') {
	printf("[%d-S]\t", node->card->value);
      }
      else if(node->card->suit == 'H') {
	printf("[%d-H]\t", node->card->value);
      }
      node = node->next;
    }
    printf("\n");
  }
}

void printCard(Card* card) {//prints just a single card
 if(card->suit == 'C') {
   printf("[%d-C]\n\n", card->value);
 }
 else if(card->suit == 'D') {
   printf("[%d-D]\n\n", card->value);
 }
 else if(card->suit == 'S') {
   printf("[%d-S]\n\n", card->value);
 }
 else if(card->suit == 'H') {
   printf("[%d-H]\n\n", card->value);
 }
}

void removeCard(GrabBag* bag, Card* card) {//removes a specified card
  BagNode* current = bag->head;
  BagNode* previous;

  while(current != NULL && current->card != card) {
    previous = current;//goes through the list until current is at the card we 
    current = current->next;//want
  }

  if(current != NULL) {
    if(previous != NULL) {//we're in the middle of the list
      previous->next = current->next;
    }
    else {//we're at the head of the list
      bag->head = bag->head->next;
    }

    if(bag->tail == current) {//we're at the tail of the list
      bag->tail = previous;
    }
    free(current->card);
    free(current);
  }
  bag->size--;
}

int main(int argc, char** argv) {
  GrabBag* bag;
  bag = (GrabBag*)malloc(sizeof(GrabBag));
  initBag(bag);
  fillWithCards(bag);
  Card* card = grab(bag);
  int i, choice, guess, points = 0;
  int previous = card->value;
  double size;
  double probabilityLower = 0, probabilityHigher = 0, count1 = 0, count2 = 0;
  double probabilitySame, singleProbability;
  GrabBag* drawnBag = (GrabBag*)malloc(sizeof(GrabBag));
  initBag(drawnBag);
  add(drawnBag, card);
  printf("\nWelcome to the Hi-Low-Guess Game!\n");
  do {
    if(drawnBag->size == 34) {//if 2/3 of the deck has been drawn, it must be 
      printf("Getting a new deck!\n");//refilled
      empty(drawnBag);
      empty(bag);
      fillWithCards(bag);
      add(drawnBag, card);
      removeCard(bag, card);
      printf("We now have a new deck!\n");
    }

    count1 = 0;
    count2 = 0;
    size = bag->size;
    for(i = 1; i<card->value; i++) {
      count1 += getFrequencyOf(bag, i);
    }//this is done in the do loop because the probabilities must be
    probabilityLower = count1/size;//recalculated every time
    
    for(i = card->value + 1; i <= 13; i++) {
      count2 += getFrequencyOf(bag, i);
    }//this is done in the do loop for the same reason
    probabilityHigher = count2/size;
    probabilitySame = 1 - probabilityHigher - probabilityLower;

    printf("Cards left in deck: %d\n", bag->size);
    printf("All drawn cards are: ");
    printBag(drawnBag);//shows the user all previously drawn cards
    printf("The current card is: ");
    printCard(card);//shows the user the currently picked card to guess against
    printf("The probability of drawing a lower card is: %.3f\n", probabilityLower);
    printf("The probability of drawing a higher card is: %.3f\n", probabilityHigher);//shows the probabilities of picking a higher or lower card
    printf("The probability of drawing the same value card is: %.3f\n", probabilitySame);
    singleProbability = getFrequencyOf(bag, 1)/size;//each of these show the 
    printf("The probability of the next card being:\n");//probability of 
    printf("1\t%.3f\n", singleProbability);         //picking a specific card
    singleProbability = getFrequencyOf(bag, 2)/size;
    printf("2\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 3)/size;
    printf("3\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 4)/size;
    printf("4\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 5)/size;
    printf("5\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 6)/size;
    printf("6\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 7)/size;
    printf("7\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 8)/size;
    printf("8\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 9)/size;
    printf("9\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 10)/size;
    printf("10\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 11)/size;
    printf("11\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 12)/size;
    printf("12\t%.3f\n", singleProbability);
    singleProbability = getFrequencyOf(bag, 13)/size;
    printf("13\t%.3f\n", singleProbability);

    printf("Your current score is: %d\n\n", points);//shows current score
    printf("Press 1 if you think the next card drawn will be lower!\n");
    printf("Press 2 if you think the next card drawn will be higher!\n");
    printf("Press 3 if you would like to guess the next card drawn!\n");
    printf("Press 4 to exit!\n");
    printf("Choose an option: ");

    scanf("%d", &choice);

    switch(choice) {
    case 1 :
      card = grab(bag);
      printf("\n\nYou guessed the next card will be lower!\n");
      printf("The card drawn was: ");
      printCard(card);
      add(drawnBag, card);//this is done in the cases since it starts outside
                          //the do loop
      if(card->value < previous) {
	printf("You were correct! +1 point!\n\n");
	points++;
	previous = card->value;//adjusts what the menu will show the previously
      }                        //drawn card was
      else if(card->value == previous) {//not higher or lower
	printf("Equal value, +0 points!\n\n");
	previous = card->value;
      }
      else {//the card was higher when you guessed lower
	printf("Sorry you have lost.\n");
	printf("Your score was: %d\n", points);
	printf("Thanks for playing!\n");
	choice = 4;
      }
      break;

    case 2 :
      card = grab(bag);
      printf("\n\nYou guessed the next card will be higher!\n");
      printf("The card drawn was: ");
      printCard(card);
      add(drawnBag, card);

      if(card->value > previous) {
	printf("You were correct! +1 point!\n\n");
	points++;
	previous = card->value;
      }
      else if(card->value == previous) {//the card is not higher or lower
	printf("Equal value, +0 points!\n\n");
	previous = card->value;
      }
      else {//you guessed higher when the card was lower
	printf("Sorry you have lost.\n");
	printf("Your score was: %d\n", points);
	printf("Thanks for playing!\n");
	choice = 4;
      }
      break;

    case 3 :
      card = grab(bag);
      printf("\nYou have chosen to guess the next card!\n");
      printf("What value do you think the next card will be (1-13)?\n");

      scanf("%d", &guess);
      printf("The card drawn was: ");
      printCard(card);

      if(guess == card->value) {
	printf("You correctly guessed the card! +5 points!\n\n");
	points += 5;
	previous = card->value;
	add(drawnBag, card);
      }

      else {//you guessed what the next card would be incorrectly
	printf("Sorry, that is incorrect. You have lost.\n");
	printf("Your score was: %d\n", points);
	printf("Thanks for playing!\n");
	choice = 4;
      }
      break;

    case 4 :
      printf("Goodbye!\n");
      break;

    default :
      printf("Incorrect input, please try again!\n\n");
      break;
    }
  }
  while(choice != 4);
  return(0);
}
