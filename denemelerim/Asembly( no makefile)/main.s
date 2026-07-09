.global _main           // Programın başlangıç noktası
.align 4                // 4 bayt hizalama zorunlu

_main:
    // 1. ADIM: Ekrana harf basma emri (write)
    mov w0, #1          // X0 = 1 (Terminal / Standart Çıktı)
    adr x1, harf        // X1 = 'harf' etiketinin adresi (Artık hemen aşağıda!)
    mov x2, #2          // X2 = 2 (Hem 'A' hem '\n' basacağımız için 2 bayt yapıyoruz)
    
    movz x16, #0x200, lsl #16  // Üst 16-bit'e 0x0200 yaz
    movk x16, #0x0004          // Alt kısma 0x0004 ekle (Toplam: 0x2000004 - write)
    svc #0                     // Çekirdeği tetikle!

    // 2. ADIM: Programı kapatma emri (exit)
    mov x0, #0          // X0 = 0 (Başarılı çıkış kodu)
    movz x16, #0x200, lsl #16  // Üst kısma 0x0200 yaz
    movk x16, #0x0001          // Alt kısma 0x0001 ekle (Toplam: 0x2000001 - exit)
    svc #0                     // Çekirdeği tetikle!

// VERİYİ DOĞRUDAN KODUN İÇİNE ALIYORUZ
.align 2                // Metin verisi için hizalama
harf:
    .ascii "A\n"        // Ekrana basılacak harf ve alt satır karakteri