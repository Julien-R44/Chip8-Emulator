/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 00:31:55 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 00:34:15 by jripoute         ###   ########.fr       */
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

void	init_all(t_win *win, t_cpu *cpu)
{
	init_cpu(cpu);
	init_pix(win);
	init_win(win);
}
