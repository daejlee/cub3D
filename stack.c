/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 19:28:13 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 20:17:20 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_stack	*init_stack(void)
{
	t_stack	*stack;
	
	stack = (t_stack *)malloc(sizeof(t_stack));
	if (!stack)
		parse_err(MALLOC_FAIL);
	stack->size = 0;
	stack->top = NULL;
	return (stack);
}

t_node	*init_node(int x, int y)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		parse_err(MALLOC_FAIL);
	node->x = x;
	node->y = y;
	node->prev = NULL;
	return (node);
}

void	push_stack(t_stack *stack, t_node *node)
{
	if (!stack || !node)
		parse_err(MALLOC_FAIL); //고쳐야함
	node->prev = stack->top;
	stack->top = node;
	stack->size++;
}

t_node	*pop_stack(t_stack *stack)
{
	t_node	*node;

	if (!stack)
		parse_err(MALLOC_FAIL); //고쳐야함
	if (!stack->size)
		parse_err(MALLOC_FAIL); //고쳐야함
	node = stack->top;
	stack->top = stack->top->prev;
	stack->size--;
	return (node);
}

void	dfs(t_info *info, int x, int y)
{
	t_stack *stack;
	t_node	*node;

	if (info->map[x][y] != ' ')
		return ;
	stack = init_stack();
	push_stack(stack, init_node(x, y));
	while (stack->size)
	{
		node = pop_stack(stack);
		info->map[node->x][node->y] = '1';
		if (info->map[node->x + 1][node->y] == ' ')
			push_stack(stack, init_node(node->x + 1, node->y));
		else if (info->map[node->x + 1][node->y] == '0')
			parse_err(INVALID_MAP);
		if (info->map[node->x][node->y + 1] == ' ')
			push_stack(stack, init_node(node->x, node->y + 1));
		else if (info->map[node->x][node->y + 1] == '0')
			parse_err(INVALID_MAP);
		if (info->map[node->x + 1][node->y + 1] == ' ')
			push_stack(stack, init_node(node->x + 1, node->y + 1));
		else if (info->map[node->x + 1][node->y + 1] == '0')
			parse_err(INVALID_MAP);
	}
}
