CC      := gcc
CFLAGS  := -Wall -Wextra -O2
INCDIR  := -Isrc

SRCDIR  := src
OBJDIR  := obj
BINDIR  := bin
TESTDIR := test

TARGET      := $(BINDIR)/kvstore
TEST_TARGET := $(TESTDIR)/test_runner

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all release prod test clean

# デバッグモード (デフォルト)
all: CFLAGS += -g
all: $(TARGET)

# リリース
release: CFLAGS += -DNDEBUG
release: $(TARGET)

# 本番想定動作確認
prod: CFLAGS += -DNDEBUG
prod: $(TARGET)

# ターゲット共通のビルドルール
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(BINDIR) $(OBJDIR) $(TESTDIR):
	mkdir -p $@

# testモード (オブジェクトと実行ファイルをtestディレクトリに出力)
TEST_OBJS      := $(patsubst $(SRCDIR)/%.c,$(TESTDIR)/%.o,$(SRCS))
TEST_CORE_OBJS := $(filter-out $(TESTDIR)/main.o,$(TEST_OBJS))

test: CFLAGS += -DNOSTATIC -g
test: $(TEST_CORE_OBJS) $(TESTDIR)/test_main.o | $(TESTDIR)
	$(CC) $(CFLAGS) $(INCDIR) $(TESTDIR)/test_main.o $(TEST_CORE_OBJS) -o $(TEST_TARGET)
	rm -f $(TEST_CORE_OBJS) $(TESTDIR)/test_main.o
	./$(TEST_TARGET)

$(TESTDIR)/%.o: $(SRCDIR)/%.c | $(TESTDIR)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

$(TESTDIR)/test_main.o: $(TESTDIR)/test_main.c | $(TESTDIR)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TEST_TARGET)