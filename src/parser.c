/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:02:12 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/19 16:18:13 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	sncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n - 1) && (s1[i] && s2[i]) && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	sisnum(const char *s)
{
	if (s)
	{
		if (*s == '+' || *s == '-')
			s++;
		while (*s)
		{
			if (*s < '0' || *s > '9')
				return (0);
			s++;
		}
	}
	else
		return (0);
	return (1);
}

int	sisint(const char *s)
{
	int		len;
	char	pre;

	if (!sisnum(s))
		return (0);
	pre = '\0';
	if (*s == '+' || *s == '-')
		pre = *s++;
	len = -1;
	while (s[++len])
		;
	if (len > 10 || len < 1)
		return (0);
	if (len == 10)
	{
		if ((!pre || pre == '+') && sncmp(s, "2147483647", 10) > 0)
			return (0);
		if (pre == '-' && (sncmp(s, "2147483648", 10) > 0))
			return (0);
	}
	return (1);
}

int	stoi(const char *nptr)
{
	int	sum;
	int	pol;

	pol = 1;
	sum = 0;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			pol = -1;
	while (*nptr >= '0' && *nptr <= '9')
		sum = sum * 10 + (*nptr++ - '0');
	return (pol * sum);
}

int	args_parser(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac == 5 || ac == 6)
	{
		while (++i < ac)
			if (!sisint(av[i]))
				return (1);
	}
	else
		return (1);
	return (0);
}
