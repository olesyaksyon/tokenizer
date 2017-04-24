/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tokenizer.h
 * Author: olesya
 *
 * Created on April 16, 2017, 12:03 AM
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <stdint.h>
#include <stack>
//#include "bitvector.h"
#include <time.h>

//#define 

using namespace std;

enum eArgToken
{
	TOKEN_ALPHA = 1
	, TOKEN_LATIN_ALPHA = 2
	, TOKEN_NUMERIC = 3
	, TOKEN_EMPTY = 4
	, TOKEN_DELIMITER = 5
	, TOKEN_MINUS = 6	// -
	, TOKEN_UNDERSCORE = 7	// _
	, TOKEN_DOT = 8			// .
	, TOKEN_PLUS = 9	// +
	, TOKEN_COLON = 10	// :
	, TOKEN_PAREN_L	= 11	// (
	, TOKEN_PAREN_R	= 12	// )
	, TOKEN_AT = 13		// @
	, NUM_TOKENS = 14
	, TOKEN_HIEROGLYPH = 15 //2E80 - 1F8FF
};

typedef vector <uint16_t> sequence;

struct word_pointer {
	const char* str;
	int size;
        int type;

	word_pointer(const char* str, int position, int size, int type) {
		if (size > 255)
			size = 255;
		this->str = str + position;
		this-> size = size;
                this->type = type;
	}

	word_pointer() {
		this->size = 0;
	}

	string copy(){ 
		string str_copied(str, size);
		return str_copied;
	}

	~word_pointer(){
	}
};

struct letter_descriptor {
	uint32_t offset;
	uint32_t length;
	uint32_t value;

	letter_descriptor() {
		this->offset = 0;
		this->length = 0;
	}
};

struct word_descriptor{
	uint32_t offset;
	int type;
	int length;
	int full_word_flag;
        sequence sequence_;
        vector <uint16_t> :: iterator curr_it;

	word_descriptor(sequence sequence_, int type) {
		this->type = type;
		this->full_word_flag = 0;
		this->offset = 0;
		this->length = 0;
                this->sequence_ = sequence_;
                curr_it = sequence_.begin();
	}
        
        bool token_in_set(eArgToken token, uint16_t set) {
            if (((1U << token) & set) != 0)
                return true;
            else
                return false;
        }
        
        void new_token(eArgToken token, letter_descriptor descriptor) {
            if (token_in_set(token, *curr_it))
            {
                if (full_word_flag == 1 || length == 0) {
                    this->offset = descriptor.offset;
                    this->full_word_flag = 0;
                    this->length = 0;
                }
                
                this->length += descriptor.length;
            }
              
            else if ((++ curr_it) != sequence_.end() && token_in_set(token, *curr_it) && length != 0) {
                this->length += descriptor.length;
                this->full_word_flag = 0;
            }
            
            else if ((this-> length != 0) && (full_word_flag != 1) && (curr_it == sequence_.end())) {
                this->curr_it = sequence_.begin();
                this->full_word_flag = 1;
                
            }
            
            else {
                this->curr_it = sequence_.begin();
                this->length = 0;
                this->full_word_flag = 0;
            }  
        }
};

class tokenizer {
public:
    letter_descriptor curr_symbl;
    vector <word_descriptor> words_containers;
    vector <word_pointer> result_strs;
    tokenizer(const char* begin, int string_size, sequence (*p[]) (), int size_of_arr);
    void process_next_letter();
    void process_next_word();
    bool get_condition_iterate();
    virtual ~tokenizer();
private:
    const char* begin;
    int string_size;
    bool condition_iterate;

};

#endif /* TOKENIZER_H */
