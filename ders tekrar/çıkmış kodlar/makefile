# 1. Derleyici ve Bayraklar
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# 2. Rotalar (Ana dizindeki ortak bin klasörü)
MAIN_BIN_DIR = ../../bin

# =====================================================================
# 3. ZEKİ FİLTRELEME SİSTEMİ (Senin Koyduğun Kurallar)
# =====================================================================

# Sadece sonu '_main.cpp' ile biten ana programları bul
MAIN_SRCS   = $(wildcard *_main.cpp)

# Klasördeki TÜM .cpp dosyalarını bul, ama içinden 'MAIN_SRCS' olanları ÇIKART.
# Böylece geriye sadece bağımlılık (nesne/yardımcı) olan saf .cpp'ler kalır.
HELPER_SRCS = $(filter-out $(MAIN_SRCS), $(wildcard *.cpp))

# Ana programların isimlerinden '_main.cpp' kısmını atıp ana bin klasörüne bağla
BINS        = $(patsubst %_main.cpp, $(MAIN_BIN_DIR)/%, $(MAIN_SRCS))

# =====================================================================

# Varsayılan Kural
all: $(MAIN_BIN_DIR) $(BINS)

$(MAIN_BIN_DIR):
	mkdir -p $(MAIN_BIN_DIR)

# 4. DERLEME VE BAĞLAMA (Linkleme) KURALI
# Sadece _main.cpp hedeflenir ama $(HELPER_SRCS) yanına eklenerek beraber derlenir
$(MAIN_BIN_DIR)/%: %_main.cpp $(HELPER_SRCS)
	@echo "🛠️  Ana Program: $<"
	@if [ -n "$(HELPER_SRCS)" ]; then \
		echo "   🔗 Bağlanan Yardımcılar: $(HELPER_SRCS)"; \
	fi
	$(CXX) $(CXXFLAGS) $< $(HELPER_SRCS) -o $@

# 5. İnteraktif Çalıştırma Menüsü
run: all
	@echo "\n🎯 Bu klasörde birden fazla ana program mevcut. Hangisini ateşleyelim?"
	@for bin in $(BINS); do \
		echo "  👉 $$(basename $$bin)"; \
	done
	@echo "--------------------------------------"
	@read -p "Programın tam adını yazın: " secim; \
	if [ -f "$(MAIN_BIN_DIR)/$$secim" ]; then \
		echo "\n🚀 Başlatılıyor: $$secim\n--------------------------------------"; \
		"$(MAIN_BIN_DIR)/$$secim"; \
		echo "\n--------------------------------------\n🏁 Program Sonlandı."; \
	else \
		echo "\n❌ Hata: '$$secim' adında bir program bulunamadı!"; \
	fi

# 6. Temizlik
clean:
	@for bin in $(patsubst %_main.cpp, %, $(MAIN_SRCS)); do \
		rm -f $(MAIN_BIN_DIR)/$$bin; \
	done
	@echo "🧹 Bu klasörün binary'leri ana merkezden temizlendi."