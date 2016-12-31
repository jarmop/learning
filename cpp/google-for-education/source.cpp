#include <iostream>
#include <cstdlib>

using namespace std;

void GuessNumber() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;
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
            break;
        } else if (guess != -1) {
            if (guess < secretNumber) {
                cout << "That's too low." << endl;
            } else {
                cout << "That's too high." << endl;
            }

        }
    } while (guess != -1);
}

void greatestCommonDivisor() {
    int number1 = -4;
    int number2 = 14;
    int remainder;
    while (number1 != 0) {
        remainder = number2 % number1;
        number2 = number1;
        number1 = remainder;
    }

    cout << number2 << endl;
}

int main() {
//    GuessNumber();
    greatestCommonDivisor();
}