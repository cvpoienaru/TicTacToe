/**
 * Copyright (c) 2015, Codrin-Victor Poienaru.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * This software is provided by the copyright holders and contributors "as is"
 * and any express or implied warranties, including, but not limited to, the
 * implied warranties of merchantability and fitness for a particular purpose
 * are disclaimed. In no event shall the copyright holder or contributors be
 * liable for any direct, indirect, incidental, special, exemplary, or
 * consequential damages (including but not limited to, procurement of
 * substitute goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether in
 * contract, strict liability, or tort (including negligence or otherwise)
 * arising in any way out of the use of this software, even if advised of the
 * possibility of such damage.
 */

/**
 * 0, Green - PORTA, PC4;
 * 0, Red   - PORTA, PC5;
 * 1, Green - PORTA, PC0;
 * 1, Red   - PORTA, PC1;
 * 2, Green - PORTA, PC2;
 * 2, Red   - PORTA, PC3;
 * 3, Green - PORTC, PC7;
 * 3, Red   - PORTC, PC6;
 * 4, Green - PORTC, PC1;
 * 4, Red   - PORTC, PC0;
 * 5, Green - PORTD, PC4;
 * 5, Red   - PORTD, PC5;
 * 6, Green - PORTB, PC5;
 * 6, Red   - PORTB, PC4;
 * 7, Green - PORTB, PC2;
 * 7, Red   - PORTB, PC3;
 * 8, Green - PORTB, PC1;
 * 8, Red   - PORTB, PC0;
 *
 * Up     - PORTB, PB6
 * Down   - PORTD, PB0
 * Right  - PORTB, PB7
 * Left   - PORTD, PB1
 * Select - PORTA, PB6
 */

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "game.h"

#define DEBUG 0
#define ON 1
#define OFF 0

static int* board;
static int player;
static int start_row;
static int start_column;
static int current_row;
static int current_column;

void initialize_registers(void);
void switch_led(int row, int column, int player, int mode);
void leds_mode(int player, int mode);
void refresh_leds(void);
void test_leds(void);
void up_button_pressed(void);
void down_button_pressed(void);
void left_button_pressed(void);
void right_button_pressed(void);
void select_button_pressed(void);
void test_button_pressed(void);

void initialize_game(void);
void change_player(void);
void destroy_game(void);

void initialize_registers(void)
{
	DDRA |= (1 << PC0);
	DDRA |= (1 << PC1);
	DDRA |= (1 << PC2);
	DDRA |= (1 << PC3);
	DDRA |= (1 << PC4);
	DDRA |= (1 << PC5);
	DDRB |= (1 << PC0);
	DDRB |= (1 << PC1);
	DDRB |= (1 << PC2);
	DDRB |= (1 << PC3);
	DDRB |= (1 << PC4);
	DDRB |= (1 << PC5);
	DDRC |= (1 << PC0);
	DDRC |= (1 << PC1);
	DDRC |= (1 << PC6);
	DDRC |= (1 << PC7);
	DDRD |= (1 << PC4);
	DDRD |= (1 << PC5);
	
	DDRD &= ~(1 << PB0);
	DDRD &= ~(1 << PB1);
	DDRA &= ~(1 << PB6);
	DDRB &= ~(1 << PB6);
	DDRB &= ~(1 << PB7);
	
	PORTD |= (1 << PB0);
	PORTD |= (1 << PB1);
	PORTA |= (1 << PB6);
	PORTB |= (1 << PB6);
	PORTB |= (1 << PB7);
}

void switch_led(int row, int column, int player, int mode)
{
	switch(row * BOARD_SIZE + column) {
		case 0:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTA |= (1 << PC4);
				else if(player == PLAYER_B)
					PORTA |= (1 << PC5);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTA &= ~(1 << PC4);
				else if(player == PLAYER_B)
					PORTA &= ~(1 << PC5);
			}
			break;
			
		case 1:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTA |= (1 << PC0);
				else if(player == PLAYER_B)
					PORTA |= (1 << PC1);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTA &= ~(1 << PC0);
				else if(player == PLAYER_B)
					PORTA &= ~(1 << PC1);
			}
			break;

		case 2:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTA |= (1 << PC2);
				else if(player == PLAYER_B)
					PORTA |= (1 << PC3);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTA &= ~(1 << PC2);
				else if(player == PLAYER_B)
					PORTA &= ~(1 << PC3);
			}
			break;

		case 3:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTC |= (1 << PC7);
				else if(player == PLAYER_B)
					PORTC |= (1 << PC6);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTC &= ~(1 << PC7);
				else if(player == PLAYER_B)
					PORTC &= ~(1 << PC6);
			}
			break;

		case 4:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTC |= (1 << PC1);
				else if(player == PLAYER_B)
					PORTC |= (1 << PC0);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTC &= ~(1 << PC1);
				else if(player == PLAYER_B)
					PORTC &= ~(1 << PC0);
			}
			break;

		case 5:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTD |= (1 << PC4);
				else if(player == PLAYER_B)
					PORTD |= (1 << PC5);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTD &= ~(1 << PC4);
				else if(player == PLAYER_B)
					PORTD &= ~(1 << PC5);
			}
			break;

		case 6:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTB |= (1 << PC5);
				else if(player == PLAYER_B)
					PORTB |= (1 << PC4);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTB &= ~(1 << PC5);
				else if(player == PLAYER_B)
					PORTB &= ~(1 << PC4);
			}
			break;

		case 7:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTB |= (1 << PC2);
				else if(player == PLAYER_B)
					PORTB |= (1 << PC3);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTB &= ~(1 << PC2);
				else if(player == PLAYER_B)
					PORTB &= ~(1 << PC3);
			}
			break;

		case 8:
			if(mode == ON) {
				if(player == PLAYER_A)
					PORTB |= (1 << PC1);
				else if(player == PLAYER_B)
					PORTB |= (1 << PC0);
			} else if(mode == OFF) {
				if(player == PLAYER_A)
					PORTB &= ~(1 << PC1);
				else if(player == PLAYER_B)
					PORTB &= ~(1 << PC0);
			}
			break;

		default:
			break;
	}	
}

void leds_mode(int player, int mode)
{
	int i, j;
	for(i = 0; i < BOARD_SIZE; ++i) {
		for(j = 0; j < BOARD_SIZE; ++j) {
			switch_led(i, j, player, mode);
		}
	}
}

void refresh_leds(void)
{
	int i, j;
	
	for(i = 0; i < BOARD_SIZE; ++i) {
		for(j = 0; j < BOARD_SIZE; ++j) {
			if(!(i == current_row && j == current_column))
			switch_led(
			i,
			j,
			board[i * BOARD_SIZE + j],
			(board[i * BOARD_SIZE + j] != FREE) ? ON : OFF);
		}
	}
}

void test_leds(void)
{
	int i, j;
	
	for(i = 0; i < BOARD_SIZE; ++i) {
		for(j = 0; j < BOARD_SIZE; ++j) {
			switch_led(i, j, PLAYER_A, ON);
			_delay_ms(1500);
			switch_led(i, j, PLAYER_B, ON);
			_delay_ms(1500);
		}
	}
	
	for(i = 0; i < BOARD_SIZE; ++i) {
		for(j = 0; j < BOARD_SIZE; ++j) {
			switch_led(i, j, PLAYER_A, OFF);
			_delay_ms(1500);
			switch_led(i, j, PLAYER_B, OFF);
			_delay_ms(1500);
		}
	}
}

void up_button_pressed(void)
{
	if(current_row > 0)
		--current_row;
}

void down_button_pressed(void)
{
	if(current_row < BOARD_SIZE - 1)
		++current_row;
}

void right_button_pressed(void)
{
	if(current_column < BOARD_SIZE - 1)
		++current_column;
}

void left_button_pressed(void)
{
	if(current_column > 0)
		--current_column;
}

void select_button_pressed(void)
{
	int result = move_board(&board, current_row, current_column, player);
	if(result == FALSE)
		return;

	switch_led(current_row, current_column, player, ON);
	change_player();
	current_row = start_row;
	current_column = start_column;
}

void test_button_pressed(void)
{
	if((PINB & (1 << PB6)) == 0)
		up_button_pressed();
	else if((PIND & (1 << PB0)) == 0)
		down_button_pressed();
	else if((PINB & (1 << PB7)) == 0)
		right_button_pressed();
	else if((PIND & (1 << PB1)) == 0)
		left_button_pressed();
	else if((PINA & (1 << PB6)) == 0)
		select_button_pressed();
}

void initialize_game(void)
{
	player = PLAYER_A;
	start_row = 0;
	start_column = 0;
	current_row = 0;
	current_column = 0;

	board = alloc_board();
	init_board(&board);
	leds_mode(PLAYER_A, OFF);
	leds_mode(PLAYER_B, OFF);
}

void change_player(void)
{
	player = (player == PLAYER_A) ? PLAYER_B : PLAYER_A;
}

void destroy_game(void)
{
	free_board(&board);
}

int main(void)
{
	int result;
	
	initialize_registers();
	if(DEBUG)
		test_leds();

	while(1) {
		initialize_game();

		while(1) {
			switch_led(current_row, current_column, player, ON);
			_delay_ms(100);
			switch_led(current_row, current_column, player, OFF);
			
			refresh_leds();
			test_button_pressed();
			
			result = check_board(board);
			if(result != FALSE) {
				if(result == DRAW) {
					leds_mode(PLAYER_A, ON);
					leds_mode(PLAYER_B, ON);
				} else {
					leds_mode(PLAYER_A, OFF);
					leds_mode(PLAYER_B, OFF);
					leds_mode(result, ON);
				}

				_delay_ms(2000);
				break;
			}
		}
		
		destroy_game();
	}

	return 0;
}