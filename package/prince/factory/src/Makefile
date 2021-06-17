TARGET	= factory

DIR		= . system/timefd fileutils/inotify multiplexing/epoll
INC		= -I. -I./system/timefd -I./fileutils/inotify -I./multiplexing/epoll
CFLAGS	= -Wall
LDFLAGS = 

OBJPATH	= .

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cc))

OBJS	= $(patsubst %.cc,%.o,$(FILES))

all:$(OBJS) $(TARGET)

$(OBJS):%.o:%.cc
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET):$(OBJPATH)
	$(CXX)  $(OBJPATH)/*.o -o $@ $(LDFLAGS)

install:
	cp $(TARGET) $(DESTDIR)/usr/bin/

clean:
	-rm *.o
	-rm $(TARGET)
	
