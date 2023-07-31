ifeq ($(TOP), )
    TOP = $(realpath ../..)
endif

include $(TOP)/top.mk

OBJS = conhash conhash_inter conhash_util util_rbtree md5

sub_dirs = chash

INCLUDES = -I.
TARGET_OBJS_DIR = $(TOP)/.conhashtmp

vpath % $(TARGET_OBJS_DIR)

.PHONY: all clean

LIB_BASE_CLASS = libconhash.a

all : $(OBJS:%=%.o)
	@echo create static lib $(LIB_BASE_CLASS) ...
	@ar r $(TARGET_OBJS_DIR)/$(LIB_BASE_CLASS)  $(foreach obj, $(OBJS:%=%.o), $(TARGET_OBJS_DIR)/$(obj))
	@echo copy $(TARGET_OBJS_DIR)/$(LIB_BASE_CLASS) to $(VODDS_LIBS)/$(LIB_BASE_CLASS)
	@mv $(TARGET_OBJS_DIR)/$(LIB_BASE_CLASS) $(VODDS_LIBS)/$(LIB_BASE_CLASS)

$(OBJS:%=%.o) : $(OBJS:%=%.cpp)
	@echo compiling $(@:%.o=%.cpp)
	mkdir -p $(TARGET_OBJS_DIR)
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $(TARGET_OBJS_DIR)/$(@) $(@:%.o=%.cpp)

.PHONY:clean
clean:
	@rm -f $(foreach obj, $(OBJS:%=%.o), $(TARGET_OBJS_DIR)/$(obj))
	@rm -f $(VODDS_LIBS)/$(LIB_BASE_CLASS)
