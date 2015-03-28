/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: so <so@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/26 23:14:59 by jripoute          #+#    #+#             */
/*   Updated: 2015/03/29 00:49:49 by so               ###   ########.fr       */
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
		i++;
	}
	return (i);
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
	a3 = ((opcode & 0x0F00) >> 8);
	a2 = ((opcode & 0x00F0) >> 4);
	a1 = ((opcode & 0x000F));

	if (act == 0)
		; // 0NNN
	else if (act == 1)
		i_00E0_clear(win);
	else if (act == 2)
		i_00EE_jumpback(cpu);
	else if (act == 3)
		i_1NNN_jump(cpu, a1, a2, a3);
	else if (act == 4)
		i_2NNN_jump(cpu, a1, a2, a3);
	else if (act == 5)
		i_3XNN_jump(cpu, a1, a2, a3);
	else if (act == 6)
		i_4XNN_jump(cpu, a1, a2, a3);
	else if (act == 7)
		i_5XY0_jump(cpu, a3, a2);
	else if (act == 8)
		i_6XNN_assign(cpu, a1, a2, a3);
	else if (act == 9)
		i_7XNN_add(cpu, a1, a2, a3);
	else if (act == 10)
		i_8XY0_assign(cpu, a2, a3);
	else if (act == 11)
		i_8XY1_assign(cpu, a2, a3);
	else if (act == 12)
		i_8XY2_assign(cpu, a2, a3);
	else if (act == 13)
		i_8XY3_assign(cpu, a2, a3);
	else if (act == 14)
		i_8XY4_add(cpu, a3, a2);
	else if (act == 15)
		i_8XY5_sub(cpu, a2, a3);
	else if (act == 16)
		i_8XY6_shift(cpu, a2, a3);
	else if (act == 17)
		i_8XY7_sub(cpu, a3, a2);
	else if (act == 18)
		i_8XYE_shift(cpu, a2, a3);
	else if (act == 19)
		i_9XY0_jump(cpu, a2, a3);
	else if (act == 20)
		i_ANNN_assign(cpu, a1, a2, a3);
	else if (act == 21)
		i_BNNN_jump(cpu, a1, a2, a3);
	else if (act == 22)
		i_CXNN_rand(cpu, a3, a2, a1);
	else if (act == 23)
		i_DXYN_draw(cpu, win, a1, a2, a3);
	else if (act == 24)
		i_EX9E_key(cpu, a3);
	else if (act == 25)
		i_EXA1_key(cpu, a3);
	else if (act == 26)
		i_FX07_assign(cpu, a3);
	else if (act == 27)
		i_FX0A_key(cpu, a3);
	else if (act == 28)
		i_FX15_assign(cpu, a3);
	else if (act == 29)
		i_FX18_assign(cpu, a3);
	else if (act == 30)
		i_FX1E_add(cpu, a3);
	else if (act == 31)
		i_FX29_assign(cpu, a3);
	else if (act == 32)
		i_FX33_stock(cpu, a3);
	else if (act == 33)
		i_FX55_stock(cpu, a3);
	else if (act == 34)
		i_FX65_stock(cpu, a3);
	cpu->mem_ptr += 2;
}

void	countf(t_cpu *cpu)
{
	if (cpu->min_sys > 0)
		cpu->min_sys--;
	if (cpu->min_son > 0)
		cpu->min_son--;
}

void	key_hook(t_cpu *cpu, SDL_Event event, int set)
{
	if (event.type == SDL_KEYDOWN)
	{
		set = 1;
		if (event.key.keysym.sym == SDLK_1)
			cpu->key[0] = set;
		else if (event.key.keysym.sym == SDLK_2)
			cpu->key[1] = set;
		else if (event.key.keysym.sym == SDLK_3)
			cpu->key[2] = set;
		else if (event.key.keysym.sym == SDLK_4)
			cpu->key[3] = set;
		else if (event.key.keysym.sym == SDLK_a)
			cpu->key[4] = set;
		else if (event.key.keysym.sym == SDLK_z)
			cpu->key[5] = set;
		else if (event.key.keysym.sym == SDLK_e)
			cpu->key[6] = set;
		else if (event.key.keysym.sym == SDLK_r)
			cpu->key[7] = set;
		else if (event.key.keysym.sym == SDLK_q)
			cpu->key[8] = set;
		else if (event.key.keysym.sym == SDLK_s)
			cpu->key[9] = set;
		else if (event.key.keysym.sym == SDLK_d)
			cpu->key[10] = set;
		else if (event.key.keysym.sym == SDLK_f)
			cpu->key[11] = set;
		else if (event.key.keysym.sym == SDLK_w)
			cpu->key[12] = set;
		else if (event.key.keysym.sym == SDLK_x)
			cpu->key[13] = set;
		else if (event.key.keysym.sym == SDLK_c)
			cpu->key[14] = set;
		else if (event.key.keysym.sym == SDLK_v)
			cpu->key[15] = set;
	}
	else if (event.type == SDL_KEYUP)
	{
		set = 0;
		if (event.key.keysym.sym == SDLK_1)
			cpu->key[0] = set;
		else if (event.key.keysym.sym == SDLK_2)
			cpu->key[1] = set;
		else if (event.key.keysym.sym == SDLK_3)
			cpu->key[2] = set;
		else if (event.key.keysym.sym == SDLK_4)
			cpu->key[3] = set;
		else if (event.key.keysym.sym == SDLK_a)
			cpu->key[4] = set;
		else if (event.key.keysym.sym == SDLK_z)
			cpu->key[5] = set;
		else if (event.key.keysym.sym == SDLK_e)
			cpu->key[6] = set;
		else if (event.key.keysym.sym == SDLK_r)
			cpu->key[7] = set;
		else if (event.key.keysym.sym == SDLK_q)
			cpu->key[8] = set;
		else if (event.key.keysym.sym == SDLK_s)
			cpu->key[9] = set;
		else if (event.key.keysym.sym == SDLK_d)
			cpu->key[10] = set;
		else if (event.key.keysym.sym == SDLK_f)
			cpu->key[11] = set;
		else if (event.key.keysym.sym == SDLK_z)
			cpu->key[12] = set;
		else if (event.key.keysym.sym == SDLK_x)
			cpu->key[13] = set;
		else if (event.key.keysym.sym == SDLK_c)
			cpu->key[14] = set;
		else if (event.key.keysym.sym == SDLK_v)
			cpu->key[15] = set;
	}

}

void	hook(t_win *win, t_cpu *cpu, t_opcode *op)
{
	int			skip;
	SDL_Event	event;
	int			i;

	skip = FALSE;
	while (skip == FALSE)
	{
		SDL_PollEvent(&event);
		skip = exit_hook(event);
		key_hook(cpu, event, 1);
		i = -1;
		while (++i < CPU_SPEED)
			interpret_opcode(cpu, op, win);
		display_screen(win);
		countf(cpu);
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

void	load_game(t_cpu *cpu, char *filename)
{
	FILE	*game;

	if ((game = fopen(filename, "rb")) == NULL)
		ft_error(NOFILE);
	fread(&cpu->mem[MEM_START], 
		sizeof(Uint8) * (MEM_SIZE - MEM_START), 1, game);
	fclose(game);
}

int		main(int ac, char **av)
{
	t_cpu		cpu;
	t_win		win;
	t_opcode	op;

	if (ac != 2)
		ft_error(NOARGZ);
	init_all(&win, &cpu, &op);
	load_game(&cpu, av[1]);

	hook(&win, &cpu, &op);
	exit_all(&win);
	return (0);
}
