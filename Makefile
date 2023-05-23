.SILENT:
NAME		= 	philo

CC 			= 	cc

INC_PATH	=	inc/

CFLAGS		= 	-Wall -Wextra -Werror -I $(INC_PATH)

RM 			= 	rm -f

SRC_PATH	=	src/

SRC_		= 	philosophers.c parser.c task.c utils00.c

SRC			=	$(addprefix $(SRC_PATH), $(SRC_))

OBJ 		= 	$(SRC_:%.c=%.o)

GREEN		= 	\033[0;32m

RESET		=	\033[0m

$(NAME):		$(OBJ)
				$(CC) $(OBJ) -o $(NAME) -pthread #-g3 -fsanitize=thread
				make done

$(OBJ):	 		$(SRC)
				$(CC) -c $(CFLAGS) $(SRC)

all: 			$(NAME)

clean:	
				$(RM) $(OBJ)

fclean:			clean
				$(RM) $(NAME)

re:				fclean	all

done:
				printf "															 	\n"
				printf "$(GREEN)	$(NAME)							 			$(RESET)\n"
				printf "$(GREEN)                             _  _             _ $(RESET)\n"
				printf "$(GREEN)                            (_)| |           | |$(RESET)\n"
				printf "$(GREEN)  ____   ___   ____   ____   _ | |  ____   _ | |$(RESET)\n"
				printf "$(GREEN) / ___) / _ \ |    \ |  _ \ | || | / _  ) / || |$(RESET)\n"
				printf "$(GREEN)( (___ | |_| || | | || | | || || |( (/ / ( (_| |$(RESET)\n"
				printf "$(GREEN) \____) \___/ |_|_|_|| ||_/ |_||_| \____) \____|$(RESET)\n"
				printf "$(GREEN)                     |_|                        $(RESET)\n"
				printf "																\n"

.PHONY: 		all clean fclean re