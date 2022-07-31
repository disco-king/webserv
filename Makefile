NAME = serv

CONF_SRCS = ./config/Config.cpp ./config/ConfigUtils.cpp \
			./config/RequestConfig.cpp ./config/ServerConfig.cpp

SRCS = Server.cpp Request.cpp Listener.cpp \
		file_utils.cpp string_utils.cpp main.cpp \
		Response.cpp

ODIR = objs
_OBJS = $(SRCS:.cpp=.o) $(CONF_SRCS:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

HEADERS = ./config/Config.hpp ./config/ConfigUtils.hpp \
			./config/RequestConfig.hpp ./config/ServerConfig.hpp \
			Server.hpp Request.hpp Listener.hpp \
			file_utils.hpp string_utils.hpp

RM = rm -rf

all: $(NAME)

$(NAME): $(ODIR) $(OBJS)
	g++ $(OBJS) -o serv

$(ODIR):
	mkdir objs
	mkdir objs/config

$(ODIR)/%.o: %.cpp $(HEADERS)
	g++ -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)