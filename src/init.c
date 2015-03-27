/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 00:31:55 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 03:15:34 by jripoute         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip.h"

static void	init_cpu(t_cpu *cpu)
{
	bzero(cpu->mem, MEM_SIZE);
	bzero(cpu->v, 16);
	bzero(cpu->stack, 16);
	cpu->vi = 0;
	cpu->min_sys = 0;
	cpu->min_son = 0;
	cpu->mem_ptr = MEM_START;
}

static void	init_pix(t_win *win)
{
	int i;
	int j;

	i = 0;
	while (i < PIX_WIDTH)
	{
		j = 0;
		while (j < PIX_HEIGHT)
		{
			win->pixel[i][j].pos.x = i * PIX_SIZE;
			win->pixel[i][j].pos.y = j * PIX_SIZE;
			win->pixel[i][j].color = WHITE;
			j++;
		}
		i++;
	}
}

static void	create_pixels_surface(t_win *win)
{
	win->pix[BLACK] = SDL_CreateRGBSurface(SDL_HWSURFACE, PIX_SIZE, PIX_SIZE,
		32, 0, 0, 0, 0);
	SDL_FillRect(win->pix[BLACK], NULL, SDL_MapRGB(win->pix[BLACK]->format,
		0, 0, 0));
	win->pix[WHITE] = SDL_CreateRGBSurface(SDL_HWSURFACE, PIX_SIZE, PIX_SIZE,
		32, 0, 0, 0, 0);
	SDL_FillRect(win->pix[WHITE], NULL, SDL_MapRGB(win->pix[WHITE]->format,
		255, 255, 255));
}

static void	init_win(t_win *win)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		ft_error(SDL_INIT);
	win->win = SDL_SetVideoMode(WIDTH, HEIGHT, 32,
		SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Chip8 Emulator", NULL);
	if (win->win == NULL)
		ft_error(SDL_INIT);
	create_pixels_surface(win);
}

/*
	Les X, Y, et N sont des inconnus.
		- NNN adresse de 12bits
		- NN constante de 8bits
		- N constante de 4bits
		- X et Y : id registre de 4bits
 */
void	init_op(t_opcode *op)
{
	op->mask[0] = 0x0000;	op->id[0] = 0x0FFF;		// 0NNN
	op->mask[1] = 0xFFFF;	op->id[1] = 0x00E0;		// 00E0
	op->mask[2] = 0xFFFF;	op->id[2] = 0x00EE;		// 00EE
	op->mask[3] = 0xF000;	op->id[3] = 0x1000;		// 1NNN
	op->mask[4] = 0xF000;	op->id[4] = 0x2000;		// 2NNN
	op->mask[5] = 0xF000;	op->id[5] = 0x3000;		// 3XNN
	op->mask[6] = 0xF000;	op->id[6] = 0x4000;		// 4XNN
	op->mask[7] = 0xF00F;	op->id[7] = 0x5000;		// 5XY0
	op->mask[8] = 0xF000;	op->id[8] = 0x6000;		// 6XNN
	op->mask[9] = 0xF000;	op->id[9] = 0x7000;		// 7XNN
	op->mask[10] = 0xF00F;	op->id[10] = 0x8000;	// 8XY0
	op->mask[11] = 0xF00F;	op->id[11] = 0x8001;	// 8XY1
	op->mask[12] = 0xF00F;	op->id[12] = 0x8002;	// 8XY2
	op->mask[13] = 0xF00F;	op->id[13] = 0x8003;	// BXY3
	op->mask[14] = 0xF00F;	op->id[14] = 0x8004;	// 8XY4
	op->mask[15] = 0xF00F;	op->id[15] = 0x8005;	// 8XY5
	op->mask[16] = 0xF00F;	op->id[16] = 0x8006;	// 8XY6
	op->mask[17] = 0xF00F;	op->id[17] = 0x8007;	// 8XY7
	op->mask[18] = 0xF00F;	op->id[18] = 0x800E;	// 8XYE
	op->mask[19] = 0xF00F;	op->id[19] = 0x9000;	// 9XY0
	op->mask[20] = 0xF000;	op->id[20] = 0xA000;	// ANNN
	op->mask[21] = 0xF000;	op->id[21] = 0xB000;	// BNNN
	op->mask[22] = 0xF000;	op->id[22] = 0xC000;	// CXNN
	op->mask[23] = 0xF000;	op->id[23] = 0xD000;	// DXYN
	op->mask[24] = 0xF0FF;	op->id[24] = 0xE09E;	// EX9E
	op->mask[25] = 0xF0FF;	op->id[25] = 0xE0A1;	// EXA1
	op->mask[26] = 0xF0FF;	op->id[26] = 0xF007;	// FX07
	op->mask[27] = 0xF0FF;	op->id[27] = 0xF00A;	// FX0A
	op->mask[28] = 0xF0FF;	op->id[28] = 0xF015;	// FX15
	op->mask[29] = 0xF0FF;	op->id[29] = 0xF018;	// FX18
	op->mask[30] = 0xF0FF;	op->id[30] = 0xF01E;	// FX1E
	op->mask[31] = 0xF0FF;	op->id[31] = 0xF029;	// FX29
	op->mask[32] = 0xF0FF;	op->id[32] = 0xF033;	// FX33
	op->mask[33] = 0xF0FF;	op->id[33] = 0xF055;	// FX55
	op->mask[34] = 0xF0FF;	op->id[34] = 0xF065;	// FX65
}

void	init_all(t_win *win, t_cpu *cpu, t_opcode *op)
{
	init_cpu(cpu);
	init_pix(win);
	init_win(win);
	init_op(op);
}
