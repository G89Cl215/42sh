/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:52:32 by baavril           #+#    #+#             */
/*   Updated: 2020/07/20 16:53:01 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

# include <stddef.h>

# define DOT '.'
# define STAR '*'
# define AROB '@'
# define EXCLAM '!'
# define OP_PAR '('
# define CL_PAR ')'
# define OP_SQUAR '['
# define CL_SQUAR ']'

# define SPC ' '
# define WHY '?'
# define PLUS '+'
# define DASH '-'
# define EQUAL '='
# define MINUS '-'
# define SLASH '/'
# define CARET '^'
# define SHARP '#'
# define TILDE '~'
# define AMPER '&'
# define COLON ':'
# define DOLLAR '$'
# define BSLASH '\\'
# define PERCENT '%'
# define DQUOTES '"'
# define SQUOTES '\''
# define OP_BRACE '{'
# define CL_BRACE '}'
# define UNDERSCORE '_'

# define EMPTY_STR ""
# define MAXCHR 95

# define ERROR -1
# define SUCCESS 0
# define DIRECT_EXP 1
# define SIMPLE_EXP 2
# define COMPLEX_EXP 3
# define WHY_EXP 4
# define PLUS_EXP 5
# define DASH_EXP 6
# define EQUAL_EXP 7
# define BSHARP_EXP 8
# define OSHARP_EXP 9
# define DSHARP_EXP 10
# define OPERCENT_EXP 11
# define DPERCENT_EXP 12
# define MATHS_EXP 13

typedef struct		s_symexp
{
	int				sym;
	int				(*expand)(char **);
}					t_symexp;

typedef struct		s_quoted
{
	int				expand;
	char			*token;
	struct s_quoted	*next;
}					t_quoted;

typedef struct		s_expand
{
	int				j;
	int				nb;
	int				type;
	char			*tmp;
	char			*btw;
	char			*ptm;
	char			*keep;
	char			**tokens;
}					t_expand;

typedef struct		s_params
{
	char			*tmp1;
	char			*tmp2;
	char			*tmp3;
	char			*tmp4;
	int				len;
	char			*esc;
}					t_params;

typedef struct		s_rev
{
	int				i;
	int				len;
	int				flag;
	char			*tmp1;
}					t_rev;

int					expansions_treatment(char **tokens, int flag);
int					expansions_launcher(t_expand *vars, int expand, int flag);
char				*expansions_management(char **tokens, int expand, int flag);

int					direct_exp(char **token);
int					simple_exp(char **token);
int					why_exp(char **token);
int					plus_exp(char **token);
int					dash_exp(char **token);
int					equal_exp(char **token);
int					bsharp_exp(char **token);
int					osharp_exp(char **token);
int					dsharp_exp(char **token);
int					opercent_exp(char **token);
int					dpercent_exp(char **token);
int					maths_exp(char **token);
int					tilde_exp(char **token);

int					ret_value(char **token);
int					reach_pos_params(char **token);
int					count_pos_params(char **token);
int					view_pos_param(char **token);
int					script_param(char **token);
char				*get_var_value(char *var_name);

int					ft_strpchr(char *str, char c);
size_t				maths_len(char *token);

int					getquotelim(char *str);
int					isexpandable(char *str, int lim);
char				*dupbtwq(char *str);
char				*dupbtwqlim(char *str, int lim);
char				**ft_expsplit(char *str);
int					back_slashed(char **tokens);
char				*set_slash(char **tokens, int flag);
char				*set_back_slash(char *tokens, int nb);
char				*getbtw(char *tokens, int type, int flag);
t_quoted			*token_quotes_generator(char *str);

int					identifier(char *token);

int					check_braces(char *token);
int					check_colon(char *token);
int					check_colon_symbol(char *token);
int					check_symbol(char *token);
int					check_maths(char *token);

int					check_next_var(char *var, char **token, int flag);
char				*ft_strcdup(char *token, char c);
char				*setasvar(char *token);

#endif
