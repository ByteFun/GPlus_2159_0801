TOPDIR := ../../../
include $(TOPDIR)sdk/build/core/config.mak


GOAL     = sysinit


CFLAGS += -I$(SDK_DIR)/include
CFLAGS += -I$(PRODUCT_DIR)
CFLAGS += -I.
CFLAGS += -g
CFLAGS += -L$(SDK_DIR)/lib -licver

OBJS := \
	module_operation.o \
	sysinit.o

LIBS := \
	$(SDK_DIR)/lib/libdiag.a \
	$(SDK_DIR)/lib/libboard.a


.PHONY: all
all: $(GOAL)
	$(MSG_SPLIT_LINE)
	@$(MKDIR) -p $(PRODUCT_DIR)/rootfs
	@$(CP) -u $(GOAL) $(PRODUCT_DIR)/rootfs/$(GOAL)


$(GOAL): $(OBJS) $(LIBS)
	$(MSG_SPLIT_LINE)
	$(MSG_LINKING)
	$(TARGET_CC) -o $@ $(OBJS) $(LIBS) $(CFLAGS)
	$(MSG_GOAL_OK)


.PHONY: clean
clean:
	$(MSG_SPLIT_LINE)
	$(RM) -rf $(GOAL) $(OBJS) $(OBJS:.o=.d)
	$(RM) -f $(PRODUCT_DIR)/rootfs/$(GOAL)


-include $(OBJS:.o=.d)
