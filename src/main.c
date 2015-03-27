/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/26 23:14:59 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 03:13:01 by jripoute         ###   ########.fr       */
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

void	interpret_opcode(t_cpu *cpu, t_opcode *op, t_win *win)
{
	Uint8	a1, a2, a3;
	Uint8	act;
	Uint16	opcode;

	opcode = get_opcode(cpu);
	act = get_action(op, opcode);
	a1 = ((opcode & 0x0F00) >> 8);
	a2 = ((opcode & 0x00F0) >> 4);
	a3 = ((opcode & 0x000F));

	if (act == 0)
		;
	if (act == 1)
		; // 00E0
	if (act == 3)
		;
	if (act == 23)
		draw_sprite_xyn(cpu, win, a1, a2, a3);

	cpu->mem_ptr += 2;
}

// L'opcode DXYN dessine un sprite en pos. X Y et de hauteur N
void	draw_sprite_xyn(t_cpu *cpu, t_win *win, Uint8 x, Uint8 y, Uint8 h)
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
					pixel[px][py].color = BLACK;
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

void	hook(t_win *win, t_cpu *cpu, t_opcode *op)
{
	int			skip;
	SDL_Event	event;

	skip = FALSE;
	while (skip == FALSE)
	{
		interpret_opcode(cpu, op);
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
	for(i = 7; i >=0; i--)
		printf("%d", (n >> i ) & 1);
}

int		main(int ac, char **av)
{
	t_cpu		cpu;
	t_win		win;
	t_opcode	op;

	init_all(&win, &cpu, &op);
	hook(&win, &cpu, &op);
	exit_all(&win);
	return (0);
}
