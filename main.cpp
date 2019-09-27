#include<iostream>       // for obvious reasons
#include<conio.h>        // for getch()
#include<windows.h>      // for color changing and Font size
#include<io.h>           // for _setmode function
#include<fcntl.h>        // to support the _setmode function in order to allow special symbols
#include<time.h>          // To use the clock as a random seed
#include<cstdlib>        // To get the random number generator function
#include<stdlib.h>       // for srand


// NOT using namespace std;

//COLORS LIST for font color
//1:  Blue
//2:  Green
//3:  Cyan
//4:  Red
//5:  Purple
//6:  Yellow (Dark)
//7:  Default white
//8:  Gray/Grey
//9:  Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors

// SYMBOLS LIST
// u2663 Clubs       ♣
// u2660 Spades      ♠
// u2665 Heart       ♥
// u2666 Diamond     ♦
//NOTE must write L in front of "text" for this to function. like this cout << (L"text");


// this creates a function Color(); that i can use to change font and backround color of text
void Color(WORD color)
{
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);   //out is used in the fontsize function down below


// calling all functions
void fontsize(int,int);
int getCard();

// Why all 52 cards you say? A: Well that is because I am going to Print the cards with symbol and Color!
// By separating the cards with 100, I am giving them a Unique ID. f.eks. 112 is Queen of Spades
int deck[52]{101,102,103,104,105,106,107,108,109,110,111,112,113,   // ACE to King in Spades    ♠
             201,202,203,204,205,206,207,208,209,210,211,212,213,   // ACE to King in hearts    ♥
             301,302,303,304,305,306,307,308,309,310,311,312,313,   // ACE to King in Diamonds  ♦
             401,402,403,404,405,406,407,408,409,410,411,412,413};  // ACE to King in Clubs     ♣

int tempDeck[52]{0};    // If a card is in play or discarded then its value is above 1000
int tempCount = 0;      // Thus the card will not be included in this deck, so that I do not Draw cards that are invalid
int cardsInTemp = 0;
int cardsInDeck = 0;



// PLAYER VALUES
int hand[10]{0};        // 11 cards is the max a player can hold to get 21 or more.
                        // I make this array so that I can cout the hand sideways using if statements
int cardsInHand = 0;    // I need this to print the cards sideways
int pVallet = 0;
int pCardV = 0;
int pHandVal = 0;


// DEALER VALUES
int dhand[10]{0};
int dcardsInHand = 0;
int dVallet = 0;
int dCardV = 0;
int dHandVal = 0;



// General values that are used everywere
int whatCard = 0;          // when returning card values I use this to decide what card to check
bool pMadeBet = false;              // To not display bet when first printing the table
int bet = 0;
int acesConverted = 0;
bool playing = true;
char playerInput;
bool firstIsDone = false;
bool playerWon;
bool tie = false;

void reShuffle()
{
    // When a card is discarded after a game ends, The card value in deck is the original value +2000
    // if the card is in play, its value is its original value +1000

    // So if i look for cards with a value above 2000, then it finds the cards that are not in play and are discarded
    // Then it takes those and adds them back into the deck.  " Reshuffle "

    // The cards are never "shuffled" i just deal a random card from the deck that has a value below 1000.
    // This is not the real world therfore I dont have to "shuffle" the deck.

    for(int i=0;i<=52;i++)
    {
        if(deck[i]>2000)
        {
            deck[i] -= 2000;
            cardsInDeck ++;
            if(cardsInDeck > 52)
            {
                std::wcout << "SOMETHING WENT WRONG ERROR CODE: CID52+";
                break;
            }
        }
    }

}

void printCardsPlayer()
{
    pHandVal = 0;
    // this function also gives the players hand a value based on what card it is

    int a = cardsInHand;

        std::wcout << "\t\t\t\t";
        for(int i=a;i>=1;i--)
        {
            std::wcout << " ___  ";
        }
        std::wcout << "\n";
        std::wcout << "\t\t\t\t";
        for(int i=0;i<a;i++)
        {
            if(hand[i]>100 && hand[i]<200) //Spades     L"\u2660"
            {
                if(hand[i]>101 && hand[i]<110)// 2-9
                {
                    std::wcout << "|" << hand[i]-100 << L"\u2660 | ";
                    pHandVal += hand[i]-100;
                }
                if(hand[i]==110)
                {
                    std::wcout << "|" << "10" << L"\u2660| ";
                    pHandVal += hand[i]-100;
                }
                if(hand[i]==101)
                {
                    std::wcout << "|" << "A" << L"\u2660 | ";
                    pHandVal += 1000;        // making the value really high so that i can use this to calculate 1 or 11 later
                }
                if(hand[i]==111)
                {
                    std::wcout << "|" << "J" << L"\u2660 | ";
                    pHandVal += hand[i]-101;
                }
                if(hand[i]==112)
                {
                    std::wcout << "|" << "Q" << L"\u2660 | ";
                    pHandVal += hand[i]-102;
                }
                if(hand[i]==113)
                {
                    std::wcout << "|" << "K" << L"\u2660 | ";
                    pHandVal += hand[i]-103;
                }
            }
            if(hand[i]>200 && hand[i]<300) //Hearts     L"\u2665"
            {
                if(hand[i]>201 && hand[i]<210)// 2-9
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << hand[i]-200;
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-200;
                }
                if(hand[i]==210)  // 10
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << hand[i]-200;
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << "| ";
                    pHandVal += hand[i]-200;
                }
                if(hand[i]==201)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "A";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += 1000;       // making the value really high so that i can use this to calculate 1 or 11 later
                }
                if(hand[i]==211)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "J";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-201;
                }
                if(hand[i]==212)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "Q";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-202;
                }
                if(hand[i]==213)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "K";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-203;
                }
            }
            if(hand[i]>300 && hand[i]<400) //Diamonds   L"\u2666"
            {
                if(hand[i]>301 && hand[i]<310)// 2-9
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << hand[i]-300;
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-300;
                }
                if(hand[i]==310)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "10";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << "| ";
                    pHandVal += hand[i]-300;
                }
                if(hand[i]==301)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "A";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += 1000;       // making the value really high so that i can use this to calculate 1 or 11 later
                }
                if(hand[i]==311)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "J";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-301;
                }
                if(hand[i]==312)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "Q";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-302;
                }
                if(hand[i]==313)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "K";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    pHandVal += hand[i]-303;
                }
            }
            if(hand[i]>400 && hand[i]<500) //Clubs      L"\u2663"
            {
                if(hand[i]>401 && hand[i]<410)// 2-9
                {
                    std::wcout << "|" << hand[i]-400 << L"\u2663 | ";
                    pHandVal += hand[i]-400;
                }
                if(hand[i]==410)
                {
                    std::wcout << "|" << "10" << L"\u2663| ";
                    pHandVal += hand[i]-400;
                }
                if(hand[i]==401)
                {
                    std::wcout << "|" << "A" << L"\u2663 | ";
                    pHandVal += 1000;           // making the value really high so that i can use this to calculate 1 or 11 later
                }
                if(hand[i]==411)
                {
                    std::wcout << "|" << "J" << L"\u2663 | ";
                    pHandVal += hand[i]-401;
                }
                if(hand[i]==412)
                {
                    std::wcout << "|" << "Q" << L"\u2663 | ";
                    pHandVal += hand[i]-402;
                }
                if(hand[i]==413)
                {
                    std::wcout << "|" << "K" << L"\u2663 | ";
                    pHandVal += hand[i]-403;
                }
            }
        }
        while(pHandVal > 999) // convert aces to 1 in value and let the program know you did it
        {
            pHandVal -=999;
            acesConverted ++;
        }
        while((pHandVal < 12) && acesConverted > 0)  // if the aces converted could be 11 and not make the value go beyond 21, then do it!
        {
            pHandVal +=10;
            acesConverted --;
        }
        if(pHandVal > 9)
        {
            std::wcout << "\n\t\t      " << "Value: " << pHandVal << " ";
        }
        if(pHandVal < 10)
        {
            std::wcout << "\n\t\t       " << "Value: " << pHandVal << " ";
        }

        for(int i=a;i>=1;i--)
        {
            std::wcout << "|___| ";
        }

    //  __   __
    // |a7| |a4|      CARDS WILL LOOK LIKE THIS
    // |__| |__|

}

void printCardsDealer()
{
    dHandVal = 0;
    // this function also gives the dealers hand a value based on what card it is


    int a = dcardsInHand;

        std::wcout << "\t\t\t\t";
        for(int i=a;i>=1;i--)
        {
            std::wcout << " ___  ";
        }
        std::wcout << "\n";
        std::wcout << "\t\t\t\t";
        for(int i=0;i<a;i++)
        {
            if((dhand[i]>100 && dhand[i]<200) || dhand[i]==0) //Spades     L"\u2660"
            {
                if(dhand[i]>101 && dhand[i]<110)// 2-9
                {
                    std::wcout << "|" << dhand[i]-100 << L"\u2660 | ";
                    dHandVal += dhand[i]-100;
                }
                if(dhand[i]==110)
                {
                    std::wcout << "|" << "10" << L"\u2660| ";
                    dHandVal += dhand[i]-100;
                }
                if(dhand[i]==101)
                {
                    std::wcout << "|" << "A" << L"\u2660 | ";
                    dHandVal += 1000;   // gives the hand a really high value so that I can calculate 1 or 11 later
                }
                if(dhand[i]==111)
                {
                    std::wcout << "|" << "J" << L"\u2660 | ";
                    dHandVal += dhand[i]-101;
                }
                if(dhand[i]==112)
                {
                    std::wcout << "|" << "Q" << L"\u2660 | ";
                    dHandVal += dhand[i]-102;
                }
                if(dhand[i]==113)
                {
                    std::wcout << "|" << "K" << L"\u2660 | ";
                    dHandVal += dhand[i]-103;
                }
                if(dhand[i]==0)
                {
                    std::wcout << "|" << " " << "  | ";
                }
            }
            if(dhand[i]>200 && dhand[i]<300) //Hearts     L"\u2665"
            {
                if(dhand[i]>201 && dhand[i]<210)// 2-9
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << dhand[i]-200;
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-200;
                }
                if(dhand[i]==210)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "10";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << "| ";
                    dHandVal += dhand[i]-200;
                }
                if(dhand[i]==201)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "A";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += 1000;   // gives the hand a really high value so that I can calculate 1 or 11 later
                }
                if(dhand[i]==211)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "J";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-201;
                }
                if(dhand[i]==212)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "Q";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-202;
                }
                if(dhand[i]==213)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "K";
                    std::wcout << L"\u2665";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-203;
                }
            }
            if(dhand[i]>300 && dhand[i]<400) //Diamonds   L"\u2666"
            {
                if(dhand[i]>301 && dhand[i]<310)// 2-9
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << dhand[i]-300;
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-300;
                }
                if(dhand[i]==310)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "10";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << "| ";
                    dHandVal += dhand[i]-300;
                }
                if(dhand[i]==301)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "A";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += 1000;   // gives the hand a really high value so that I can calculate 1 or 11 later
                }
                if(dhand[i]==311)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "J";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-301;
                }
                if(dhand[i]==312)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "Q";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-302;
                }
                if(dhand[i]==313)
                {
                    std::wcout << "|" ;
                    Color(12);
                    std::wcout << "K";
                    std::wcout << L"\u2666";
                    Color(7);
                    std::wcout << " | ";
                    dHandVal += dhand[i]-303;
                }
            }
            if(dhand[i]>400 && dhand[i]<500) //Clubs      L"\u2663"
            {
                if(dhand[i]>401 && dhand[i]<410)// 2-9
                {
                    std::wcout << "|" << dhand[i]-400 << L"\u2663 | ";
                    dHandVal += dhand[i]-400;
                }
                if(dhand[i]==410)
                {
                    std::wcout << "|" << "10" << L"\u2663| ";
                    dHandVal += dhand[i]-400;
                }
                if(dhand[i]==401)
                {
                    std::wcout << "|" << "A" << L"\u2663 | ";
                    dHandVal += 1000;   // gives the hand a really high value so that I can calculate 1 or 11 later
                }
                if(dhand[i]==411)
                {
                    std::wcout << "|" << "J" << L"\u2663 | ";
                    dHandVal += dhand[i]-401;
                }
                if(dhand[i]==412)
                {
                    std::wcout << "|" << "Q" << L"\u2663 | ";
                    dHandVal += dhand[i]-402;
                }
                if(dhand[i]==413)
                {
                    std::wcout << "|" << "K" << L"\u2663 | ";
                    dHandVal += dhand[i]-403;
                }
            }
        }
        std::wcout << "\n\t\t\t\t";
        for(int i=a;i>=1;i--)
        {
            std::wcout << "|___| ";
        }
        while(dHandVal > 999) // convert aces to 1 in value and let the program know you did it
        {
            dHandVal -=999;
            acesConverted ++;
        }
        while((dHandVal < 12) && acesConverted > 0)  // if the aces converted could be 11 and not make the value go beyond 21, then do it!
        {
            dHandVal +=10;
            acesConverted --;
        }

    //  __   __
    // |a7| |a4|      CARDS WILL LOOK LIKE THIS
    // |__| |__|
std::wcout << "\n";
        for(int i=dcardsInHand;i>0;i--)
        {
            std::wcout << " ";
        }
}

void drawTable()
{
    system("CLS");
    printCardsDealer(); // also calculates value of cards
    std::wcout << "\t\t\t     " << "DEALER HAND: " << dHandVal;

    std::wcout << "\n\nDealer has: ";
    Color(2);
    std::wcout << "$" << dVallet << "\n";
    Color(7);

    std::wcout << "\n\n\n\n\t\t\t\t\t\t\t\t\tCards in Deck: " << cardsInDeck << "\n\n\n\n\n";
    std::wcout << "You have: ";
    Color(2);
    std::wcout << "$" << pVallet << "\n";
    Color(7);

    if(pMadeBet)
    {
        Color(12);
        std::wcout << "You are Betting: $" << bet << "\n";
        Color(7);
    }
    else
    {
        std::wcout << "\n";
    }

    std::wcout << "Type H to draw a card\n"
               << "Type S to stand\n"
               << "Type P to Exit Game\n" ;
    printCardsPlayer(); // also calculates value of cards
    std::wcout << "\n";

}

int getCard()
{
    int extra = 5;
    tempCount = 0;
//    Sleep(1000);
    int b = 0;
    int c = 0;
    extra += 13;
    std::srand(time(nullptr)+extra);
    for(int i=0;i<cardsInDeck;i++)              // Find all the cards that are avaliable and throw them into a temp Deck.
    {
        if(deck[i]<1000)
        {
            tempDeck[tempCount] = deck[i];
            tempCount++;
        }
    }

    b = rand() % (tempCount);    // get a random number between 0 and the amount of cards in temp deck
    c = b;
    b = tempDeck[c];

    for(int i=0;i<52;i++)
    {
        if(deck[i] == b)     // Find that card in the deck and set its value to "in play" (value 1000)
        {
            deck[i] +=1000;

        }
    }

    for (int i=0;i<52;i++)  // then wipe the temp deck so that it can be reused later
    {
        tempDeck[i] = 0;
    }

    cardsInDeck --;   // the deck is now missing 1 more card
    return b; // return the value of the card
}

void firstSetup()
{
    if(firstIsDone == false)
    {
        pVallet = 100;
        dVallet = 100;
        cardsInDeck = 52;

        // give the dealer one card face up and one facing down
        dhand[0] = getCard();
        dhand[1] = 0; // give the dealer a blank card (a face down card) ((this card is later swapped))
        dcardsInHand = 2;  // Dealer only has 1 card but i set this value to 2 so that it will print a "face down" card as well
        hand[0] = getCard();
        hand[1] = getCard();
        cardsInHand = 2;
     }
}

void nextRound()
{
    for(int i=0;i<cardsInHand;i++) // "discard" the cards that are in play
    {
        for(int b=0;b<52;b++)
        {
            if(deck[i] == hand[i]+1000)     // Find that card in the deck and set its value to "Discarded" (value 2000)
            {
                deck[i] +=1000;
            }
        }
    }
    for (int i=0;i<dcardsInHand;i++)
    {
        for(int b=0;b<52;b++)
        {
            if(deck[i] == dhand[i]+1000)     // Find that card in the deck and set its value to "Discarded" (value 2000)
            {
                deck[i] +=1000;
            }
        }
    }
    if(cardsInDeck<4)
    {
        reShuffle();
    }
    dhand[0] = getCard();
    dhand[1] = 0; // give the dealer a blank card (a face down card) ((this card is later swapped))
    dcardsInHand = 2;  // Dealer only has 1 card but i set this value to 2 so that it will print a "face down" card as well
    hand[0] = getCard();
    hand[1] = getCard();
    cardsInHand = 2;
    if((playerWon == true) && tie==false)
    {
        pVallet += bet;
        dVallet -= bet;
    }
    else if ((playerWon == false) && tie==false)
    {
        pVallet -= bet;
        dVallet += bet;
    }
    else
    {

    }
    pMadeBet = false;

    if(pVallet<10)
    {
        system("CLS");
        Color(12);
        std::wcout << "\n\n\n\n\n\n\n\n\t\t\t\t You Have No More Money! \n\t\t\t\t      YOU LOOSE!\n\n\n\n\n\n";
        Color(7);
        playing = false;
        system("PAUSE");
        return;
    }
    if(dVallet<10)
    {
        system("CLS");
        Color(2);
        std::wcout << "\n\n\n\n\n\n\n\n\t\t\t\t Dealer Has No More Money! \n\t\t\t\t      YOU WIN!\n\n\n\n\n\n";
        Color(7);
        playing = false;
        system("PAUSE");
        return;
    }
    system("CLS");
    drawTable();
}

void StartGame()
{

    firstSetup();
    firstIsDone = true;  //failsafe

    drawTable();

    while(playing == true)
    {
        if(pMadeBet == false)
        {
            std::wcout << "Place your bet Min 10$: ";
            bet = 0;
            while((bet<10) && (bet <= pVallet) && (bet <= dVallet))
            {
                std::cin >> bet;
            }
            if((bet>=10) && (bet <= pVallet) && (bet <= dVallet))
            {
                pMadeBet = true;
                drawTable();
            }
        }

        std::cin >> playerInput;

        if((playerInput == 'h') || (playerInput == 'H'))
        {
            if(cardsInDeck<1)
            {
                reShuffle();
            }
            hand[cardsInHand] = getCard();
            cardsInHand++;
            drawTable();
        }
        else if((playerInput == 's') || (playerInput == 's'))
        {
            dhand[1] = getCard();
            drawTable();
            Sleep(400);
            while(dHandVal<16)
            {
                if(cardsInDeck<1)
                {
                    reShuffle();
                }
                dhand[dcardsInHand] = getCard();
                dcardsInHand++;
                drawTable();
                Sleep(400);
            }
            if(dHandVal>=16)
            {
                if((pHandVal > dHandVal) || dHandVal >21)
                {
                    playerWon = true;
                    tie = false;
                    std::wcout << "Player won!               ";
                    system("PAUSE");
                    nextRound();
                }
                else if (pHandVal < dHandVal)
                {
                    playerWon = false;
                    tie = false;
                    std::wcout << "Dealer won!               ";
                    system("PAUSE");
                    nextRound();
                }
                else if (pHandVal == dHandVal)
                {
                    tie = true;
                    std::wcout << "Tie, you get your money back!               ";
                    system("PAUSE");
                    nextRound();
                }
            }
        }
        else if((playerInput == 'P') || (playerInput == 'p'))
        {
            return;
        }
        else
        {
            std::cin >> playerInput;
        }
        if(pHandVal >21)
        {
            drawTable();
            std::wcout << "You are bust, Dealer wins!";
            system("PAUSE");
            playerWon = false;
            tie = false;
            nextRound();
            drawTable();
        }

    }
}


int main()
{


    _setmode(_fileno(stdout), _O_U16TEXT); // uses functions from io.h and fcntl.h libary to print symbols like ♠
    fontsize(30,40); // Makes the text big and readable.. DISCLAIMER I copied this code from internet, but I'm using it because it looks better!

    StartGame();




//        std::wcout << (rand() %10) + 1;

// USED FOR TESTING || USED FOR TESTING || USED FOR TESTING || USED FOR TESTING ||

//    hand[0] = 101;
//    hand[1] = 105;

//    hand[2] = 213;
//    hand[3] = 205;

//    hand[4] = 411;
//    hand[5] = 405;

//    hand[6] = 312;
//    hand[7] = 305;
//    cardsInHand = 8;
//    printCardsPlayer();

// USED FOR TESTING || USED FOR TESTING || USED FOR TESTING || USED FOR TESTING ||

 return 0;
}



// I will not take credit for knowing how this works, but it changes font size and it looks good, SO I'm keeping it...
void fontsize(int a, int b)
{
 PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
 lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
 GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
 lpConsoleCurrentFontEx->dwFontSize.X = a;
 lpConsoleCurrentFontEx->dwFontSize.Y = b;
 SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
