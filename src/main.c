/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/26 23:14:59 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 03:55:01 by jripoute         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip.h"

void	display_screen(t_win *win)
{
	int i;
	int j;
	int	color;

	i = 0;
	while (i < PIX_WIDTH)
	{
		j = 0;
		while (j < PIX_HEIGHT)
		{
			SDL_Blit2(win->pix[win->pixel[i][j].color], win->win,
				win->pixel[i][j].pos.x, win->pixel[i][j].pos.y, NULL);
			j++;
		}
		i++;
	}
}

Uint16	get_opcode(t_cpu *cpu)
{
	return ((cpu->mem[cpu->mem_ptr] << 8) + cpu->mem[cpu->mem_ptr + 1]);
}

Uint8	get_action(t_opcode *op, Uint16 opcode)
{
	int i;
	Uint16 ret;

	i = 0;
	while (i < NB_OPCODE)
	{
		ret = (op->mask[i] & opcode);
		if (ret == op->id[i])
			break ;
	}
	return (ret);
}

int		exit_hook(SDL_Event event)
{
	if (event.type == SDL_QUIT ||
		(event.type == SDL_KEYDOWN &&
		event.key.keysym.sym == SDLK_ESCAPE))
			return (1);
	return (0);
}

// L'opcode DXYN dessine un sprite en pos. X Y et de hauteur N
void	i_DXYN_draw(t_cpu *cpu, t_win *win, Uint8 x, Uint8 y, Uint8 h)
{
	Uint8	i = 0, j = 0, px = 0, py = 0, dec = 7;
	Uint8	lign;

	cpu->v[0xF] = 0; // carry flag
	while (i < h)
	{
		lign = cpu->mem[cpu->vi + i];
		py = (cpu->v[y] + i) % PIX_HEIGHT;
		while (j < 8)
		{
			px = (cpu->v[x] + j) % PIX_WIDTH;
			if ((lign >> dec) & 0x1)
			{
				if (win->pixel[px][py].color == WHITE)
				{
					win->pixel[px][py].color = BLACK;
					cpu->v[0xF] = 1;
				}
				else
					win->pixel[px][py].color = WHITE;
			}
			dec--;
			j++;
		}
		dec = 7;
		j = 0;
		i++;
	}
}

void	interpret_opcode(t_cpu *cpu, t_opcode *op, t_win *win)
{
	Uint8	a1, a2, a3;
	Uint8	act;
	Uint16	opcode;

	opcode = get_opcode(cpu);
	act = get_action(op, opcode);
	a3 = ((opcode & 0x0F00) >> 8);
	a2 = ((opcode & 0x00F0) >> 4);
	a1 = ((opcode & 0x000F));

	if (act == 0)
		;
	if (act == 1)
		i_00E0_clear(win);
	if (act == 2)
		; // 1NNN
	if (act == 3)
		; // 2NNN
	if (act == 4)
		; // 00EE
	if (act == 5)
		i_3XNN_jmp(cpu, a3, a2, a1);
	if (act == 23)
		i_DXYN_draw(cpu, win, a1, a2, a3);
	cpu->mem_ptr += 2;
}


void	hook(t_win *win, t_cpu *cpu, t_opcode *op)
{
	int			skip;
	SDL_Event	event;

	skip = FALSE;
	while (skip == FALSE)
	{
		interpret_opcode(cpu, op, win);
		// SDL_WaitEvent(&event);
		// skip = exit_hook(event);
		// display_screen(win);
		SDL_Flip(win->win);
		SDL_Delay(FPS);
	}
}

void 	display_binary(unsigned int n)
{
	int i;

	printf("%d\t : ", n);
	for(i = 16; i >=0; i--)
		printf("%d", (n >> i ) & 1);
	printf("\n");
}

int		main(int ac, char **av)
{
	t_cpu		cpu;
	t_win		win;
	t_opcode	op;

	Uint16 test = 0xF142;

	Uint8 a1, a2, a3;

	a3 = ((test & 0x0F00) >> 8);
	a2 = ((test & 0x00F0) >> 4);
	a1 = ((test & 0x000F));

	Uint8 a4;

	display_binary(test);

	display_binary(a3);
	display_binary(a2);
	display_binary(a1);

	// i_1NNN_jump

	// init_all(&win, &cpu, &op);
	// hook(&win, &cpu, &op);
	// exit_all(&win);
	return (0);
}
