TARGET	= myjpeg

DIR		= . 
INC		= -I. 
CFLAGS	= -Wall
LDFLAGS = -lpthread -ljpeg

OBJPATH	= .

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))

all:$(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET):$(OBJPATH)
	$(CXX)  $(OBJPATH)/*.o -o $@ $(LDFLAGS)

install:
	cp $(TARGET) $(DESTDIR)/usr/bin/

clean:
	-rm *.o
	-rm $(TARGET)
	
