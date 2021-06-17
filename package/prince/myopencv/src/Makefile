TARGET	= myopencv

DIR		= . 
INC		= -I. -I./include -I/usr/include
CFLAGS	= -O2 -Wall
LDFLAGS = -lpthread -ljpeg -lopencv_core -lopencv_photo -lopencv_imgproc

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
