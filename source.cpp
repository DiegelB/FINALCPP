/*
Programmer: Ben Diegel
Program: Casino Blackjack
*/


#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<stdlib.h>
// TODO DELETE UNECCESARY DIRECTIVES TO HELP SPEED UP PROGRAM 
using namespace std;

// class to hold all the player info
class Player{
public:
    string name, hand;
    double funds = 100, betAmount;
    int score, wins, losses, cardTotal = 0;
};


//prototypes
string startScreen(fstream &userNameFile, fstream &passwordFile);
void mainMenu(Player P1, Player CPU);
void lineDraw(int x);
void singlePlayer(Player P1, Player CPU);
int deck(int x);
int bet(Player P1);
int playerChoice(vector<int>&P1Hand, Player P1);
void winning(Player P1, Player CPU, vector<int>P1Hand, vector<int>CPUHand);
void dealerAdd(vector<int>&CPUHand, Player CPU);

int main(){
    Player P1, P2, CPU; // declaring all players

    fstream userNameFile, passwordFile; // declaring files for user and pass

    userNameFile.open("usernames.txt", ios::app); // creates the critical files if not found
    passwordFile.open("passwords.txt", ios::app);

    P1.name = startScreen(userNameFile, passwordFile); // start up screen and gets the username
    mainMenu(P1, CPU); // if it passes the input val it goes to a second main

    userNameFile.close(); // closes files at the end
    passwordFile.close();
}

//start screen for login
string startScreen(fstream &userNameFile, fstream &passwordFile){  
    int userInput = 0;
    string userName, password;
    vector<string> userNames; // vectors so much easier
    vector<string> passwords;
    fstream readFile; // to read from username file (will add validation later)

    readFile.open("usernames.txt", ios::in);
    // todo put a way to populate the vectors with the usernams and passwords from the files
    // that will be used for username authentication (and password matching a[0] username == b[0] password)
    lineDraw(49); // neat lil box
    cout << "\n|\t\t\t\t\t\t|\n"
            "|\t\t(1)LOGIN\t\t\t|\n"
            "|\t\t(2)NEW PROFILE\t\t\t|\n"
            "|\t\t\t\t\t\t|\n";
    lineDraw(49);
    cout << endl;
    
    // todo add better input val and user prompts
    while(userInput != 1 || userInput != 2){
        cout << ">>";
        cin >> userInput;

        if (userInput == 2){
            cout << "Please enter a username.\n>>";
            cin >> userName;

            userNameFile << userName << endl;
            
            cout << "Please enter a password " << userName << ".\n>>";
            cin >> password;

            passwordFile << password << endl;
            return userName;
        }
        else if (userInput == 1){
            cout << "Please enter your username.\n>>";
            cin >> userName;

            return userName;
        }
    }   
}

// main menu of the game
void mainMenu(Player P1, Player CPU){
    char userInput;
    cout << "\t\t--Welcome to Blackjack CASINO!!!--\n\n";
    cout << "Player: " << P1.name << endl;
    cout << "1. Player VS CPU\n";
    cout << "2. Multiple Players VS CPU\n";
    cout << "3. Player Stats\n";
    cout << "4. Exit\n\n>>";

    // todo add input val
    cin >> userInput;
    if (userInput == '1'){
        singlePlayer(P1, CPU); //ONLY WORKING RN 1/2 IMPLEMENT
    }
    else if (userInput == '2'){
        //multiPlayer(P1);
    }
    else if (userInput == '3'){
        //stats(P1);
    }
    else if (userInput == '4'){
        exit(0);
    }
}

// single player logic
void singlePlayer(Player P1, Player CPU){
    int i;
    int card1, card2;
    vector<int>P1Hand; // vectors for hands
    vector<int>CPUHand;

    P1.betAmount = bet(P1); // get the bet amount before dealing

    // TODO MAKE THIS A LOOP SOMEHOW, WILL MAKE EVERYTHING EASIER
    //deal 2 cards each i tried a loop but kept getting perdictrable results 
    card1 = deck(15); // these numbers im passong are for a more random result
    card2 = deck(16);
    P1Hand.push_back(card1);
    P1Hand.push_back(card2);
    card1 = deck(12);
    card2 = deck(11);
    CPUHand.push_back(card1);
    CPUHand.push_back(card2);

    // gets the total of 2 cards (TODO MAKE THIS INTO A FUNCTION)
    for(i=0;i<P1Hand.size();i++){
        P1.cardTotal += P1Hand[i];
    }

    // TODO MAKE THIS INTO A LOOP OR LOOP AND FUNCTION 
    cout << "CPU top card: " << CPUHand[0] << endl;
    cout << "your cards: " << P1Hand[0] << " " << P1Hand[1] << endl;
    cout << "cards total " << P1.cardTotal << endl;
    cout << "your bet amount is " << P1.betAmount << endl;
    playerChoice(P1Hand, P1);
    P1.cardTotal = 0;
    for(i=0;i<P1Hand.size();i++){
        P1.cardTotal += P1Hand[i];
    }
    for(i=0;i<CPUHand.size();i++){
        CPU.cardTotal += CPUHand[i];
    }
    cout << "cards total " << P1.cardTotal << endl;
    dealerAdd(CPUHand, CPU);

    winning(P1, CPU, P1Hand, CPUHand);
}

// this is the deck, it will output a single random card
int deck(int x){
    const int DECK_SIZE = 10;
    int randomNum;
    int deck[DECK_SIZE] = {1,2,3,4,5,6,7,8,9,10}; // TODO MAKE SURE TO ADD FUNCTION FOR 1 (1 can be 1 or 11)
    
    srand(time(NULL)); // set the random seed to users time
    
    for(int i=0; i <x; i++){ // used to make it more random. ie generateing 10 (if x == 10) random numbers and using the 10th
        randomNum = rand() % 10 + 0;
    }
    return deck[randomNum]; 

}

// this is the player choice menu. can hit stand split double down or buy insurance
int playerChoice(vector<int>&P1Hand, Player P1){
    char userInput;
    int card;
    cout << "Would you like to (1)Hit, (2)Stand, TODO MAKE OTHER OPTIONS\n>>";
    cin >> userInput;
    // todo input val
    if(userInput == '1'){
        card = deck(10);
        cout << "you were dealt a " << card << endl;
        P1Hand.push_back(card);
        return 0;
    }
    else{
        return 0;
    }
}

// calculates the winner. TODO NEED TO ADD WIN LOSS RATIO, BET WINNING, AND REPLAY BUTTON
void winning(Player P1, Player CPU, vector<int>P1Hand, vector<int>CPUHand){
    int i;
    cout << "player final score " << P1.cardTotal << endl;
    cout << "CPU final score " << CPU.cardTotal << endl;

    if (P1.cardTotal > CPU.cardTotal && P1.cardTotal <= 21){
        cout << "You win! ";
    }
    else{
        cout << "You lose! ";
    }
}

// allows the player to bet and subtracts it from total funds 
int bet(Player P1){
    int amt;
    cout << "How much would you like to bet? (10, 20, 50, 100, 200?)\n"
         << "You have: " << P1.funds << "\n>>";
    cin >> amt;
    P1.funds = P1.funds - amt;
    return amt;

}

// dealer tries to hit a soft 17. not working just right so i need to fix
void dealerAdd(vector<int>&CPUHand, Player CPU){
    while(CPU.cardTotal < 17){
        int card = deck(10);
        CPUHand.push_back(card);
        CPU.cardTotal = CPU.cardTotal + card;
    }
}

// draws a neat line (used for boxes)
void lineDraw(int x){
    int i;
    for (i =0; i < x; i++){
        cout << "-";
    }
}