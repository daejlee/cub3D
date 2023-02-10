/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 18:49:52 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/10 18:58:41 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	dfs_check_1(t_info *info, t_stack *stack, t_node *node)
{
	if (node->x + 1 < info->height)
	{
		if (info->map[node->x + 1][node->y] == ' ')
			push_stack(stack, init_node(node->x + 1, node->y));
		else if (info->map[node->x + 1][node->y] == '0')
			err(INVALID_MAP);
	}
	if (node->y + 1 < info->width)
	{
		if (info->map[node->x][node->y + 1] == ' ')
			push_stack(stack, init_node(node->x, node->y + 1));
		else if (info->map[node->x][node->y + 1] == '0')
			err(INVALID_MAP);
	}
	if (node->x - 1 >= 0)
	{
		if (info->map[node->x - 1][node->y] == ' ')
			push_stack(stack, init_node(node->x - 1, node->y));
		else if (info->map[node->x - 1][node->y] == '0')
			err(INVALID_MAP);
	}
}

static void	dfs_check_2(t_info *info, t_stack *stack, t_node *node)
{
	if (node->y - 1 >= 0)
	{
		if (info->map[node->x][node->y - 1] == ' ')
			push_stack(stack, init_node(node->x, node->y - 1));
		else if (info->map[node->x][node->y - 1] == '0')
			err(INVALID_MAP);
	}
	if (node->x + 1 < info->height && node->y + 1 < info->width)
	{
		if (info->map[node->x + 1][node->y + 1] == ' ')
			push_stack(stack, init_node(node->x + 1, node->y + 1));
		else if (info->map[node->x + 1][node->y + 1] == '0')
			err(INVALID_MAP);
	}
	if (node->x - 1 >= 0 && node->y - 1 >= 0)
	{
		if (info->map[node->x - 1][node->y - 1] == ' ')
			push_stack(stack, init_node(node->x - 1, node->y - 1));
		else if (info->map[node->x - 1][node->y - 1] == '0')
			err(INVALID_MAP);
	}
}

static void	dfs_check_3(t_info *info, t_stack *stack, t_node *node)
{
	if (node->x - 1 >= 0 && node->y + 1 < info->width)
	{
		if (info->map[node->x - 1][node->y + 1] == ' ')
			push_stack(stack, init_node(node->x - 1, node->y + 1));
		else if (info->map[node->x - 1][node->y + 1] == '0')
			err(INVALID_MAP);
	}
	if (node->x + 1 < info->height && node->y - 1 >= 0)
	{
		if (info->map[node->x + 1][node->y - 1] == ' ')
			push_stack(stack, init_node(node->x + 1, node->y - 1));
		else if (info->map[node->x + 1][node->y - 1] == '0')
			err(INVALID_MAP);
	}
}

void	dfs(t_info *info, int x, int y)
{
	t_stack	*stack;
	t_node	*node;

	if (info->map[x][y] != ' ')
		return ;
	stack = init_stack();
	push_stack(stack, init_node(x, y));
	while (stack->size)
	{
		node = pop_stack(stack);
		info->map[node->x][node->y] = '1';
		dfs_check_1(info, stack, node);
		dfs_check_2(info, stack, node);
		dfs_check_3(info, stack, node);
	}
}
