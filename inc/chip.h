#ifndef CHIP_H
# define CHIP_H

# include <SDL/SDL.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum		s_er
{
	SDL_INIT,
}					t_er;

typedef enum	s_bool
{
	FALSE = 0,
	TRUE,
}				t_bool;

# define CPU_SPEED	4
# define FPS		16

# define MEM_SIZE	4096
# define MEM_START	512

// La Chip 8 contient 4096 octet. Les 512 premiers octet sont reserve pour l'interpreteur.
// 16 registres d'un octet chacun. Ainsi que le registre d'adresse de 16 bits : (vi)
// Une stack de 16 niveaux.
// Deux compteurs qui decomptent a 60 Hertz jusqu'a zero
typedef struct		s_cpu
{
	Uint8			mem[MEM_SIZE];
	Uint8			v[16];
	Uint16			vi;
	Uint16			stack[16]; // remplacer par liste
	Uint8			stacklvl; //
	Uint8			min_sys;
	Uint8			min_son;
	Uint16			mem_ptr;
}					t_cpu;

# define BLACK 0
# define WHITE 1
# define PIX_WIDTH	64
# define PIX_HEIGHT	32
# define PIX_SIZE	8
# define HEIGHT		PIX_HEIGHT * PIX_SIZE
# define WIDTH		PIX_WIDTH * PIX_SIZE

// La resolution est de 64 x 32 pixels
typedef struct		s_pixel
{
	SDL_Rect		pos;
	Uint8			color;
}					t_pixel;

typedef struct		s_win
{
	t_pixel			pixel[PIX_WIDTH][PIX_HEIGHT];
	SDL_Surface		*win;
	SDL_Surface		*pix[2];
}					t_win;

# define NB_OPCODE	35

typedef struct		s_opcode
{
	Uint16			mask[NB_OPCODE];
	Uint16			id[NB_OPCODE];
}					t_opcode;

// ----- Prototypes
void				init_all(t_win *win, t_cpu *cpu, t_opcode *op);

// instructions
void				i_00E0_clear(t_win *win);
void				i_1NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3);
void				i_2NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3);
void				i_00EE_jumpback(t_cpu *cpu);
void				i_3XNN_jump(t_cpu *cpu, Uint8 x, Uint8 a2, Uint8 a3);
void				i_8XY0_assign(t_cpu *cpu, Uint8 a3, Uint8 a2);
void				i_CXNN_rand(t_cpu *cpu, Uint8 a3, Uint8 a2, Uint8 a1);
void				i_8XY4_add(t_cpu *cpu, Uint8 a3, Uint8 a2);
void				i_8XY7_sub(t_cpu *cpu, Uint8 a3, Uint8 a2);
void				i_FX33_stock(t_cpu *cpu, Uint8 a3);

#endif
