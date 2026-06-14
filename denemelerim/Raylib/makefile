# 1. Rotalar ve Ayarlar
MAIN_BIN_DIR = ../../bin
BUILD_DIR = build
# CMakeLists.txt içindeki add_executable satırından program adını otomatik al
APP_NAME := $(shell grep -m1 -o 'add_executable([^ )]*' CMakeLists.txt | sed 's/^add_executable(//' | tr -d ')')

# Eğer otomatik bulamazsa, burada manuel adı ayarlayın:
ifneq ($(APP_NAME),)
else
APP_NAME := my_game # BURAYI PROJENİN ADINA GÖRE BİR KERE AYARLA:
endif

# 2. Varsayılan Kural
all:
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f "$(BUILD_DIR)/CMakeCache.txt" ]; then \
		echo "🛠️  İlk yapılandırma (CMake Configure)..."; \
		cd $(BUILD_DIR) && cmake ..; \
	else \
		echo "⚡ Yapılandırma zaten hazır, derlemeye geçiliyor..."; \
	fi
	@cd $(BUILD_DIR) && $(MAKE)

# 3. Çalıştırma Kuralı (Artık sormayacak!)
run: all
	@echo "\n🚀 Başlatılıyor: $(APP_NAME)"
	@if [ -f "$(BUILD_DIR)/$(APP_NAME)" ]; then \
		cp "$(BUILD_DIR)/$(APP_NAME)" "$(MAIN_BIN_DIR)/$(APP_NAME)"; \
		echo "--------------------------------------"; \
		"$(MAIN_BIN_DIR)/$(APP_NAME)"; \
		echo "\n--------------------------------------\n🏁 Program Sonlandı."; \
	else \
		echo "\n❌ Hata: '$(BUILD_DIR)/$(APP_NAME)' bulunamadı!"; \
		echo "İpucu: Makefile içindeki APP_NAME değişkenini CMakeLists.txt ile eşleştir."; \
	fi

clean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Build klasörü temizlendi."