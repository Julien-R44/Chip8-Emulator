/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 00:35:39 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 00:35:48 by jripoute         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip.h"

void	ft_error(int er)
{
	if (er == SDL_INIT)
		printf("SDL failed at init.\n");
	else if (er == NOARGZ)
		printf("./chip8 <game.ch8>\n");
	else if (er == NOFILE)
		printf("No such file.\n");
	exit(-1);
}


void	SDL_Blit2(SDL_Surface *src, SDL_Surface *dst, int x, int y,
	SDL_Rect *clip)
{
	SDL_Rect	offset;

	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, clip, dst, &offset);
}

void	exit_all(t_win *win)
{
	SDL_FreeSurface(win->pix[0]);
	SDL_FreeSurface(win->pix[1]);
	SDL_Quit();
}
