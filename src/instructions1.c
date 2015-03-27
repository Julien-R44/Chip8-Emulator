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

void	i_1NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->mem_ptr = (a3 << 8) + (a2 << 4) + a1;
	cpu->mem_ptr -= 2;
}

void	i_2NNN_jump(t_cpu *cpu, Uint8 a1, Uint8 a2, Uint8 a3)
{
	cpu->stack[cpu->stack.lvl] = cpu->mem_ptr;
	if (cpu->stacklvl < 15)
		cpu->stacklvl++;
	cpu->mem_ptr = (a3 << 8) + (b2 << 4) + b1;
	cpu->mem_ptr -= 2;
}

void	i_00EE_jumpback(t_cpu *cpu)
{
	if (cpu->stacklvl > 0)
	{
		cpu->stacklvl--;
		cpu->mem_ptr = cpu->stack[cpu->stacklvl];
	}
}

void	i_3XNN_jump(t_cpu *cpu, Uint8 x, Uint8 a2, Uint8 a3)
{
	if (cpu->v[x] == ((a3 << 2) + a2))
		cpu->mem_ptr += 2;
}

void	i_8XY0_assign(t_cpu *cpu, Uint8 a3, Uint8 a2)
{
	cpu->v[a3] = cpu->v[a2];
}

void	i_CXNN_rand(t_cpu *cpu, Uint8 a3, Uint8 a2, Uint8 a1)
{
	cpu->v[a3] = (rand()) % ((a2 << 4) + b1 + 1);
}

void	i_8XY4_add(t_cpu *cpu, Uint8 a3, Uint8 a2)
{
	if ((cpu->v[a3] += cpu->v[a2]) > 0xFF)
		cpu.v[0xF] = 1;
	else
		cpu.v[0xF] = 0;
}

void	i_8XY7_sub(t_cpu *cpu, Uint8 a3, Uint8 a2)
{
	if (cpu->v[a2] > cpu->v[a3])
		cpu.v[0xF] = 1;
	else
		cpu.v[0xF] = 0;
	cpu->v[a3] = cpu->v[a2] - cpu->v[a3];
}

void	i_FX33_stock(t_cpu *cpu, Uint8 a3)
{
	cpu->mem[cpu->vi] = (cpu->v[a3] - cpu->v[a3] % 100) / 100;
	cpu->mem[cpu->vi+1] = (((cpu->v[a3] - cpu->v[a3] % 10) / 10) % 10);
	cpu->mem[cpu->vi+2] = cpu->v[a3] - cpu->mem[cpu->vi] * 100
		- cpu->mem[cpu->vi + 1] * 10;
}
