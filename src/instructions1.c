/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instructions1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jripoute <jripoute@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 03:19:45 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/27 17:18:06 by jripoute         ###   ########.fr       */
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

void	i_00EE_jumpback(t_cpu *cpu)
{
	if (cpu->stacklvl > 0)
	{
		cpu->stacklvl--;
		cpu->mem_ptr = cpu->stack[cpu->stacklvl];
	}
}

void	i_1NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->mem_ptr = (a3 << 8) + (a2 << 4) + a1;
	cpu->mem_ptr -= 2;
}

void	i_2NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->stack[cpu->stacklvl] = cpu->mem_ptr;
	if (cpu->stacklvl < 15)
		cpu->stacklvl++;
	cpu->mem_ptr = (a3 << 8) + (a2 << 4) + a1;
	cpu->mem_ptr -= 2;
}

void	i_3XNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	if (cpu->v[a3] == ((a2 << 4) + a1))
		cpu->mem_ptr += 2;
}

void	i_4XNN_jump(t_cpu *cpu, Uint8 x, Uint8 a2, Uint8 a3)
{
	if (cpu->v[x] != ((a3 << 4) + a2))
		cpu->mem_ptr += 2;
}

void	i_5XY0_jump(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	if (cpu->v[a2] == cpu->v[a3])
		cpu->mem_ptr += 2;	
}

void	i_6XNN_assign(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] = ((a2 << 4) + a1);
}

void	i_7XNN_add(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] += (a2 << 4) + a1;
}

void	i_8XY0_assign(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] = cpu->v[a2];
}

void	i_8XY1_assign(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] |= cpu->v[a2];
}

void	i_8XY2_assign(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] &= cpu->v[a2];
}

void	i_8XY3_assign(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[a3] ^= cpu->v[a2];
}

void	i_8XY4_add(t_cpu *cpu, Uint8 a3, Uint8 a2)
{
	if ((cpu->v[a3] += cpu->v[a2]) > 0xFF)
		cpu->v[0xF] = 1;
	else
		cpu->v[0xF] = 0;
}

void	i_8XY5_sub(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	if (cpu->v[a2] > cpu->v[a3])
		cpu->v[0xF] = 0;
	else
		cpu->v[0xF] = 1;
	cpu->v[a3] -= cpu->v[a2];
}

void	i_8XY6_shift(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[0xF] = (cpu->v[a3] & 1);
	cpu->v[a3] = cpu->v[a3] >> 1;
}

void	i_8XY7_sub(t_cpu *cpu, Uint8 a3, Uint8 a2)
{
	if (cpu->v[a2] > cpu->v[a3])
		cpu->v[0xF] = 1;
	else
		cpu->v[0xF] = 0;
	cpu->v[a3] = cpu->v[a2] - cpu->v[a3];
}

void	i_8XYE_shift(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	cpu->v[0xF] = (cpu->v[a3] >> 7);
	cpu->v[a3] = cpu->v[a3] << 1;
}

void	i_9XY0_jump(t_cpu *cpu, Uint8 a2, Uint8 a3)
{
	if (cpu->v[a2] != cpu->v[a3])
		cpu->mem_ptr += 2;
}

void	i_ANNN_assign(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->vi = (a3 << 8) + (a2 << 4) + a1;
}

void	i_BNNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->mem_ptr = ((a3 << 8) + (a2 << 4) + a1 + cpu->v[0]) - 2;
}

void	i_CXNN_rand(t_cpu *cpu, Uint8 a3, Uint8 a2, Uint8 a1)
{
	cpu->v[a3] = (rand()) % ((a2 << 4) + a1 + 1);
}

void	i_DXYN_draw(t_cpu *cpu, t_win *win, Uint8 a1, Uint8 a2, Uint8 a3)
{
	Uint8	i = 0, j = 0, px = 0, py = 0, dec = 7;
	Uint8	lign;

	cpu->v[0xF] = 0;
	while (i < a1)
	{
		lign = cpu->mem[cpu->vi + i];
		py = (cpu->v[a2] + i) % PIX_HEIGHT;
		while (j < 8)
		{
			px = (cpu->v[a3] + j) % PIX_WIDTH;
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

void	i_FX07_assign(t_cpu *cpu, Uint8 a3)
{
	cpu->v[a3] = cpu->min_sys;
}

void	i_FX15_assign(t_cpu *cpu, Uint8 a3)
{
	cpu->min_sys = cpu->v[a3];
}

void	i_FX18_assign(t_cpu *cpu, Uint8 a3)
{
	cpu->min_son = cpu->v[a3];
}

void	i_FX1E_add(t_cpu *cpu, Uint8 a3)
{
	if (cpu->vi + cpu->v[a3] > 0xFFF)
		cpu->v[0xF] = 1;
	else
		cpu->v[0xF] = 0;
	cpu->vi += cpu->v[a3];
}

void	i_FX29_assign(t_cpu *cpu, Uint8 a3)
{
	cpu->vi = cpu->v[a3] * 5;
}

void	i_FX33_stock(t_cpu *cpu, Uint8 a3)
{	
	cpu->mem[cpu->vi] = (cpu->v[a3] - cpu->v[a3] % 100) / 100;
	cpu->mem[cpu->vi+1] = (((cpu->v[a3] - cpu->v[a3] % 10) / 10) % 10);
	cpu->mem[cpu->vi+2] = cpu->v[a3] - cpu->mem[cpu->vi] * 100
		- cpu->mem[cpu->vi + 1] * 10;
}

void	i_FX55_stock(t_cpu *cpu, Uint8 a3)
{
	Uint8 i;

	i = 0;
	while (i <= a3)
	{
		cpu->mem[cpu->vi + i] = cpu->v[i];
		i++;
	}
}

void	i_FX65_stock(t_cpu *cpu, Uint8 a3)
{
	Uint8 i;

	i = 0;
	while (i <= a3)
	{
		cpu->v[i] = cpu->mem[cpu->vi + i];
		i++;
	}
}