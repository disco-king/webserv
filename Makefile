NAME = serv

CFLAGS = -Wall -Wextra -Werror

CONF_DIR = config
_CONF_SRCS = Config.cpp ConfigUtils.cpp \
			RequestConfig.cpp ServerConfig.cpp
CONF_SRCS = $(patsubst %,$(CONF_DIR)/%,$(_CONF_SRCS))

RESP_DIR = response
_RESP_SRCS = Response.cpp CGIResponse.cpp
RESP_SRCS = $(patsubst %,$(RESP_DIR)/%,$(_RESP_SRCS))

REQ_DIR = request
_REQ_SRCS = Request.cpp
REQ_SRCS = $(patsubst %,$(REQ_DIR)/%,$(_REQ_SRCS))

UTILS_DIR = utils
_UTILS_SRCS = string_utils.cpp file_utils.cpp
UTILS_SRCS = $(patsubst %,$(UTILS_DIR)/%,$(_UTILS_SRCS))

SERV_DIR = server
_SERV_SRCS = Server.cpp Interface.cpp main.cpp
SERV_SRCS = $(patsubst %,$(SERV_DIR)/%,$(_SERV_SRCS))

SRCS = $(CONF_SRCS) $(RESP_SRCS) $(REQ_SRCS) $(UTILS_SRCS) $(SERV_SRCS)

ODIR = objs
_OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

HEADERS = $(CONF_DIR)/*.hpp $(RESP_DIR)/*.hpp $(REQ_DIR)/*.hpp $(UTILS_DIR)/*.hpp $(SERV_DIR)/*.hpp

RM = rm -rf

all: $(NAME)

$(NAME): $(ODIR) $(OBJS)
	g++  $(OBJS) -o serv

$(ODIR):
	mkdir objs
	mkdir objs/$(CONF_DIR)
	mkdir objs/$(SERV_DIR)
	mkdir objs/$(RESP_DIR)
	mkdir objs/$(REQ_DIR)
	mkdir objs/$(UTILS_DIR)
	echo $(HEADERS)

$(ODIR)/%.o: %.cpp $(HEADERS)
	g++ $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(ODIR)

re: fclean $(NAME)