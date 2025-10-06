Olay güdümlü (event-driven) , gerçek zamanlı bir işletim sistemi olan Contiki-NG içerisinde process seçimi, parent process-lerin tetiklemesi sonucunda koşma işlemine alınmaktadır. Bu yüzden sistemde hali hazırda bir process tarifeleme (scheduling) yapılmamaktadır. process isimli yapının (struct) içeriğine, elemanlarına ve kullanımına process.c ve process.h kaynak dosyalarından ulaşabilirsiniz (contiki-ng/os/sys/ dizini altındadır.)
process yapısının içerisinde luck isimli öncelik bilgisi barındıran bir değişken atamalısınız. Atanan bu değişkenin max. değeri 1, min. değeri 128 olsun. Ödev kapsamında, process-lere öncelik atama veya atanan önceliğe göre kuyruktan çekme işlemi process.c içerisinde gerçekleştirilmesi mantıklı olacaktır.

Kernel kaynak dosyalarının yolları:
contiki-ng/os/sys/process.h
contiki-ng/os/sys/process.c


Uygulama firmware yolu: (Kernel-a uygulama katmanından gelen process-leri görmek için)
contiki-ng/examples/os/hardworker.c //firmware kodu
contiki-ng/examples/os/simulation-of-process-scheduling.csc //Cooja için simülasyon dosyası

kaynaklarını inceleyiniz. process.c dosyasında call_process() fonksiyonunun process-lerin isim bilgilerine nasıl eriştiğini gözlemleyip; bu isim bilgilerine göre öncelik ataması yapabilirsiniz. Bu fonksiyonun ekrana bastığı değerleri görmek için Cooja simülasyonunda “Mote Output” penceresinde alttaki filtre bölümüne "calling process" etiketini yazıp, ENTER tuşuna basınız.


PROCESS(led_process, "LED Toggle P.ID:03");
PROCESS(udp_process, "Simulated UDP P.ID:34");
PROCESS(sensor_process, "Sensor Event P.ID:06");
PROCESS(dummy_printer_process, "Dummy Printer P.ID:61");
AUTOSTART_PROCESSES(&led_process,&udp_process,&sensor_process,&dummy_printer_proces);

Kernel-daki process.h içerisinde process isimlerinin nasıl alındığını 2.parametreden görebilirsiniz: 308.satır

#define PROCESS(name, strname)           \
 PROCESS_THREAD(name, ev, data);         \
 struct process name = { NULL, strname,       \
                         process_thread_##name, {0}, 0, 0 }
#endif

Dolayısıyla çekilen bu isme göre koşmakta olan process-lerin isimlerini bastırma işlemi process.c de call_process() metodunda nasıl yapıldıysa; benzer şekilde isme göre öncelik atama veya koşmak için kuyruktan çekerken de isme göre process seçimi yapabilirsiniz. 
    
Öncelikleri şu sıralamada belirleyebilirsiniz: (En yüksekten aşağı)
Event timer //os timer processi
LED Toggle //yanan ledleri görmek için Cooja düğümlere sağ tıkla, LED seçeneğini açabilirsiniz.
Dummy Printer 
CC2420 driver //radyo haberleşmesi
