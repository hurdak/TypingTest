#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <vector>


int Randint(const int& min, const int& max) {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

std::string choose_word(int word_index, std::string text) {
    int numb_of_word = 0; // number of word, on which loop is located
    std::string result = ""; // word, which will returned

    for (int i{ 0 }; true; i++) { // loop, that search for start index of right word

        if (text[i] == ' ') { // by space come to next word (add 1 to numb_of_word)
            numb_of_word++;
            continue;
        }

        if (numb_of_word == word_index) { // if right word was found
            for (int j{ i }; text[j] != ' '; j++) { // go by letters to space
                result += text[j]; // add letter to result word
            }
            break; // out from loop (else loop work in vain
        }
    }

    return result;
}


int quantity_of_words(std::string text) { // function, that count spaces(' ') in string
    // in end of string must be space(' '), else will appears error
    int result = -1; // count of spaces(' '). it stars from negative one, cause in future it become index of word
    for (char i : text) {
        if (i == ' ') {
            result++;
        }
    }
    return result;
}

bool uncorrect_word(std::string typd, std::string gend) { // function, that check accuracy of word
    for (int i{ 0 }; i < gend.size() 
        && i < typd.size(); i++) { // checking for end of the shortest word

        if (typd[i] != gend[i]) { // checking similarity of chars
            return true;
        }
    }
    return false;
}


int main() {
    std::fstream file;
    file.open("resources.txt"); // open file with instances of words, which will used to type

    if (!file.is_open()) {
        std::cout << "File resources.txt was not found\nThe file must be in the same folder as the program";
        return 1; // check open state of file
    }

    std::string file_in_str; // variable of words from file
    std::getline(file, file_in_str); // get words from file
    

    std::cout << "Enter number of words: ";
    int words_n; // number of words, that will generate to type
    std::cin >> words_n; // get number of words, that will generate to type
    std::cout << std::endl;
    

    std::vector<std::string> gen_words; // generated words
    for (int i{ 0 }; i < words_n; i++) {
        gen_words.push_back(choose_word( // choose random word from file words
            Randint(0, quantity_of_words(file_in_str)), // random number for choose 
            file_in_str)); 
        std::cout << gen_words[i] << " ";
    }

    std::cout << std::endl << std::endl << "__press key to start test__: ";
    std::cin; // timer will start when someting is pressed
    auto start = std::chrono::high_resolution_clock::now(); // begin of time typing

    std::vector<std::string> typied_words; // typied words
    std::string temp_input = ""; // typied word, temp variable
    for (int i{ 0 }; i < words_n; i++) {
        std::cin >> temp_input; // get typied word
        typied_words.push_back(temp_input); // add typied word
    }
    
    auto end = std::chrono::high_resolution_clock::now(); // end of time typing

    int uncor_words = 0; // variable of number uncorrect words
    for (int i{ 0 }; i < words_n; i++) {
        if (uncorrect_word(typied_words[i], gen_words[i])) { // if function return true
            uncor_words++; // then add one to uncorrect words
        }
    }

    std::chrono::duration<double> duration = end - start; // solve duration of typing
    std::cout << std::endl << "time: " << duration.count() // print typing time
        << std::endl << "correct words: " << (words_n - uncor_words) // print number of correct words
        << std::endl << "WPM: " <<
        (words_n - uncor_words) / (duration.count() / 60); // print typing speed (words per minute)
}
