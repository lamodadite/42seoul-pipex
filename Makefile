NAME:=pipex

#BONUS:=checker

SRC_DIR:=srcs
#BONUS_SRC_DIR:=bonus_srcs

SRCS:=main.c parse.c free.c exit.c utils.c

#BONUS_SRC:=

INC_DIR:=incs
INCS:=pipex.h

LIBFT:=ft
LIBFT_DIR:=libft

CC:=cc
CFLAGS:=-Wall -Wextra -Werror

$(NAME) : $(addprefix $(SRC_DIR)/, $(SRCS)) $(addprefix $(INC_DIR)/, $(INCS))
	@make -sC $(LIBFT_DIR) all
	$(CC) $(CFLAGS) -o $@ $(addprefix $(SRC_DIR)/, $(SRCS)) -I $(INC_DIR) \
	-I $(LIBFT_DIR) -L $(LIBFT_DIR) -l$(LIBFT)

#$(BONUS) : $(addprefix $(BONUS_SRC_DIR)/, $(BONUS_SRC)) $(addprefix $(INC_DIR)/, $(INCS))
#	@make -sC $(LIBFT_DIR) all
#	$(CC) $(CFLAGS) -o $@ $(addprefix $(BONUS_SRC_DIR)/, $(BONUS_SRC)) -I $(INC_DIR) \
#	-I $(LIBFT_DIR) -L $(LIBFT_DIR) -l$(LIBFT)

.PHONY : all clean fclean re bonus

all : $(NAME)

#bonus : $(BONUS)

clean :
	make -C $(LIBFT_DIR) fclean

fclean : clean
	rm -f $(NAME)
#$(BONUS)

re : fclean all
