// A Chip-8 emulator (interpreter)
// by Mike Stapleton August 2019

#include <stdio.h>

/*
Memory
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/

unsigned short opcode;		//opcode is 2 bytes
unsigned char memory[4096];
unsigned char V[16];		//Registers
unsigned short I;		//Index Register??  12bits
unsigned short pc;		//Program Counter - memory address of next instruction 
unsigned char gfx[64 * 32];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned short stack[16];
unsigned short sp;
unsigned char key[16];
unsigned char draw_flag;

void initialize() {
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	
}

// fetch, decode and execute opcodes
void emulate_cycle() {
	//fetch opcode
	opcode = memory[pc] << 8 | memory[pc+1];

	//decode
	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000:
					//set draw flag to clear screen
					break;
				case 0x000E:	
					sp--;
					pc = stack[sp];
					pc += 2;
					break;
				default:
					break;
			}
		case 0x1000:
			pc = opcode & 0x0FFF;
			break;
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = memory[opcode & 0x0FFF];
			break;
		case 0x3000:
			if ( V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF) )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x4000:
			if ( V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF) )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x5000:
			if ( V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4] )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = (opcode % 0x00FF);
			pc += 2;
			break;
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += (opcode % 0x00FF);
			pc += 2;
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0001:
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0 >> 4)];
					pc += 2;
					break;
				case 0x0002:
					V[(opcode & 0x0F00) >> 8] &=  V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0003:
					V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0004:
					if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
				case 0x0005: 
					if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0006:
					V[0xF] = ( V[(opcode & 0x0F00) >> 8] & 0x1 ); 
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
					break;
				case 0x0007:
					if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1;					
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x000E:
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7; 
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
					break;
				default:
					break;				
			}
		case 0x9000:
			if ( V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4] )
				pc += 4;
			else
				pc += 2;

			break;			
		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000:
			pc = V[0] + (opcode & 0x0FFF);			
			break;
		case 0xC000: 
			V[(opcode & 0x0F00) >> 8] = (opcode && 0x00FF) & (rand() % 0xFF);
			pc +=2;
			break;
		case 0xD000: //draw
	

		case 0xF000:
			switch (opcode & 0x00FF)
				case 0x0033:
					memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;
				default:
					break;

		default:
			printf("uknown opcode\n");
	}
	
	
	// Update timers
	if(delay_timer > 0)
		--delay_timer;
	if(sound_timer > 0) {
		if(sound_timer == 1)
      			printf("BEEP!\n");
		--sound_timer;
	}  

}

void set_keys() {}
void load_game(char *name) {}
void setup_graphics() {}
void setup_memory() {}
void draw_graphics() {}

void main () {
	
	
	setup_graphics();
	setup_memory();	
	initialize();
	load_game("pong.c8");

	printf("opcode: %ld\n", sizeof(opcode));
	printf("draw_flag: %ld\n", sizeof(draw_flag));
	printf("memory: %ld\n", sizeof(memory));

	for(;;) {
		emulate_cycle();
		if(draw_flag) // opcodes 0x00E0  and 0xDXYN only
			draw_graphics();
		set_keys();    //store keypress state (press and release)
		
	}



}

