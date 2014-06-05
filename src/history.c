#include "42sh.h"

int	load_history_f_file(char *line, t_sh *shell)
{
  add_history(&(shell->history), line);
  return (1);
}

int		view_history(UNSEDP char *path, UNSEDP char **argv, t_sh *shell)
{
  t_history	*pcourant;

  if (shell->history == NULL)
    return (-1);
  pcourant = shell->history;
  while (pcourant->next != NULL)
    pcourant = pcourant->next;
  while (pcourant != NULL)
    {
      my_putstr(" ", 1, -1);
      my_put_nbr(pcourant->nb_history);
      my_putstr("  ", 1, -1);
      my_putstr(pcourant->cmd, 1, my_strlen(pcourant->cmd));
      my_putstr("\n", 1, -1);
      pcourant = pcourant->back;
    }
  return (0);
}

void	clear_history(t_history *ptr)
{
  if (ptr == NULL)
    return ;
  while (ptr->next != NULL)
    {
      ptr = ptr->next;
      free(ptr->back->cmd);
      free(ptr->back);
    }
  if (ptr != NULL)
    free(ptr->cmd);
  free(ptr);
}
