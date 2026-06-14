# Derleyici ve Bayraklar
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Klasör Yapısı
SRC_DIR  = denemelerim #bu kısmı kendi klasör yapınıza göre değiştirin
BIN_DIR  = bin

.PHONY: all clean push help

# 1. Dinamik Derleme ve Çalıştırma Mantığı (Beyin Kısmı)
all: | $(BIN_DIR)
	@if [ -n "$(run)" ]; then \
		FOLDER="$(run)"; \
		SHOULD_RUN=1; \
	elif [ -n "$(dir)" ]; then \
		FOLDER="$(dir)"; \
		SHOULD_RUN=0; \
	else \
		echo "❌ Hata: Klasör seçmediniz!"; \
		echo "👉 Sadece derlemek için: make dir=\"<klasör_adı>\""; \
		echo "👉 Derleyip anında çalıştırmak için: make run=\"<klasör_adı>\""; \
		echo "\n📁 Mevcut laboratuvar klasörleriniz:"; \
		ls -1 "$(SRC_DIR)"; \
		exit 1; \
	fi; \
	if [ ! -d "$(SRC_DIR)/$$FOLDER" ]; then \
		echo "❌ Hata: '$(SRC_DIR)/$$FOLDER' adında bir klasör bulunamadı."; \
		exit 1; \
	fi; \
	if [ -f "$(SRC_DIR)/$$FOLDER/Makefile" ] || [ -f "$(SRC_DIR)/$$FOLDER/makefile" ]; then \
		echo "🔄 '$$FOLDER' içinde alt Makefile bulundu! İşlem oraya devrediliyor..."; \
		if [ "$$SHOULD_RUN" -eq 1 ]; then \
			$(MAKE) -C "$(SRC_DIR)/$$FOLDER" run; \
		else \
			$(MAKE) -C "$(SRC_DIR)/$$FOLDER"; \
		fi; \
	else \
		echo "🛠️ '$(SRC_DIR)/$$FOLDER' içindeki kodlar derleniyor..."; \
		$(CXX) $(CXXFLAGS) "$(SRC_DIR)/$$FOLDER"/*.cpp -o "$(BIN_DIR)/$${FOLDER}_run"; \
		if [ $$? -eq 0 ]; then \
			echo "✅ Derleme başarılı! Çıktı: $(BIN_DIR)/$${FOLDER}_run"; \
			if [ "$$SHOULD_RUN" -eq 1 ]; then \
				echo "🚀 Program Başlatılıyor...\n--------------------------------------"; \
				"./$(BIN_DIR)/$${FOLDER}_run"; \
				echo "\n--------------------------------------\n🏁 Program Sonlandı."; \
			fi; \
		else \
			echo "❌ Derleme başarısız oldu!"; \
			exit 1; \
		fi; \
	fi

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 2. Temizlik
clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Tüm derlenmiş binary'ler (bin/ klasörü) temizlendi."

# 3. Git Otomasyonu
push:
	@echo "🚀 Git operasyonu başlatılıyor..."
	git add .
	@read -p "Commit mesajını gir yazılım lideri: " msg; \
	git commit -m "$$msg"
	git push origin main
	@echo "🎉 Değişiklikler GitHub'a uçuruldu!"
help:
	@echo "================================================================"
	@echo "🛠️  C-Cpp-Laboratory Merkezi Otomasyon Sistemi (Vmake Öncesi) 🛠️"
	@echo "================================================================"
	@echo ""
	@echo "KULLANIM KOMUTLARI:"
	@echo "  make help                  : Bu yardım menüsünü gösterir."
	@echo "  make dir=\"klasör_adı\"    : Belirtilen klasörü derler ve bin/ içine atar."
	@echo "  make run=\"klasör_adı\"    : Belirtilen klasörü derler ve hemen çalıştırır."
	@echo "                               (Klasörde birden fazla program varsa interaktif menü sunar)"
	@echo "  make clean                 : Tüm derlenmiş binary'leri (bin/ klasörünü) siler."
	@echo "  make push                  : Kodları otomatik olarak GitHub'a gönderir."
	@echo ""
	@echo "SİSTEM TASARIM KURALLARI (Gelecekteki İlhan'a Notlar):"
	@echo "  1. Ana Programlar          : İçinde 'int main()' olan ve bağımsız çalışacak"
	@echo "                               dosyaların adı '_main.cpp' ile BİTMELİDİR."
	@echo "                               (Örn: custom_allocator_main.cpp)"
	@echo "  2. Yardımcı Dosyalar       : Düz '.cpp' uzantılı dosyalar nesne/yardımcı"
	@echo "                               kabul edilir ve '_main.cpp' olan komutanlara"
	@echo "                               otomatik olarak bağlanır."
	@echo "  3. Çıktılar (Binaries)     : Derlenen tüm çalıştırılabilir dosyalar projenin"
	@echo "                               en dışındaki ortak 'bin/' klasöründe toplanır."
	@echo "  4. Alt Makefile'lar        : Bir alt klasörün içinde özel bir Makefile varsa,"
	@echo "                               merkezi sistem sorumluluğu otomatik ona devreder."
	@echo "================================================================"