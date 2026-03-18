# OpenTS Makefile
# www.coin-or.org
# SVN: https://projects.coin-or.org/svn/Ots/trunk
# Command: make
# Output: OpenTS.jar


# Programs and flags
CC	= javac
CP	= cp
JAR	= jar
RM	= rm
RMFLAGS	= -rf
CFLAGS	= -d $(BUILD)

# Directories
BUILD	= build
PKG	= org/coinor/opents

# Final Output
PROG	= OpenTS.jar


# Default entrance to makefile.
all: $(PROG) 


# Make jar file
$(PROG) : classes images $(BUILD)/manifest
	$(JAR) cfm $@ $(BUILD)/manifest -C $(BUILD) $(PKG)


# Target directory
$(BUILD):
	mkdir $(BUILD)


# Build all classes. Needs target directory.
classes: $(BUILD)
	$(CC) $(CFLAGS) $(PKG)/*.java


# Copy images for JavaBean. Needs target directory.
images: $(BUILD)
	$(CP) $(PKG)/*.gif $(BUILD)/$(PKG)/


# Build manifest file. Needs target directory.
$(BUILD)/manifest: $(BUILD)
	echo Manifest-Version: 1.0 >  $@
	echo Main-Class: org/coinor/opents/Main >>  $@




# Delete build directory and final output.
clean:
	$(RM) $(RMFLAGS) $(BUILD)
	$(RM) $(RMFLAGS) $(PROG)




