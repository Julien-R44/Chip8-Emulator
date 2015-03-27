/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instructions1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 03:19:45 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 03:52:56 by jripoute         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chip.h"

void	i_00E0_clear(t_win *win)
{
	int i;
	int j;

	i = 0;
	while (i < PIX_WIDTH)
	{
		j = -1;
		while (++j < PIX_HEIGHT)
			win->pixel[i][j].color = WHITE;
		i++;
	}
}

void	i_1NNN_jump(t_cpu *cpu)
{
	// cpu->mem_ptr = ;
}

void	i_3XNN_jmp(t_cpu *cpu, Uint8 x, Uint8 a2, Uint8 a1)
{
	if (cpu->v[x] == ((a1 << 2) + a2))
		cpu->mem_ptr += 2;
}
