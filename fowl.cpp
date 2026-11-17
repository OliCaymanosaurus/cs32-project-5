#define _CRT_SECURE_NO_WARNINGS

#include "utilities.h"
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

const int MAXWORDS = 9000;                  //capacity of the word list (must be >= 8000)
const int MAXLINE = 100;                    //test lines are always fewer than 100 chars
const char WORDFILENAME[] = "C:/Users/olive/OneDrive/Documents/test.txt";    //path to the word file

//Returns true if s is 4 to 6 characters, all of them lower case letters.
bool isValidFormat(const char s[])
{
    int len = strlen(s);
    if (len < MINWORDLEN || len > MAXWORDLEN)
        return false;
    for (int k = 0; k < len; k++)
    {
        if (s[k] < 'a' || s[k] > 'z')
            return false;
    }
    return true;
}

//Returns true if s matches one of the first nWords words in the list.
bool isInWordList(const char words[][MAXWORDLEN + 1], int nWords, const char s[])
{
    for (int k = 0; k < nWords; k++)
    {
        if (strcmp(words[k], s) == 0)
            return true;
    }
    return false;
}

//Counts the ducks and chickens that test would earn against target.
void countDucksAndChickens(const char target[], const char test[],
    int& ducks, int& chickens)
{
    int tarLen = strlen(target);
    int tesLen = strlen(test);

    //Track which positions have already been paired up.
    bool targetUsed[MAXWORDLEN] = { false };
    bool testUsed[MAXWORDLEN] = { false };

    ducks = 0;
    chickens = 0;

    //Ducks first, since they take priority over chickens.
    for (int i = 0; i < tarLen && i < tesLen; i++)
    {
        if (target[i] == test[i])
        {
            ducks++;
            targetUsed[i] = true;
            testUsed[i] = true;
        }
    }

    //Then pair each remaining test letter with an unused matching target letter.
    for (int i = 0; i < tesLen; i++)
    {
        if (testUsed[i])
            continue;
        for (int j = 0; j < tarLen; j++)
        {
            if (!targetUsed[j] && target[j] == test[i])
            {
                chickens++;
                targetUsed[j] = true;
                testUsed[i] = true;
                break;
            }
        }
    }
}

//Plays one round using words[wordnum] as the target word.
//Returns the score (number of valid test words used), or -1 if the
//arguments are bad.
int handleOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
    if (nWords <= 0 || wordnum < 0 || wordnum >= nWords)
        return -1;

    const char* target = words[wordnum];
    int tries = 0;

    while(true)
    {
        char line[MAXLINE + 1];
        cout << "Test word: ";
        if (!cin.getline(line, MAXLINE + 1))
            return tries;   //input ended; nothing more we can do

        if (!isValidFormat(line))
        {
            cout << "Your test word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        if (!isInWordList(words, nWords, line))
        {
            cout << "I don't know that word." << endl;
            continue;
        }

        tries++;   //only valid, known words count
        if (strcmp(line, target) == 0)
            return tries;

        int ducks, chickens;
        countDucksAndChickens(target, line, ducks, chickens);
        cout << "Ducks: " << ducks << ", Chickens: " << chickens << endl;
    }
}

int main()
{
    char wordList[MAXWORDS][MAXWORDLEN + 1];
    int nWords = getWords(wordList, MAXWORDS, WORDFILENAME);

    if (nWords < 1)
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 1;
    }

    cout << "How many rounds do you want to play? ";
    int nRounds;
    cin >> nRounds;
    cin.ignore(10000, '\n');   //discard the rest of that line

    if (nRounds <= 0)
    {
        cout << "The number of rounds must be positive." << endl;
        return 1;
    }

    cout.setf(ios::fixed);
    cout << setprecision(2);

    int totalScore = 0;
    int minScore = 0;
    int maxScore = 0;

    for (int round = 1; round <= nRounds; round++)
    {
        cout << endl << "Round " << round << endl;

        int target = randInt(0, nWords - 1);
        cout << wordList[target] << endl;
        cout << "The target word is " << strlen(wordList[target])
            << " letters long." << endl;

        int score = handleOneRound(wordList, nWords, target);

        if (score == 1)
            cout << "You got it in 1 try." << endl;
        else
            cout << "You got it in " << score << " tries." << endl;

        //update the running statistics
        totalScore += score;
        if (round == 1 || score < minScore)
            minScore = score;
        if (round == 1 || score > maxScore)
            maxScore = score;

        cout << "Average: " << static_cast<double>(totalScore) / round
            << ", minimum: " << minScore
            << ", maximum: " << maxScore << endl;
    }
}
