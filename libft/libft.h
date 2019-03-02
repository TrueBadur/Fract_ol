/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 15:55:20 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/01/17 15:11:31 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include "get_next_line.h"
# include "t_vec.h"
# include "btavl.h"
# include "mtrx.h"
# include <unistd.h>
# include <string.h>
# define IN_RNGII(a, b, c) (a <= b && b <= c)
# define IN_RANGE_CI(a, b, c) (a < b && b <= c)
# define IN_RANGE_IC(a, b, c) (a <= b && b < c)
# define IN_RANGE_CC(a, b, c) (a < b && b < c)
# define FT_ABS(x) (((x) > 0) ? (x) : -(x))
# define FT_SIGN(x) (((x) > 0) ? 1 : -1)

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

void				*ft_memset(void *b, int c, size_t len);
void				ft_putnbr_fd(int n, int fd);
void				ft_putnbr(int n);
void				ft_putchar(char c);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr(char const *s);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl(char const *s);
void				ft_putendl_fd(char const *s, int fd);
char				*ft_itoa(long n);
char				*ft_strncpy(char *dst, char const *src, size_t len);
char				**ft_strsplit(char const *s, char c);
int					ft_tolower(int c);
char				*ft_tolower_str(char *str);
int					ft_toupper(int c);
char				*ft_toupper_str(char *str);
int					ft_isprint(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
char				*ft_int_to_unicode(int n, char *bytes);
size_t				ft_strlen(const char *s);
int					ft_atoi(const char *st);
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strdup(const char *s1);
void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);
void				*ft_memmove(void *dst, const void *src, size_t len);
char				*ft_strcat(char *s1, const char *s2);
char				*ft_strncat(char *s1, const char *s2, size_t n);
size_t				ft_strlcat(char *dst, const char *src, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(void const *s1, void const *s2, size_t n);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
char				*ft_strstr(const char *haystack, const char *needle);
char				*ft_strnstr(const char *haystack, const char *needle,
		size_t len);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnew(size_t size);
void				ft_strdel(char **as);
void				ft_strclr(char *s);
void				ft_striter(char *s, void (*f)(char*));
void				ft_striteri(char *s, void (*f)(unsigned int, char*));
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strequ(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s);
t_list				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void				ft_lstaddlast(t_list **alst, t_list *new);
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
void				*ft_lstpop(t_list **lst);
int					ft_abs(int a);
int					ft_isspace(int c);
void				ft_foreachint(int **arr, size_t len, void (*f)(int *n));
int					*ft_foreachintm(const int *arr, size_t len, int (*f)(int));
int					ft_sqrt(int c);
int					ft_atoi_m(const char **str);
char				*ft_itoa_base(__int128_t n, int rad);
void				*ft_realloc(void *old_data, size_t prev_size,
		size_t new_size);
size_t				ft_strlen_u(const int *s, char f);
char				ft_unilen(int u);
int					ft_intlen(long int n);
int					ft_itoa_buf(long n, char *res);
#endif
