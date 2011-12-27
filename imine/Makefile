#CC = /path/to/my/toolchain/bin/arm-apple-darwin9-gcc
CC=arm-apple-darwin9-gcc
CXX=arm-apple-darwin9-g++
LD = $(CC)

# Here comes your IP of the ipod/iphone.
# If you do a "make bundle" we build our
# application and copy via ssh the files to
# your ipod/iphone and sign them there.
#
# please make sure you have 'ldid' installed on your
# phone: 
#   apt-get install ldid
#

# iPhone SDK path
IPHONESDK = /opt/iphone/toolchain/sys
# include path (SDK)
INCPATH += -I"$(IPHONESDK)/usr/include"
# include path (gcc)
INCPATH += -I"$(IPHONESDK)/usr/include/gcc/darwin/4.2"

CFLAGS  += $(INCPATH) \

LDFLAGS=        -lobjc \
				-lz\
				-lstdc++.6\
                -bind_at_load \
                -framework Foundation \
                -framework CoreFoundation \
                -framework UIKit \
                -framework CoreGraphics \
                -w


LDFLAGS += -L"$(IPHONESDK)/usr/lib"
LDFLAGS += -F"$(IPHONESDK)/System/Library/Frameworks"
LDFLAGS += -F"$(IPHONESDK)/System/Library/PrivateFrameworks"

CFLAGS += -F"$(IPHONESDK)/System/Library/Frameworks"
CFLAGS += -F"$(IPHONESDK)/System/Library/PrivateFrameworks"

SRC=./src
IMG=./img
IP=192.168.1.100
APP=imine

OBJS     = $(patsubst %.m,%.o,$(wildcard $(SRC)/*.m))
OBJS    += $(patsubst %.cpp,%.o,$(wildcard $(SRC)/*.cpp))
OBJS    += $(patsubst %.mm,%.o,$(wildcard $(SRC)/*.mm))


all:	$(APP) bundle

$(APP): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.m
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o:    %.mm
	$(CXX) -c -x objective-c++ $(CFLAGS) $< -o $@

%.o:    %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

bundle: $(APP)
	@mkdir -p imine.app
	@cp imine imine.app/imine_
	@cp ${IMG}/* imine.app
	@cp Info.plist imine.app
	@cp imine.sh imine.app/imine


deploy: clean bundle
	@ssh root@$(IP) "cd /Applications/imine.app && rm -R * || echo 'not found' "
	@scp -rp imine.app root@$(IP):/Applications
	@ssh root@$(IP) "chmod -R 755 /Applications/imine.app ; cd /Applications/imine.app ; ldid -S imine_; killall SpringBoard"

clean:
	@rm -f $(SRC)/*.o
	@rm -Rf imine.app
