// macOS ARM64 (Apple Silicon) Hello World

.global _main             // Programın giriş noktasını belirtiyoruz
.align 2                  // Apple Silicon kodun 4-byte hizalı olmasını ister

_main:
    // 1. Ekrana Yazdırma: write(1, "Hello World\n", 13)
    mov x0, #1            // İlk parametre (x0): 1 = Standart çıktı (stdout)
    adrp x1, mesaj@PAGE        // Mesajın bulunduğu 4KB'lık sayfanın adresini yükle
    add  x1, x1, mesaj@PAGEOFF // O sayfa içindeki tam konumunu (offset) ekle      
    mov x2, #13           // Üçüncü parametre (x2): Mesajın uzunluğu (13 karakter)
    mov x16, #4           // x16 kaydedicisine sistem çağrısı numarasını koy: 4 = write
    svc #0x80             // Çekirdeğe (kernel) seslen: "Bu işlemi yap!"

    // 2. Programı Kapatma: exit(0)
    mov x0, #0            // İlk parametre (x0): 0 = Hata yok (success)
    mov x16, #1           // x16 kaydedicisine: 1 = exit
    svc #0x80             // Çekirdeğe seslen: "Benim işim bitti."

.data
mesaj: .ascii "Hello World\n"