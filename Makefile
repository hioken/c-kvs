CC      := gcc
CFLAGS  := -Wall -Wextra
INCDIR  := -Isrc

SRCDIR  := src
OBJDIR  := obj
BINDIR  := bin

TARGET  := $(BINDIR)/kvstore
SRCS    := $(wildcard $(SRCDIR)/*.c)
OBJS    := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all test prod release clean

# 引数なしのデフォルトはprodと同じ
all: prod

# パターン1: testモード (-DTESTMODE -g)
test: CFLAGS += -DTESTMODE -g
test: clean $(TARGET)
	./$(TARGET)

# パターン2: prodモード / 引数無し (-g)
prod: CFLAGS += -g
prod: $(TARGET)

# パターン3: releaseモード (-DNDEBUG でassert無効化等)
release: CFLAGS += -DNDEBUG
release: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(wildcard test/*.c) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)