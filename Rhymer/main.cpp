//
//  main.cpp
//  Rhymer
//
//  Created by Charles Fei on 4/22/16.
//  Copyright © 2016 Charles Fei. All rights reserved.
//
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

#include <iostream>
#include <fstream>

using namespace std;

struct word
{
    string name;
    string lastVowel;
    vector<string> phonemes;
    vector<string> phonemesAfterLastVowel;
    int numMatchingPhonemes;
};

bool checkIfVowel(string phoneme);
void buildDict(string fileName, map<string, word> &dict, map<string, vector<word>> &lastVowelMap);
vector<word> findRhymes(string rhymeWord, map<string, word> &dict, map<string,
                        vector<word>> &lastVowelMap);
int findMatchingPhonemes(word w1, word w2);

//Check if phoneme is a vowel
bool checkIfVowel(string phoneme)
{
    if (phoneme[0] == 'A' || phoneme[0] == 'E' || phoneme[0] == 'I' ||
        phoneme[0] == 'O' || phoneme[0] == 'U')
    {
        return true;
    }
    
    return false;
}

//Build dictionary of rhymes
void buildDict(string fileName, map<string, word> &dict, map<string, vector<word>> &lastVowelMap)
{
    ifstream myFile(fileName);
    string line;
    
    if(myFile.is_open())
    {
        while (getline(myFile, line))
        {
            string myWord;
            stringstream ss(line);
            ss >> myWord;
            
            if(myWord != ";;;")
            {
                word dictWord;
                dictWord.name = myWord;
                
                string phoneme;
                bool afterVowel = false;
                while(ss >> phoneme)
                {
                    dictWord.phonemes.push_back(phoneme);
                    
                    if(afterVowel)
                    {
                        dictWord.phonemesAfterLastVowel.push_back(phoneme);
                    }
                    
                    if(checkIfVowel(phoneme))
                    {
                        afterVowel = true;
                        dictWord.lastVowel = phoneme;
                        dictWord.phonemesAfterLastVowel.clear();
                    }
                }
                lastVowelMap[dictWord.lastVowel].push_back(dictWord);
                dict[dictWord.name] = dictWord;
            }
        }
    }
}

//Find words that rhyme with given word
vector<word> findRhymes(string rhymeWord, map<string, word> &dict, map<string,
                        vector<word>> &lastVowelMap)
{
    vector<word> rhymeWords;
    
    if(dict.find(rhymeWord) != dict.end())
    {
        word myWord = dict[rhymeWord];
        for(word testWord : lastVowelMap[myWord.lastVowel])
        {
            if(testWord.name != myWord.name && testWord.phonemesAfterLastVowel == myWord.phonemesAfterLastVowel)
            {
                testWord.numMatchingPhonemes = findMatchingPhonemes(testWord, myWord);
                rhymeWords.push_back(testWord);
            }
        }
    }
    
    return rhymeWords;
}

//Find the number of matching phonemes between two words
int findMatchingPhonemes(word w1, word w2)
{
    int frequency = 0;
    map<string, int> frequencies;
    
    for(string phoneme : w1.phonemes)
    {
        frequencies[phoneme]++;
    }
    
    for(string phoneme2 : w2.phonemes)
    {
        if(frequencies[phoneme2] > 0)
        {
            frequencies[phoneme2]--;
            frequency++;
        }
    }
    
    return frequency;
}

int main(int argc, const char * argv[])
{
    map<string, word> dict;
    map<string, vector<word>> lastVowelMap;
    vector<word> rhymingWords;
    
    //Build dictionary of words
    buildDict("dict.txt", dict, lastVowelMap);
    
    //Take input
    string rhymeWord;
    cout << "Enter a word: ";
    cin >> rhymeWord;
    
    //Find words that rhyme with input word
    rhymingWords = findRhymes(rhymeWord, dict, lastVowelMap);
    
    //Sort rhyming words by number of matching phonemes
    sort(rhymingWords.begin(), rhymingWords.end(),
         [](const word &a, const word &b) -> bool
         {
             if(a.numMatchingPhonemes == b.numMatchingPhonemes)
             {
                 return a.name < b.name;
             }
             
             return a.numMatchingPhonemes > b.numMatchingPhonemes;
         });
    
    if(!rhymingWords.empty())
    {
        for(word rhymingWord : rhymingWords)
        {
            cout << rhymingWord.name << " " << rhymingWord.numMatchingPhonemes << endl;
        }
    }
    else
    {
        cout << "Could not find words that rhyme with your word. :(" << endl;
    }
    
    
    return 0;
}
