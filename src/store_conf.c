#include "42sh.h"

void		store_history_f(t_sh *shell, int fd)
{
  t_history	*tmp;

  tmp = shell->history;
  if (tmp == NULL)
    return ;
  while (tmp->next != NULL)
  tmp = tmp->next;
  while (tmp != NULL)
    {
      my_putstr(tmp->cmd, fd, -1);
      my_putstr("\n", fd, -1);
      tmp = tmp->back;
    }
}

void	store_conf_file(char *filename, t_sh *shell,
                      void (*f)(t_sh *shell, int fd))
{
  char	*path;
  int	fd;

  if (((path = check_vars_in_str(filename, shell->env, 0)) == NULL)
      || ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, REDI_FRIGHT)) == -1))
    {
      my_perror(path);
      return ;
    }
  f(shell, fd);
  close(fd);
  if (path != filename)
    free(path);
}
