#include <iostream>
using namespace std;

void guessNumber() {
    int secretNumber = 3;
    int guess;

    do {
        cout << "Guess a number between 1 and 10. Or stop by giving -1." << endl;
        if (!(cin >> guess)) {
            cout << "Please enter only numbers." << endl;
            cin.clear();
            int remove_this_many_characters = 10000;
            char remove_until_this_character_is_found = '\n';
            cin.ignore(remove_this_many_characters, remove_until_this_character_is_found);
        } else if (guess == secretNumber) {
            cout << "That's correct!" << endl;
        } else if (guess != -1) {
            cout << "That's wrong." << endl;
        }
    } while (guess != -1);
}

int main() {
    guessNumber();
}