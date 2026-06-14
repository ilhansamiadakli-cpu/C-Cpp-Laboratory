# 1. Rotalar
MAIN_BIN_DIR = ../../bin
BUILD_DIR = build

# 2. Dinamik Hedef Bulucu (Sadece _main.cpp isimlerini tarar)
MAIN_SRCS = $(wildcard *_main.cpp)
TARGETS   = $(patsubst %_main.cpp, %, $(MAIN_SRCS))

# 3. Varsayılan Kural (CMake Derleme)
all:
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f "$(BUILD_DIR)/CMakeCache.txt" ]; then \
		echo "🛠️  İlk yapılandırma (CMake Configure)..."; \
		cd $(BUILD_DIR) && cmake ..; \
	fi
	@cd $(BUILD_DIR) && $(MAKE)

# 4. İnteraktif Menülü Çalıştırma Kuralı
run: all
	@echo "\n🎯 Bu CMake projesinde birden fazla program mevcut. Hangisini ateşleyelim?"
	@for target in $(TARGETS); do \
		echo "  👉 $$target"; \
	done
	@echo "--------------------------------------"
	@read -p "Programın tam adını yazın: " secim; \
	if [ -f "$(BUILD_DIR)/$$secim" ]; then \
		cp "$(BUILD_DIR)/$$secim" "$(MAIN_BIN_DIR)/$$secim"; \
		echo "\n🚀 Başlatılıyor: $$secim\n--------------------------------------"; \
		"$(MAIN_BIN_DIR)/$$secim"; \
		echo "\n--------------------------------------\n🏁 Program Sonlandı."; \
	else \
		echo "\n❌ Hata: '$$secim' adında bir program bulunamadı!"; \
	fi

clean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Build klasörü temizlendi."