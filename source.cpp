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
    string name = "user", hand;
    double funds = 100, betAmount;
    int score, wins = 0, losses = 0, cardTotal = 0;
};


//prototypes
string startScreen(fstream &userNameFile, fstream &passwordFile);
void mainMenu(Player &P1, Player &CPU, fstream &userNameFile, fstream &passwordFile, fstream &statsFile);
void lineDraw(int x);
void singlePlayer(Player &P1, Player &CPU);
int deck(int x);
int bet(Player &P1);
bool playerChoice(vector<int>&P1Hand, Player &P1);
void winning(Player &P1, int CPU, vector<int>P1Hand, vector<int>CPUHand);
int dealerAdd(vector<int>&CPUHand, Player CPU);
int handTotal(Player Play1, vector<int>PlayHand);
void singlePlayerUpdate(Player &P1, Player &CPU, vector<int>&P1Hand, vector<int>&CPUHand);
int aceCheck(vector<int>&P1Hand);
void displayStats(Player P1);

int main(){
    Player P1, P2, CPU; // declaring all players

    fstream userNameFile, passwordFile; // declaring files for user and pass
    fstream statsFile;

    userNameFile.open("usernames.txt", ios::app | ios::in); // creates the critical files if not found
    passwordFile.open("passwords.txt", ios::app | ios::in);

    P1.name = startScreen(userNameFile, passwordFile); // start up screen and gets the username

    string statsFileName = P1.name + "_statsFile.txt"; // creates the stats file for each user
    statsFile.open(statsFileName, ios::app | ios::in);
    if (statsFile){ // if its not empty it enters the stats into the player variables
        statsFile >> P1.wins;
        statsFile >> P1.losses;
        statsFile >> P1.funds;
    }

    mainMenu(P1, CPU, userNameFile, passwordFile, statsFile); // if it passes the input val it goes to a second main

    userNameFile.close(); // closes files at the end
    passwordFile.close();
}

//start screen for login
string startScreen(fstream &userNameFile, fstream &passwordFile){  
    int userInput = 0, i = 0;
    string userName, password, fileUser, filePass, temp;
    vector<string> userNames; // vectors so much easier
    vector<string> passwords;

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

        if (userInput == 2){ // creates a new user and enters it into the username/password files
            cout << "Please enter a username.\n>>";
            cin >> userName;
            userNameFile << userName << endl;
            
            cout << "Please enter a password " << userName << ".\n>>";
            cin >> password;

            passwordFile << password << endl;
            return userName;
        }
        else if (userInput == 1){
            userNameFile >> temp;
            while(userNameFile){ // populates a vector with all the known usernames
                userNames.push_back(temp);
                userNameFile >> temp;
            }
            passwordFile >> temp;
            while(passwordFile){ // same with passwords
                passwords.push_back(temp);
                passwordFile >> temp;
            }

            cout << "Please enter your username.\n>>";
            cin >> userName;
            for(i=0;i<userNames.size();i++){ // checks to see if the entered user name is within its database
                if (userName == userNames[i]){
                    cout << "Please enter your password " << userNames[i] << "\n>>"; 
                    cin >> password;

                    if (password == passwords[i]){ // compares to its corrisponding password
                        return userName;
                    }
                    else {
                        cout << "Incorrect password" << endl;
                        main();
                    }
                }
                else{
                    continue;
                }
            }
            cout << "No user with that name." << endl;
            main();
        } 
    }
}

// main menu of the game
void mainMenu(Player &P1, Player &CPU, fstream &userNameFile, fstream &passwordFile, fstream &statsFile){
    while(true){
        char userInput;
        system("cls");
        cout << "\t\t--Welcome to Blackjack CASINO!!!--\n\n";
        cout << "Player: " << P1.name << endl;
        cout << "1. Player VS CPU\n";
        cout << "2. Multiple Players VS CPU\n";
        cout << "3. Player Stats\n";
        cout << "4. Exit\n\n>>";
    
        // todo add input val
        cin >> userInput;
        if (userInput == '1'){
            singlePlayer(P1, CPU); 
        }
        else if (userInput == '2'){ // couldnt finish this Dx
            //multiPlayer(P1);
        }
        else if (userInput == '3'){
            displayStats(P1);
        }
        else if (userInput == '4'){
            statsFile << P1.wins << endl << P1.losses << endl << P1.funds << endl;
            userNameFile.close(); // closes files at the end
            passwordFile.close();
            statsFile.close();
            exit(0);
        }
    }

}
void displayStats(Player P1){ // displays the player stats
    string a;
    cout << "\nWins: "<< P1.wins << endl;
    cout << "Losses: "<< P1.losses << endl;
    cout << "Funds: " << P1.funds << endl;
    cout << "--Press enter to continue back to menu--\n";
    getline(cin, a);
    cin.ignore(); 
}

// single player logic
void singlePlayer(Player &P1, Player &CPU){
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

    char aceChoice;

    // checks to see if the dealt cards have an ace
    aceCheck(P1Hand);

    // gets the total of 2 cards (TODO MAKE THIS INTO A FUNCTION)
    P1.cardTotal = handTotal(P1, P1Hand);

    // TODO MAKE THIS INTO A LOOP OR LOOP AND FUNCTION 
    singlePlayerUpdate(P1, CPU, P1Hand, CPUHand);


}

// this function is used as the loop within the single player game
void singlePlayerUpdate(Player &P1, Player &CPU, vector<int>&P1Hand, vector<int>&CPUHand){
    bool flag = false;
    int cpuFinalTotal;
    while(!flag){ 
        system("cls");
        cout << "\n\nCPU top card: " << CPUHand[0] << endl;
        cout << "Your bet amount is " << P1.betAmount << endl << endl;
        cout << "--Your cards--" << endl;
        for(int i=0; i<P1Hand.size();i++){
            cout << "Card " << i+1 << ": " << P1Hand[i] << endl; // lists all your current cards
        }
        cout << "Cards total = " << P1.cardTotal << endl << endl;
        flag = playerChoice(P1Hand, P1);
        P1.cardTotal = handTotal(P1, P1Hand);    
        CPU.cardTotal = handTotal(CPU, CPUHand);
    }
    cpuFinalTotal = dealerAdd(CPUHand, CPU);
    winning(P1, cpuFinalTotal, P1Hand, CPUHand);
}

// checks to see if there is an ace, and if there is asks the user how it wants to play it
int aceCheck(vector<int>&P1Hand){
    char aceChoice;
    int i;
    for (i=0; i < P1Hand.size(); i++){
        if (P1Hand[i] == 1){
            cout << "\nYou were dealt and ACE, would you like it to be counted as a (1)one or (2)eleven?\n";
            cout << "--Your current cards are--\n";
            for(int r=0; r < P1Hand.size(); r++){
                cout << "Card " << r+1 << ": "<< P1Hand[r] << endl;
            }
            cout << "\n>>";
            while(true){
                cin >> aceChoice;
                if (aceChoice == '1'){
                    P1Hand[i] = 1;
                    return 0;
                }
                else if (aceChoice == '2') {
                    P1Hand[i] = 11;
                    return 0;
                }
                else {
                    cout << "Please only enter the number 1 (1) or 2 (11).\n>>";
                    continue;
                }
            }
        }
    }

}

// totals up the cards in the players hand
int handTotal(Player Play1, vector<int>PlayHand){
    int i;
    Play1.cardTotal = 0;
    for(i=0;i<PlayHand.size();i++){
        Play1.cardTotal += PlayHand[i];
    }
    return Play1.cardTotal;
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
bool playerChoice(vector<int>&P1Hand, Player &P1){
    char userInput, aceChoice;
    int card;
    cout << "Would you like to (1)Hit, (2)Stand, (3)2x-Down\n>>";
    cin >> userInput;
    // todo input val
    if(userInput == '1'){
        card = deck(10);

        // had to make another ace check custom to the card choice
        if (card == 1){
            cout << "You were dealt and ACE, would you like it to be counted as a (1)one or (2)eleven?\n";
            cout << "--Your current cards are--\n";
            for(int r=0; r < P1Hand.size(); r++){
                cout << "Card " << r+1 << ": "<< P1Hand[r] << endl;
            }
            cout << ">>";
            while(true){
                cin >> aceChoice;
                if (aceChoice == '1'){
                    card = 1;
                    break;
                }
                else if (aceChoice == '2') {
                    card = 11;
                    break;
                }
                else {
                    cout << "Please only enter the number 1 (1) or 2 (11).\n>>";
                    continue;
                }
            }
        }

        lineDraw(15);
        cout << "You were dealt a " << card;
        lineDraw(15);
        P1Hand.push_back(card);
        cout << "\n\n--Press enter to continue--\n>>";
        string a;
        getline(cin, a);
        cin.ignore(); 
        return false;
    }
    else if (userInput == '2'){
        return true;
    }
    else if (userInput == '3'){ // this is for doubling down your bet
        P1.betAmount = P1.betAmount * 2;
        cout << "You multiplied your bet to " << P1.betAmount << " and have to stand.";
        cout << "\n\n--Press enter to continue--\n>>";
        string a;
        getline(cin, a);
        cin.ignore();
        return true;
    }
}

// calculates the winner. TODO NEED TO ADD WIN LOSS RATIO, BET WINNING, AND REPLAY BUTTON
void winning(Player &P1, int CPU, vector<int>P1Hand, vector<int>CPUHand){
    int i;
    string a;
    system("cls");
    cout << "player final score " << P1.cardTotal << endl;
    cout << "CPU final score " << CPU << endl;

    // winning calculation
    if (P1.cardTotal > 21){
        cout << "You lose!\n\n";
        P1.losses += 1;
        P1.funds -= P1.betAmount;
    }
    else if (P1.cardTotal == 21){
        cout << "You win!\n\n";
        P1.wins += 1;
        double earnings;
        earnings = P1.betAmount * 2;
        cout << "You've won $" << earnings << "!\n";
        P1.funds += earnings;
    }
    else if (P1.cardTotal > CPU && P1.cardTotal <= 21){
        cout << "You win!\n\n";
        P1.wins += 1;
        double earnings;
        earnings = P1.betAmount + (.5 * P1.betAmount);
        cout << "You've won $" << earnings << "!\n";
        P1.funds += earnings;
    }
    else{
        cout << "You lose!\n\n";
        P1.losses += 1;
        P1.funds -= P1.betAmount;
    }
    cout << "--Press enter to continue back to menu--\n";
    getline(cin, a);
    cin.ignore(); 
}

// allows the player to bet and subtracts it from total funds 
int bet(Player &P1){
    int amt;
        cout << "How much would you like to bet? (10, 20, 50, 100, 200?)\n"
        << "You have: " << P1.funds << "\n>>";
        cin >> amt;
        if (amt > P1.funds){
            cout << "You do not have enough money for that!\n";
            continue;;
        }
        else{
            break;
        }
    }
    return amt;

}

// dealer tries to hit a soft 17. not working just right so i need to fix
int dealerAdd(vector<int>&CPUHand, Player CPU){
    int card;
    while(CPU.cardTotal < 17){
        card = deck(10);
        CPUHand.push_back(card);
        CPU.cardTotal = CPU.cardTotal + card;
    }
    return CPU.cardTotal;
}

// draws a neat line (used for boxes)
void lineDraw(int x){
    int i;
    for (i =0; i < x; i++){
        cout << "-";
    }
}