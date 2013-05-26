/*
** FILE for FILE in /home/moriss_h/projet
**
** Made by hugues morisset
** Login   <moriss_h@epitech.net>
**
** Started on  Mon Oct  8 09:34:29 2012 hugues morisset
** Last update Tue May 21 14:28:33 2013 maxime lavandier
*/

#include "42sh.h"

char	*exec_line_a_g_res(char **line, t_sh *shell)
{
  char	*str;
  int	sizeread;
  int	pipefd[2];
  char	buffer[READ_SIZE];
  t_fds	tmpfd;
  t_grp	*grp;

  str = my_strdup("\"");
  if (pipe(pipefd) == -1)
    return (my_strdup(""));
  init_stdfd_t_def_val(&tmpfd, 0, pipefd[PIPE_WRITE], 2);
  grp = parse_linked_grp_process(shell, (*line), &tmpfd, 0);
  exec_process_group(shell, grp);
  if (MEXIT)
    return (NULL);
  sizeread = 1;
  close(pipefd[PIPE_WRITE]);
  while (sizeread > 0)
    if ((sizeread = read(pipefd[PIPE_READ], buffer, READ_SIZE)) > 0)
      str = my_stradd(str, buffer, sizeread);
  close(pipefd[PIPE_READ]);
  str = my_stradd(str, "\"", 1);
  wait_no_fg_grp(shell);
  return (str);
}

void	check_and_load_backquote(char **line, t_sh *shell)
{
  char	**backtab;
  int	i;
  char	*str;

  i = 0;
  if ((backtab = str_to_wordtab((*line), "`", 1)) != NULL)
    while (backtab[i] != NULL)
      {
        if ((i % 2) == 1)
          {
            if ((str = exec_line_a_g_res(&(backtab[i]), shell)) != NULL)
              {
                free(backtab[i]);
                backtab[i] = str;
              }
            if (MEXIT)
              return ;
          }
        i++;
      }
  str = strtab_to_str(backtab, " ");
  free_ptr_tab((void**)backtab, &free);
  tr_str(str, '\n', ' ');
  free((*line));
  (*line) = str;
}
