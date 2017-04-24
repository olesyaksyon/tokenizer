/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tokenizer.cpp
 * Author: olesya
 * 
 * Created on April 16, 2017, 12:03 AM
 */

#include "tokenizer.h"



tokenizer::tokenizer(const char* begin, int string_size, sequence (*p[]) (), int size_of_arr) //constructor
{
	this->begin = begin;
	this->string_size = string_size;
	this->condition_iterate = 0;
        for (int i = 0; i < size_of_arr; i ++) {
            sequence new_sequence = (*p[i]) ();
            word_descriptor descriptor(new_sequence, i);
            words_containers.push_back(descriptor);
        }
}

uint8_t mask8(char oc) //unicode mask
    {
        return static_cast<uint8_t>(0xff & oc);
    }

int sequence_length(uint8_t lead_it) //detect the length of unicode symbol
    {
        uint8_t lead = mask8(lead_it);
        if (lead < 0x80)
            return 1;
        else if ((lead >> 5) == 0x6)
            return 2;
        else if ((lead >> 4) == 0xe)
            return 3;
        else if ((lead >> 3) == 0x1e)
            return 4;
        else
            return 1;
    }


eArgToken DetectToken(uint32_t codepoint) //type of token: letter, number etc
{	
	if( (codepoint >= 'a') && (codepoint <= 'z') )
		return TOKEN_LATIN_ALPHA;
	if( (codepoint >= 'A') && (codepoint <= 'Z') )
		return TOKEN_LATIN_ALPHA;
	if( (codepoint >= '0') && (codepoint <= '9') )
		return TOKEN_NUMERIC;
	if(codepoint == '-')
		return TOKEN_MINUS;
	if(codepoint == '_')
		return TOKEN_UNDERSCORE;
	if(codepoint == '.')
		return TOKEN_DOT;
	if(codepoint == '+')
		return TOKEN_PLUS;
	if(codepoint == ':')
		return TOKEN_COLON;
	if(codepoint == '(')
		return TOKEN_PAREN_L;
	if(codepoint == ')')
		return TOKEN_PAREN_R;
	if(codepoint == '@')
		return TOKEN_AT;
	if ((codepoint >= 0x00) && (codepoint <= 0x2F)) //���������, ����� ������ 
		return TOKEN_DELIMITER;
        //if ((codepoint == '\\n') or (codepoint == '\\0'))
        //    return TOKEN_DELIMITER;
	if(codepoint <= 0x7F) //C0 controls and ASCII punctuation and symbols
		return TOKEN_DELIMITER;
	if(codepoint <= 0xBF) //C1 controls Latin-1 punctuation and symbols
		return TOKEN_DELIMITER;
	if((codepoint == 0xD7) || (codepoint == 0xF7)) //Multiplication Sign & Division Sign
		return TOKEN_DELIMITER;
	if((codepoint >= 0x2000) && (codepoint <= 0x27FF))
	{
		//2000 General Punctuation 206F
		//2070 Superscripts and Subscripts 209F
		//20A0 Currency Symbols 20CF
		//20D0 Combining Diacritical Marks for Symbols 20FF
		//2100 Letterlike Symbols 214F
		//2150 Number Forms 218F
		//2190 Arrows 21FF
		//2200 Mathematical Operators 22FF
		//2300 Miscellaneous Technical 23FF
		//2400 Control Pictures 243F
		//2440 Optical Character Recognition 245F
		//2460 Enclosed Alphanumerics 24FF
		//2500 Box Drawing 257F
		//2580 Block Elements 259F
		//25A0 Geometric Shapes 25FF
		//2600 Miscellaneous Symbols 26FF
		//2700 Dingbats 27BF
		//27C0 Miscellaneous Mathematical Symbols-A 27EF
		//27F0 Supplemental Arrows-A 27FF
		return TOKEN_DELIMITER;		
	}
		//2800 Braille Patterns 28FF
	if((codepoint >= 0x2900) && (codepoint <= 0x2BFF))

	{
		//2900 Supplemental Arrows-B 297F
		//2980 Miscellaneous Mathematical Symbols-B 29FF
		//2A00 Supplemental Mathematical Operators 2AFF
		//2B00 Miscellaneous Symbols and Arrows 2BFF
		return TOKEN_DELIMITER;
	}
	
	//if ((codepoint >= 0x2E80) && (codepoint <= 0x1F8FF))
	//	return TOKEN_HIEROGLYPH;

	return TOKEN_ALPHA;
}

uint32_t get_unicode(const char* curr, int length){ //return the value of unicode in uint_32
    uint32_t cp = mask8(*curr);
    
    switch (length) {
		case 1:
			break;

		case 2:
			curr++;
			cp = ((cp << 6) & 0x7ff) + ((*curr) & 0x3f);
			break;

		case 3:
			++curr;
			cp = ((cp << 12) & 0xffff) + ((mask8(*curr) << 6) & 0xfff);
			cp += (*curr) & 0x3f;
			break;

		case 4:
			++curr;
			cp = ((cp << 18) & 0x1fffff) + ((mask8(*curr) << 12) & 0x3ffff);                
			++curr;
			cp += (mask8(*curr) << 6) & 0xfff;
			++curr;
			cp += (*curr) & 0x3f;
                        break;
            }
    return cp;
}

void tokenizer :: process_next_letter (){ //get info about curr_symbol
    const char* curr;
    
    if (this->curr_symbl.length == 0) {
		curr = begin;
	}
    
    else {
	this->curr_symbl.offset = curr_symbl.offset + curr_symbl.length;
	curr = begin + curr_symbl.offset; 
	}
    
    int length = sequence_length(*curr);

    if (curr_symbl.offset <= (string_size - length + 2)) {
        this->curr_symbl.value = get_unicode(curr, length);
        this->curr_symbl.length = length;
    }
    
    else
        this->condition_iterate = 1;
}

bool tokenizer :: get_condition_iterate(){
    return this->condition_iterate;
}

void tokenizer ::  process_next_word() { //обрезает email до домена, разберись
    for (int i = 0; i != words_containers.size(); i++) {
        this->words_containers[i].new_token(DetectToken(curr_symbl.value), curr_symbl);
        if (this->words_containers[i].full_word_flag == 1) {
            word_pointer new_word(begin, this->words_containers[i].offset, this->words_containers[i].length, this->words_containers[i].type);
            this->result_strs.push_back(new_word);
        }
    }
    
}


tokenizer::~tokenizer() {
}
