/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: olesya
 *
 * Created on April 16, 2017, 12:01 AM
 */

#include <cstdlib>
#include "tokenizer.h"

using namespace std;

/*
 * 
 */

struct generates {
    
};
sequence generate_words_template(){
    sequence new_sequence;
    uint16_t set = (1U << (TOKEN_ALPHA) |
            1U << (TOKEN_LATIN_ALPHA) |
            1U << (TOKEN_NUMERIC));
    
    new_sequence.push_back(set);
    return new_sequence;
}

sequence generate_emails_template() {
    sequence new_sequence;
    uint16_t set = 0;
    set = (1U << (TOKEN_LATIN_ALPHA) |
            1U << (TOKEN_NUMERIC) |
            1U << (TOKEN_DOT) |
            1U << (TOKEN_MINUS) |
            1U << (TOKEN_UNDERSCORE));
    new_sequence.push_back(set);
    set = 1U << (TOKEN_AT);
    new_sequence.push_back(set);
    set = 1U << (TOKEN_LATIN_ALPHA);
    new_sequence.push_back(set);
    set = 1U << (TOKEN_DOT);
    new_sequence.push_back(set);
    set = 1U << (TOKEN_LATIN_ALPHA);
    new_sequence.push_back(set);
    return new_sequence;
}

sequence (*p[2]) ();

int main() {
    sequence (*func_ptr[2])() = {generate_words_template, generate_emails_template};
    string test("hello! how are you? I'm fine, lala@yandex.ru привет хорошо yeeeee. blo123@gmail.com web_master@mail.ru");
    //
    tokenizer new_tokenizer(test.c_str(), test.size(), func_ptr, 2);
    while(!new_tokenizer.get_condition_iterate()) {
        new_tokenizer.process_next_letter();
        new_tokenizer.process_next_word();
    }
    string result;
    for (auto it : new_tokenizer.result_strs) {
        if (it.type == 0) {
            result = it.copy();
            cout << "word " << result << endl;
        }
        else if (it.type == 1) {
            result = it.copy();
            cout << "email " << result << endl;
        }
    }
   

    //cout << (5 & 7) << endl;
    return 0;
}

