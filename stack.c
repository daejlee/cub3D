/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 19:28:13 by hkong             #+#    #+#             */
/*   Updated: 2023/02/11 15:07:45 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_stack	*init_stack(void)
{
	t_stack	*stack;

	stack = (t_stack *)malloc(sizeof(t_stack));
	if (!stack)
		err(MALLOC_FAIL);
	stack->size = 0;
	stack->top = NULL;
	return (stack);
}

t_node	*init_node(int x, int y)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		err(MALLOC_FAIL);
	node->x = x;
	node->y = y;
	node->prev = NULL;
	return (node);
}

void	push_stack(t_stack *stack, t_node *node)
{
	if (!stack || !node)
		err(UNEXPECTED);
	node->prev = stack->top;
	stack->top = node;
	stack->size++;
}

t_node	*pop_stack(t_stack *stack)
{
	t_node	*node;

	if (!stack || !stack->size)
		err(UNEXPECTED);
	node = stack->top;
	stack->top = stack->top->prev;
	stack->size--;
	return (node);
}

void	free_node(t_node *node)
{
	if (!node)
		err(UNEXPECTED);
	node->prev = NULL;
	free(node);
}
