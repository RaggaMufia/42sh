#include "42sh.h"

t_grp	*parse_grp(t_sh *shell, char *line, t_fds *def_fd, int back)
{
  t_grp	*grp;

  if ((grp = create_n_process_group(shell, my_strdup(line))) == NULL)
    return (NULL);
  change_fdout_t_def_z(grp, def_fd);
  if (back == 0)
    SETFLAG(grp->flags, FLAGPOS(FGRP_FORGROUND));
  return (grp);
}

t_grp	*parse_linked_grp_process(t_sh *shell, char *line,
                                t_fds *def_fd, int back)
{
  t_grp	*grp;
  int	type;
  char	*next_line;

  next_line = type_next_and_or(line, &type);
  if ((grp = parse_grp(shell, line, def_fd, back)) != NULL)
    {
      grp->transition = type;
      grp->transition_line = next_line;
    }
  else
    free(next_line);
  if (MEXIT)
    return (NULL);
  return (grp);
}

int	parse_launch_background(char **line)
{
  int	back;
  int	i;
  char	**tab;

  back = 0;
  i = 0;
  tab = str_to_wordtab((*line), " ", 1);
  if (tab != NULL)
    {
      i = ptr_tab_size((void**)tab);
      if (i > 0)
        if (!(my_strncmp(tab[i - 1], "&", -1)))
          {
            back = 1;
            free(tab[i - 1]);
            tab[i - 1] = NULL;
          }
      free((*line));
      (*line) = strtab_to_str(tab, " ");
      free_ptr_tab((void**)tab, &free);
    }
  return (back);
}

t_grp	**parse_colon_line(t_sh *shell, char *line, t_fds *def_fd)
{
  char	**tmptab;
  t_grp	**grp_lst;
  char	**tmpline;
  int	i;
  int	back;

  i = 0;
  grp_lst = NULL;
  if ((tmptab = str_to_wordtab(line, ";", 1)) != NULL)
    while ((*(tmpline = &(tmptab[i]))) != NULL)
      {
        back = parse_launch_background(tmpline);
        grp_lst = (t_grp**)add_ptr_t_tab((void**)grp_lst,
                                         (void*)parse_linked_grp_process
                                         (shell, (*tmpline), def_fd, back));
        if (MEXIT)
          return (NULL);
        i++;
      }
  free_ptr_tab((void**)tmptab, &free);
  return (grp_lst);
}

void	parse_user_cmd(t_sh *shell, char **line, t_fds *def_fd)
{
  t_grp	**grp_lst;

  if ((shell->too_much_parsing)++ > 1000)
    return ;
  if (MEXIT)
    return ;
  grp_lst = parse_colon_line(shell, (*line), def_fd);
  if (MEXIT)
    return ;
  exec_grp_lst(grp_lst, shell);
  free(grp_lst);
}
