#include "42sh.h"

char	*init_read_cmd(char *cmd, t_param *param)
{
  if (mod_raw(param->fd_tty) == EXIT_FAILURE)
    {
      my_putstr("Error mod_raw termcap\n", 2, -1);
      return (NULL);
    }
  my_putstr(param->str_prompt, 1, -1);
  if (SIZE_BUFFER <= 0 ||
      (cmd = malloc(sizeof(char) * SIZE_BUFFER)) == NULL)
    return (NULL);
  my_memset(cmd, SIZE_BUFFER, '\0');
  cmd[0] = '\0';
  param->pos = 0;
  param->pos_history = 0;
  return (cmd);
}

void	actu_begin_pos(t_param *param, int *start)
{
  *start = 1;
  get_pos_curser(&param->x, &param->y, param->fd_tty);
  param->begin_pos_x = param->x;
  param->begin_pos_y = param->y;
}

char	*loop_cmd(t_param *param, t_history **history)
{
  int	start;
  int	ret;
  char	buff[10];

  ret = 1;
  start = 0;
  while (ret > 0)
    {
      if ((ret = read(0, buff, 9)) == -1)
        return (NULL);
      buff[ret] = '\0';
      if (buff[0] == CTRLD && buff[1] == '\0')
        return (NULL);
      if (buff[0] == '\n' && buff[1] == '\0')
        return (return_string(param->cmd, param));
      if (buff[0] != '\n' && start == 0)
	actu_begin_pos(param, &start);
      if (get_window_size(param->cmd, param->begin_pos_x) == 1 &&
          gere_keyboard(buff, param->cmd, param, history) == 1)
        {
          add_character(param->cmd, param, buff[0]);
          view(param->cmd, param);
        }
    }
  return (param->cmd);
}

char	*read_cmd(char *prompt, t_param *param, t_history **history)
{
  int	tty;

  param->str_prompt = prompt;
  if ((tty = isatty(0)) == 0)
    param->fallback = 0;
  else if (check_perror("stdin", tty) == -1)
    return (NULL);
  if (param->fallback == 0)
    {
      if (isatty(0))
        my_putstr(prompt, 1, -1);
      return (get_next_line(0));
    }
  param->cmd = NULL;
  if ((param->cmd = init_read_cmd(param->cmd, param)) == NULL)
    return (NULL);
  return (loop_cmd(param, history));
}
