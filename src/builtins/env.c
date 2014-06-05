#include "42sh.h"

int	builtin_env_print(UNSEDP char *path, char **argv, t_sh *shell)
{
  int	i;

  i = 0;
  if (shell->env != NULL)
    while (shell->env[i] != NULL)
      {
        my_putstr(shell->env[i], 1, -1);
        if (argv[0][0] != 'E')
          my_putstr("\n", 1, -1);
        i++;
      }
  return (0);
}

int	builtin_env_exec_opt(char *arg, UNSEDP t_cmd *cmd,
                         char ***env, int *nnewline)
{
  int	opt;
  char	*tmp;

  opt = 0;
  tmp = NULL;
  if (!my_strncmp(arg, "-i", -1)
      || !my_strncmp(arg, "--ignore-environment", -1))
    {
      opt = 1;
      free_ptr_tab((void**)*env, &free);
      *env = NULL;
    }
  if (!my_strncmp(arg, "-0", -1) || !my_strncmp(arg, "--null", -1))
    {
      opt = 1;
      *nnewline = 1;
    }
  if (!my_strncmp(arg, "--unset=", my_strlen("--unset=")))
    {
      if (!my_strncmp(arg, "--unset=", my_strlen("--unset=")))
        tmp = &(arg[my_strlen("--unset=")]);
      rm_env(*env, tmp);
      opt = 1;
    }
  return (opt);
}

void	builtin_env_get_flags(t_cmd *cmd, char ***env, int *nnewline)
{
  int	i;

  i = 0;
  while (cmd->argv[i] != NULL)
    {
      if (builtin_env_exec_opt(cmd->argv[i], cmd, env, nnewline))
        cmd->argv[i][0] = '\0';
      i++;
    }
  rm_empty_str_f_tab(cmd->argv);
}

void	builtin_env(t_cmd *cmd, t_fds *fd, t_sh *shell)
{
  char	**tmpenv;
  char	**env;
  char	*line;
  int	nnewlin;

  nnewlin = 0;
  tmpenv = cpy_env(shell->env);
  builtin_env_get_flags(cmd, &tmpenv, &nnewlin);
  env = shell->env;
  shell->env = tmpenv;
  if (cmd->argv != NULL && cmd->argv[1] != NULL)
    {
      line = strtab_to_str(&(cmd->argv[1]), " ");
      parse_user_cmd(shell, &line, fd);
    }
  else if (cmd->argv != NULL)
    {
      if (nnewlin == 1)
        cmd->argv[0][0] = 'E';
      exec_process(cmd, fd, shell, &builtin_env_print);
    }
  shell->env = env;
  free_ptr_tab((void**)tmpenv, &free);
}
