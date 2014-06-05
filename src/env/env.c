#include "42sh.h"

char	*get_envvar(char *var, char **env)
{
  int	varlen;
  char	*varegal;

  varlen = my_strlen(var);
  if ((varegal = malloc((varlen + 2) * sizeof(char))) == NULL)
    return (NULL);
  my_strncpy(varegal, var, -1);
  varegal[varlen] = '=';
  varegal[varlen + 1] = '\0';
  while ((env != NULL) && (env[0] != NULL))
    {
      if (my_strncmp(varegal, env[0], varlen + 1) == 0)
        {
          free(varegal);
          return (&(env[0][varlen + 1]));
        }
      env = &env[1];
    }
  free(varegal);
  return (NULL);
}

char	**cpy_env(char **envp)
{
  char	**res;
  int	size;

  size = 0;
  if (envp == NULL)
    return (NULL);
  while (envp[size++] != NULL);
  if ((res = malloc(size * sizeof(char*))) == NULL)
    return (NULL);
  size--;
  while (size >= 0)
    {
      res[size] = my_strdup(envp[size]);
      size--;
    }
  return (res);
}

void	rm_env(char **envp, char *env)
{
  if (envp != NULL)
    while (envp[0] != NULL)
      {
        if ((env != NULL)
            && (my_strncmp(envp[0], env, my_strlen(env)) == 0)
            && (envp[0][my_strlen(env)] == '='))
          {
            free(envp[0]);
            envp[0] = envp[1];
            while (envp[0] != NULL)
              {
                envp[0] = envp[1];
                envp = &(envp[1]);
              }
            return ;
          }
        envp = &(envp[1]);
      }
}

char	**add_change_env(char **env, char *var, char *value)
{
  int	varlen;
  char	*varegal;
  char	**newenv;
  int	i;

  i = 0;
  if (env == NULL)
    return (NULL);
  rm_env(env, var);
  varlen = my_strlen(var) + my_strlen(value) + 1;
  i = ptr_tab_size((void**)env);
  if (!(((varegal = malloc((varlen + 2) * sizeof(char))) != NULL)
        && ((newenv = malloc((i + 2) * sizeof(char*))) != NULL)))
    return (NULL);
  i = -1;
  my_strncpy(varegal, var, -1);
  my_strncpy(&(varegal[my_strlen(varegal)]), "=", -1);
  my_strncpy(&(varegal[my_strlen(var) + 1]), value, -1);
  while (env[++i] != NULL)
    newenv[i] = (env)[i];
  newenv[i] = varegal;
  newenv[i + 1] = NULL;
  free(env);
  return (newenv);
}
