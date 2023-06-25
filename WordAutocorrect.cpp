#include<iostream>
#include<bits/stdc++.h>
using namespace std;

class TrieNode{

    public:

    char data;
    TrieNode** children;
    bool isTerminal;

    TrieNode(char data) {
        this -> data = data;
        children = new TrieNode* [26];
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        isTerminal = false;
    }

};

class Trie {

    public:

    TrieNode *root;

    Trie(){ 
        root = new TrieNode('\0'); 
    }

    void insertWordHelper(TrieNode*, string);

    void insertWord(string word){
        insertWordHelper(root, word);
    }
    
    bool searchWordHelper(TrieNode*, string);

    bool searchWord(string word){
        return searchWordHelper(root, word);
    }

};

void Trie::insertWordHelper(TrieNode *root, string word) {
    
    // Base Case
    if (word.size() == 0) {
        root->isTerminal = true;
        return;
    }

    int index = word[0] - 'a';     
    TrieNode *child;
    if (root->children[index] != NULL) {
        child = root->children[index];
    }
    else {
        child = new TrieNode(word[0]);
        root->children[index] = child;
    }

    // Recursive Call
    insertWordHelper(child, word.substr(1));

}

bool Trie :: searchWordHelper(TrieNode*root,string word){

    // Base Case
    if(!word.length())return root -> isTerminal;
    int index = word[0] - 'a';
    if(!root -> children[index])return false;

    // Recursive Call
    return searchWordHelper(root -> children[index],word.substr(1));

}

class AutoCorrect : public Trie{

    string input_word;

    unordered_map<string,int> results;
    
    public:

    void take_input();

    void check_word();

    void print_results();

};

// Take a word as input from user

void AutoCorrect :: take_input(){

    string valid_word = "";
    cout << "\nEnter A Word: ";
    cin >> input_word;

    // Convert each character to lower case English

    for(int i = 0 ; i<input_word.length() ; i++){
        valid_word += tolower(input_word[i]);
    }

    input_word = valid_word;
    cout << endl;

}

void AutoCorrect :: check_word(){

    // Word is already present in trie

    if(searchWord(input_word)){
        cout << "Word Is Spelled Correctly." << endl << endl;
    }

    // Word is not present in trie

    else{
        cout << "Word Is Spelled Incorrectly." << endl;

        // Check for a single incorrect character

        for(int i = 0 ; i<input_word.length(); i++){
            for(char ch = 'a';ch <= 'z'; ch++){
                char prev_char = input_word[i];
                input_word[i] = ch;
                if(searchWord(input_word))results[input_word]++;
                input_word[i] = prev_char;
            }
        }

        // Check for a single missing character

        for(int i = 0; i<input_word.length(); i++){
            for(char ch = 'a'; ch <= 'z'; ch++){
                string prev_word = input_word;
                input_word.insert(input_word.begin() + i, ch);
                if(searchWord(input_word))results[input_word]++;
                input_word = prev_word;
            }
        }

        // Check for a single extra character

        for(int i = 0; i<input_word.length(); i++){
            string prev_word = input_word;
            input_word.erase(input_word.begin() + i);
            if(searchWord(input_word))results[input_word]++;
            input_word = prev_word;
        }

        if(!results.empty()){
            print_results();
            return;
        }

        // Check for two incorrect characters

        for(int i = 0; i<input_word.length() - 1; i++){
            for(int j = i + 1; j<input_word.length(); j++){
                for(char ch1 = 'a';ch1 <= 'z'; ch1++){
                    for(char ch2 = 'a'; ch2 <= 'z'; ch2++){
                        char prev_i = input_word[i];
                        char prev_j = input_word[j];
                        input_word[i] = ch1;
                        input_word[j] = ch2;
                        if(searchWord(input_word))results[input_word]++;
                        input_word[i] = prev_i;
                        input_word[j] = prev_j;
                    }
                }
            }
        }

        print_results();

    }
}

void AutoCorrect :: print_results(){

    if(results.empty()){
        cout << "Could Not Find Correct Results." << endl;
        cout << "\nPossible Reason : Too many mis-spelled characters in input." << endl << endl;
        return;
    }

    cout << "\nAuto-Correct Results:" << endl << endl;
    for(auto it : results){
        cout << it.first << endl;
    }

    cout << "\nIf You Think Your Word Was Correctly Spelled and/or Results Were Inconsistent:" << endl;
    cout << "Press 1 To Add Word To Dictionary; 0 Otherwise." << endl << endl;
    cout << "Enter Choice: ";
    int choice;
    cin >> choice;
    if(!choice)return;
    cout << "Enter Correctly Spelled Word: ";
    cin >> input_word;
    string valid_word = "";
    for(int i = 0 ; i<input_word.length() ; i++){
        valid_word += tolower(input_word[i]);
    }
    fstream dict;
    dict.open("dictionary.txt", ios::app);
    if(dict.is_open()){
        dict << "\n" + input_word;
        cout << input_word << " Was Added." << endl;
    }
    else cout << "Failed To Open \"dictionary.txt\"" << endl;

}

int main(){

    fstream dict;
    dict.open("dictionary.txt", ios :: in | ios :: out);

    if(!dict.is_open()){
        cout << "Failed To Open \"dictionary.txt\"" << endl;
        return 0;
    }

    string new_word;
    AutoCorrect user;

    while(getline(dict, new_word)){
        user.insertWord(new_word);
    }

    user.take_input();
    user.check_word();

}