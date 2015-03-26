/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/26 23:14:59 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 00:35:50 by jripoute         ###   ########.fr       */
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

int		exit_hook(SDL_Event event)
{
	if (event.type == SDL_QUIT ||
		(event.type == SDL_KEYDOWN &&
		event.key.keysym.sym == SDLK_ESCAPE))
			return (1);
	return (0);
}

void	hook(t_win *win, t_cpu *cpu)
{
	int			skip;
	SDL_Event	event;

	skip = FALSE;
	while (skip == FALSE)
	{
		SDL_WaitEvent(&event);
		skip = exit_hook(event);
		display_screen(win);
		SDL_Flip(win->win);
	}
}

int		main(int ac, char **av)
{
	t_cpu	cpu;
	t_win	win;

	init_cpu(&cpu);
	init_win(&win);
	init_all(&win, &cpu);

	hook(&win, &cpu);

	exit_all(&win);
	return (0);
}
