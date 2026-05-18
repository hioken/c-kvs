CC      := gcc
INCDIR  := -Isrc

SRCDIR  := src
BINDIR  := bin
TESTDIR := test

# =========================================================
# 🌟 1. ビルドモードの設定 (MODE変数で切り替え)
# =========================================================
MODE ?= debug

ifeq ($(MODE), release)
	# --- リリースモード (-DNDEBUGあり, Valgrind無効) ---
	CFLAGS_MODE  := -Wall -Wextra -O2 -DNDEBUG
	VALGRIND_CMD := 
	BASE_OBJDIR  := obj/release
else ifeq ($(MODE), prod)
	# --- 本番想定動作確認モード (-DNDEBUGあり, Valgrind有効) ---
	CFLAGS_MODE  := -Wall -Wextra -O2 -DNDEBUG
	VALGRIND_CMD := valgrind --leak-check=full --track-origins=yes
	BASE_OBJDIR  := obj/prod
else
	# --- デバッグモード (デフォルト: -DNDEBUGなし, Valgrind有効) ---
	MODE         := debug
	CFLAGS_MODE  := -Wall -Wextra -g -O0
	VALGRIND_CMD := valgrind --leak-check=full --track-origins=yes
	BASE_OBJDIR  := obj/debug
endif

# =========================================================
# 🌟 2. テストモードの判定 (`make test` 実行時の特別処理)
# =========================================================
# 実行されたコマンドライン引数に「test」が含まれているかチェック
ifneq (,$(filter test,$(MAKECMDGOALS)))
	# testモード: staticを無効化するマクロを追加
	CFLAGS_MODE += -DNOSTATIC
	# テスト時はオブジェクトファイル(.o)を test ディレクトリに出力する
	OBJDIR      := $(TESTDIR)
else
	# 通常ビルド
	OBJDIR      := $(BASE_OBJDIR)
endif

CFLAGS := $(CFLAGS_MODE)

# =========================================================
# 3. ターゲットとファイル定義
# =========================================================
TARGET      := $(BINDIR)/kvstore
# テストの実行ファイルも test ディレクトリに出力
TEST_TARGET := $(TESTDIR)/test_runner

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# main.o を除いたオブジェクト（テストのリンク用）
CORE_OBJS := $(filter-out $(OBJDIR)/main.o,$(OBJS))

.PHONY: all clean test

all: $(TARGET)

# 本番バイナリのビルド
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# オブジェクトファイルのビルド
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

# ディレクトリ作成
$(BINDIR) $(OBJDIR):
	mkdir -p $@

# テストバイナリのビルドと自動検証、および .o のクリーンアップ
test: $(CORE_OBJS) | $(TESTDIR)
	$(CC) $(CFLAGS) $(INCDIR) $(TESTDIR)/test_main.c $(CORE_OBJS) -o $(TEST_TARGET)
	@echo "=== Cleaning up test object files ==="
	rm -f $(CORE_OBJS)
	@echo "=== Running Tests [MODE=$(MODE)] ==="
	$(VALGRIND_CMD) $(TEST_TARGET)

# クリーンアップ
clean:
	rm -rf obj $(BINDIR) $(TEST_TARGET)