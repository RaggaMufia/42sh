#include "42sh.h"

/*
** Replace alias
*/

void	alias_replace(char ***argv, char **aliastab)
{
  char	*alias;
  char	*assoc;
  char	**arg_alias;
  char	**tmp_tab;

  if (((*argv) == NULL) || ((*argv)[0] == NULL))
    return ;
  alias = (*argv)[0];
  if ((assoc = get_envvar(alias, aliastab)) == NULL)
    return ;
  if ((arg_alias = str_to_wordtab(assoc, " ", 2)) == NULL)
    return ;
  tmp_tab = (*argv);
  (*argv) = (char**)concat_ptr_tab((void**)arg_alias, (void**)(&((*argv)[1])));
  free(arg_alias);
  free(tmp_tab[0]);
  free(tmp_tab);
}

void	alias_multiple_replace(char ***argv, char **aliastab)
{
  int	i;
  char	*lcmd;
  char	*ncmd;

  i = 0;
  ncmd = NULL;
  if (((*argv) == NULL) || ((*argv)[0] == NULL))
    return ;
  lcmd = my_strdup((*argv)[0]);
  while ((i < 500) && (my_strncmp(lcmd, ncmd, -1)))
    {
      free(lcmd);
      lcmd = my_strdup((*argv)[0]);
      alias_replace(argv, aliastab);
      free(ncmd);
      ncmd = my_strdup((*argv)[0]);
      i++;
    }
  free(ncmd);
  free(lcmd);
}
