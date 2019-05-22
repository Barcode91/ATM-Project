#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<locale.h>
#include<string.h>
#define temizle system("cls");
//////////////////FONKSIYON PROTOTIPLERI//////////////////////////
int hspkont(); //M�steri hesabini ve sifresini kontrol edecek.
char havale(int);//m�steri keyini girdi olarak alir.
char adminpanel(void);
char musteripanel(void);
char bakiye(int);//m�steri keyini girdi olarak alir.
char para_cek_yatir(int key, int islemkey);//key g�nderilmesi gerek
char hesapolustur(void);
char faturaode(int);//m�steri keyini girdi olarak alir.
char hesapduzelt (int);//m�steri keyini girdi olarak alir.
int hspkeytespit(int);//kullanicidan alinan hesap numarasini kntrol edecek ve indis g�ndercek.
void dosyaislem(int key);//dosya adini musteri keyine g�re degistirecek.
//Kullanici bilgilerini tutacak kisi isminde struct tanimlanir.
typedef struct kisi{
	int hesapno;
	short sifre;
	char ad[20];
	char soyad[20];
	char adres[30];
	float bakiye;
};
kisi musteri[100]; //100 tane kullanici verisini tutacak struct tipinde dizi tanimlanir.  
//GLOBAL DEGISKENLER
const short islem1=1;//para cekme islem kodu
const short islem2=2;//para yatirma islem kodu 
char dosyadi[50]="C:\\ATM Project\\";//dosya islemlerinde dosya adini tutacak global degisken

/////////////////////////////////////////////MAIN /////////////////////////////////////////////////////
main(){
	char secim, secim2, sonuc;
	int password , adminkey=1234;//Bankaci Sifresi
	int key;//M�steri indis degerini tutacak degisken
	setlocale(LC_ALL, "Turkish");
	menu1 :
	temizle;
	printf("L�TFEN KULLANICI TIPINIZI BELIRLEYINIZ. \n[1]BANKACI\n[2] M�STERI\n");
	secim=getch();//kullanicidan tercihi alinir.
		switch (secim){//kontrol edilir.
		case '1' :{//1'e esitse admin sifresi istenir
			menu2 :
			temizle;
			printf("Admin Sifresini giriniz:");
			scanf("%d", &password);
			if (adminkey==password){//Alinan sifre dogru ise admin paneline gecilir
				sonuc=adminpanel();
				if(sonuc=='3') goto menu1;
			} else {//sifre yanlis ise programdan cikilir.
				printf("Hatali Sifre Girdiniz.\n[1] Tekrar Dene\n[2] Iptal");
				secim2=getch();
				if(secim2=='1') goto menu2;
				else goto menu1;
					}
			break;
		}
		case '2' :{//2'e esitse M�steri islemleri icin devm edilir.
			//m�steri sifre ve hesap numarasi alacak ve isteyecek fonk.
			temizle;
			//musteri panele gecilir.
			sonuc=musteripanel();
			if (sonuc=='3') goto menu1;
			break;
		}
		default :{// 1 ve 2 den baska bir giris yapilirsa ikaz edilir ve d�ng� basa d�ner.
			printf("HATALI SECIM YAPTINIZ...");
			getch();
			temizle;
			goto menu1;
			break;
		}
	}
}
/////////////////////////////////////////////////ADMIN PANELI////////////////////////////////////////////////////////////
char adminpanel(void){
	int musterino, key=-1;//para cekme ve yatirma islemi icin
	char secim1, sonuc;
	while(1) {
		temizle;
		printf("\t**** Admin ****\n[1] Kullanici Hesap Olustur\n[2] Kullanici Hesap D�zelt\n[3] Para �ekme\n[4] Para Yatirma\n");
		secim1=getch();
		//Kullanici Hesabi Olusturma
		switch (secim1){
			case '1' : {
				sonuc=hesapolustur();//Kullanici hesabi olusturacak fonksiyon
				break;
			}
			case '2' : {
				//kullanici hesabi d�zeltme fonk
				while (1) {//girilen m�steri numarasi dogru mu kontrol edilir.
					printf("Islem Yapmak Isteginiz Musteri Numarasini Giriniz: ");
					scanf("%d",&musterino);//Adminden m�steri numarasi girmesi beklenir.
					key=hspkeytespit(musterino);//hesap dogru ise indisi degil ise 101 d�nd�r�r.
					if(key==101){//key 101 ise tekrar m�steri numarasi istenir.
						printf("Hatali M�steri Numarasi Girdiniz!!!\n");
					} else {break;}//key 101 den farkli ise d�ng�den cikilir
				}//Musteri numarasi dogro ise para cekme fonksiyonu cagirilir.
				sonuc=hesapduzelt(key);//hesap d�zeltme fonk
				if (sonuc=='3') return '3' ; //�st men�ye gecilir(Bankaci-M�steri Ayrimina)
				break;
			}
			//Para Cekme
			case '3' : {//Para cekme ve yatirma islemi icin islem yap�lacak m�steri numarasi girilir.
				while (1) {//girilen m�steri numarasi dogru mu kontrol edilir.
					printf("Islem Yapmak Isteginiz Musteri Numarasini Giriniz: ");
					scanf("%d",&musterino);//Adminden m�steri numarasi girmesi beklenir.
					key=hspkeytespit(musterino);//hesap dogru ise indisi degil ise 101 d�nd�r�r.
					if(key==101){//key 101 ise tekrar m�steri numarasi istenir.
						printf("Hatali M�steri Numarasi Girdiniz!!!\n");
					} else {break;}//key 101 den farkli ise d�ng�den cikilir
				}//Musteri numarasi dogro ise para cekme fonksiyonu cagirilir.
				sonuc=para_cek_yatir(key, islem1);//paracekme fonk
				if (sonuc=='3') return '3' ; //�st men�ye gecilir(Bankaci-M�steri Ayrimina)
				break;
			}
			//Para Yatirma
			case '4' : {//Musteri numarasi dogro ise para cekme fonksiyonu cagirilir.
				while (1) {//girilen m�steri numarasi dogru mu kontrol edilir.
					printf("Islem Yapmak Isteginiz Musteri Numarasini Giriniz: ");
					scanf("%d",&musterino);//Adminden m�steri numarasi girmesi beklenir.
					key=hspkeytespit(musterino);//hesap dogru ise indisi degil ise 101 d�nd�r�r.
					if(key==101){//key 101 ise tekrar m�steri numarasi istenir.
						printf("Hatali M�steri Numarasi Girdiniz!!!\n");
					} else {break;}//key 101 den farkli ise d�ng�den cikilir
				}
				sonuc=para_cek_yatir(key, islem2);//paracekme fonk
				if (sonuc=='3') return '3' ; //�st men�ye gecilir(Bankaci-M�steri Ayrimina)
				break;
			}
			default :{
				system("cls");
				printf("**** Admin ****\n[1] Kullanici Hesap Olustur\n[2] Kullanici Hesap D�zelt\n[3] Para �ekme\n[4] Para Yatirma\n");
				scanf("%d", &secim1);
				break;
			}
		}
	}
}
////////////////////////////////////////////////////MUSTERI PANELI////////////////////////////////////////////////////
char musteripanel(void) {
	int i, key;
	char tercih, sonuc;
	key=hspkont();//M�sterinin girdigi m�seri numarasi ve sifre kontrol edilir.
	if(key==101){//101 indisi d�nerse ana men�ye d�n�l�r.(ADmin-M�steri Ayrimina d�n�l�r)
		return '3';
	}
	menu1:
	temizle;
	printf("[1] Para Yatirma\t[4] Fatura Odeme\n");
	printf("[2] Para Cekme\t\t[5] Bakiye Goruntuleme\n");
	printf("[3] Havale\n");
	tercih=getch();
	switch(tercih){
		sonuc='0';
		case '1':{//para yatirma
			sonuc=para_cek_yatir(key, islem2); 
			break;
		}
		case '2':{//para cekme
			sonuc=para_cek_yatir(key, islem1);
			break;
		}
		case '3':{//havale cagir
			sonuc=havale(key);
			break;
		}
		case '4':{//Fatura �deme 
			sonuc=faturaode(key);
			break;
		}
		case '5':{//Bakiye g�r�nt�leme
			sonuc=bakiye(key);
			break;
		}
	}
	//T�m fonksiyonlar char tipinde deger d�nd�r�r. 
	if(sonuc!='3'){goto menu1;}//Deger '3' ise Admin-m�steri ayrimina ge�ilir.
}
//////////////////////////////////////////////////HESAP OLUSTURMA///////////////////////////////////////////////////////
char hesapolustur(){
	int i,hesapno, sifre;
	char tercih;
	menu1:
	temizle;
	for(i=0;musteri[i].hesapno!=0;i++);//Icinde kayit olmayan siradaki musteri[] dizisi indisi tespit edilir. 
	printf("  8 haneli Musteri Hizmet No\t[1] Sil\n\tGiriniz.\t\t[2] Kaydet\n");
	printf("\t________\t\t[3] Geri\r\t");
	scanf("%d", &hesapno);//Admin tarafindan hesabin numarasi tanimlanir.
	tercih=getch();
	if(tercih=='1') goto menu1;//Sil secenegi
	if(tercih=='2') {musteri[i].hesapno=hesapno;}//Kaydet secenegi
	if(tercih=='3') return '3';//Geri secenegi Admin paneline d�n�s.
	menu2:
	temizle;
	printf("4 haneli Sifre Uret\t[2] Kaydet\n    ____  \t\t[3] Geri\r    ");
	scanf("%d", &sifre);//Admin m�steri hesap sifresini belirler.
	tercih=getch();
	if(tercih=='2') musteri[i].sifre=sifre;//Kaydet
	if(tercih=='3') {goto menu1;} //Geri
	temizle;
	printf("***Kullanici***\nAdi\t:");
	getchar();
	gets(musteri[i].ad);
	printf("Soy Adi\t:");
	gets(musteri[i].soyad);
	printf("Adres\t:");
	gets(musteri[i].adres);

//printf("Musteri Hizmet No : %d\n", musteri[i].hesapno);
//printf("Sifre\t\t  : %d\n", musteri[i].sifre);
//printf("Adi\t\t  : %s\n", musteri[i].ad);
//printf("Soy Adi\t\t  : %s\n", musteri[i].soyad);
//	printf("Adres\t\t  : %s", musteri[i].adres);
	//Dosyalama Islemleri
	FILE *fptr;
	dosyaislem(i);//Her kullanici adinda dosya olusturmak icin adi d�zenleyecek fonksiyon.
	fptr=fopen(dosyadi,"a");//Adsoyad.txt seklinde dosya acilir.
	fprintf( fptr,"Musteri Hizmet No : %d\n", musteri[i].hesapno);
	fprintf(fptr,"Sifre\t\t  : %d\n", musteri[i].sifre);
	fprintf(fptr,"Adi\t\t  : %s\n", musteri[i].ad);
	fprintf(fptr,"Soy Adi\t\t  : %s\n", musteri[i].soyad);
	fprintf(fptr,"Adres\t\t  : %s", musteri[i].adres);
	fprintf(fptr,"\n----------------HESAP HAREKETLERI----------------\n");
	fclose(fptr);
}
///////////////////////////////////////////////HESAP DUZELTME////////////////////////////////////////////////////////
char hesapduzelt (int i){
	char tercih;
	temizle;
	FILE *fptr;
	dosyaislem(i);//Her kullanici adinda dosya olusturmak icin adi d�zenleyecek fonksiyon.
	fptr=fopen(dosyadi,"r+");//Adsoyad.txt seklinde dosya acilir.
	printf("Degistirmek Istediginiz Bilgiyi Seciniz\n");
	printf("[1] Musteri Hizmet No : %d\n", musteri[i].hesapno);
	printf("[2] Sifre\t\t  : %4d\n", musteri[i].sifre);
	printf("[3] Adi\t\t\t  : %s\n", musteri[i].ad);
	printf("[4] Soy Adi\t\t  : %s\n", musteri[i].soyad);
	printf("[5] Adres\t\t  : %s\n", musteri[i].adres);
	printf("[6] GERI");
	tercih=getch();//Kullanici tercihi alinir.
	temizle;
	switch (tercih){
		case '1':{
				printf(" 8 haneli Yeni Musteri Hizmet No Giriniz:");
				scanf("%d", &musteri[i].hesapno);
			break;
		}
		case '2':{
			printf("4 haneli Yeni Sifre Giriniz :");
			scanf("%d", &musteri[i].sifre);//Admin m�steri hesap sifresini belirler.
			break;
		}
		case '3':{
			printf("Yeni Adi Giriniz\t:");
			gets(musteri[i].ad);
			break;
		}
		case '4':{
			printf("Yeni Soy Adini Giriniz\t:");
			gets(musteri[i].soyad);
			break;
		}
		case '5':{
			printf("Yeni Adresi Giriniz\t:");
			gets(musteri[i].adres);
			break;
		}
		default :{
			return '3';//Admin paneline d�n.
			break;
		}
	}
	temizle;
	printf("------Kullanici Bilgileri----------\n");// Son kullanici Bilgileri Ekranda G�r�nt�lenir.
	printf("Musteri Hizmet No : %d\n", musteri[i].hesapno);
	printf("Sifre\t\t  : %d\n", musteri[i].sifre);
	printf("Adi\t\t  : %s\n", musteri[i].ad);
	printf("Soy Adi\t\t  : %s\n", musteri[i].soyad);
	printf("Adres\t\t  : %s", musteri[i].adres);
	getch();
	
	//Dosyalama Islemleri (Desiklikler dosyanin basindan itiaren yazilir)
	fprintf(fptr,"Musteri Hizmet No : %d\n", musteri[i].hesapno);
	fprintf(fptr,"Sifre\t\t  : %d\n", musteri[i].sifre);
	fprintf(fptr,"Adi\t\t  : %s\n", musteri[i].ad);
	fprintf(fptr,"Soy Adi\t\t  : %s\n", musteri[i].soyad);
	fprintf(fptr,"Adres\t\t  : %s\n", musteri[i].adres);
	fclose(fptr);
}
/////////////////////////////////////HAVALE/////////////////////////////////////////
char havale(int keygon){
	int i, keyalan;
	char ustmenu, aciklama[20];
	int musterino;
	float hvmiktar;
	char tercih;
	FILE *fptr=NULL;
	menu1:
	temizle;
	printf("\nMusteri Hizmet Numaranizi\t[1] Evet\n");
	printf("\tGiriniz\t\t\t[2] Hayir\n");
	printf("\t________\t\t[3] Sil\r\t");
	scanf("%d",&musterino);//M�steriden kendi hesap numarasi istenir.
	tercih=getch();
	if (tercih=='1'){// Evet secilir.
		if(musteri[keygon].hesapno!=musterino){//Baslangictaki numarasi ile girdigi numara karsilastirilir.
		printf("\nMusteri Hizmet Numarasi Yanlis \n\tTekrar Deneyiniz.");
		getch();
		goto menu1;
		} 
	}
	else if(tercih=='2') return '2';//Hayir (M�steri Paneline d�n)
	else if(tercih=='3') goto menu1;//Sil
	menu2:
	temizle;
	printf("\nHavale Yapilacak Kullanici\t");
	printf("[1] Evet\n Hesap Numarasini Giriniz\t[2] Hayir\n ");
	printf("\t________\t\t[3] Sil\r\t");
	scanf("%d",&musterino);// Havale  yapilacak m�steri numarasi istenir.
	tercih=getch();
	if(tercih=='1'){//Hesap numrasi var mi kontrol edilir.
		keyalan=hspkeytespit(musterino);//hesap dogru ise indisi degil ise 101 d�nd�r�r.
		if(keyalan==101){//key 101 ise tekrar m�steri numarasi istenir.
			printf("\nHatali M�steri Numarasi Girdiniz!!!");
			getch();
			goto menu2;
		}
	}	
	else if(tercih=='2') return '2';//Hayir (M�steri Paneline d�n)
	else if(tercih=='3') goto menu2;//Sil
	menu3:
	temizle;
	printf("\nAciklama Giriniz.\t\t[1] Evet");
	printf("\n\t\t\t\t[2] Hayir\n");
	printf("\t\t\t\t[3] Sil\n");
	printf("_\t\t\t\t[4] Atla\r");
	for(i=0;i<20;i++){//Aciklama g�ng� ila karakter karakter aliir.
		aciklama[i]=getche();
		if(aciklama[0]=='4' && aciklama[1]=='\x0D') {//4 ile enter tusuna basilirsa adim atlanir.
		printf("\n"); break;}
		if(aciklama[i]=='\x0D'){
		printf("\n");
		break;}//aciklama girildikten sonra entere basilirsa onay alinir.
	}
	tercih=getch();
	if(tercih=='1'){
		//Bir sonraki adima gecilir.
	} else if(tercih=='2'){
		 return '2'; //M�steri Paneline d�n.
	} else if(tercih=='3'){ // Sil
		goto menu3;
	}
	temizle;
	menu4:
	printf("\n   Havale Miktarini\t\t[1] Evet");
	printf("\n\tGiriniz\t\t\t[2] Hayir\n ");
	printf("\t_____,00 TL\t\t[3] Sil\r\t");
	scanf("%f",&hvmiktar);// Havale tutari istenir.
	tercih=getch();
	if(tercih=='1'){ // EVet
		//Havale icin yeterli bakiyesi var mi kontrol edilir.
		if(musteri[keygon].bakiye<hvmiktar){
			printf("Bakiye Yetersiz.Tekrar Deneyin."); 
			goto menu4;}
	} else if(tercih=='2'){
		return '2'; // Hayir (M�steri Paneline d�n)
		} else if(tercih=='3') goto menu4; //Sil
	
	printf("\n%d Nolu Kullaniciya\t[1] Evet",musteri[keyalan].hesapno);
	printf("\n%.2f TL Gonderilecek\t\t[2] Hayir\n ", hvmiktar);
	printf("  Onayliyormusunuz?");
	tercih=getch();
	if(tercih=='1'){// Onay verilirse
		musteri[keygon].bakiye-=hvmiktar;
		musteri[keyalan].bakiye+=hvmiktar;
		//Para g�nderen hesaba para ciktisi kayit edilir.
		dosyaislem(keygon);//Dosya adi tespit edilir.
		fptr=fopen(dosyadi,"a");//Var olan dosya acilir.
		fprintf(fptr,"%d numarali hesaba %.2f TL g�nderildi. Aciklama->%s\t\t\t",musteri[keyalan].hesapno,hvmiktar,aciklama);
		fprintf(fptr,"BAKIYE : %.2f\n", musteri[keygon].bakiye);
		fclose(fptr);
		//Para alan hesaba para girdisi kayit edilir.
		dosyaislem(keyalan);//Dosya adi tespit edilir.
		fptr=fopen(dosyadi,"a");//Var olan dosya acilir.
		fprintf(fptr,"%d numarali hesaptan hesaba %.2f TL g�nderildi. Aciklama->%s\t\t\t",musteri[keygon].hesapno,hvmiktar,aciklama);
		fprintf(fptr,"BAKIYE : %.2f\n", musteri[keyalan].bakiye);
		fclose(fptr);
	} else if(tercih=='2'){// Onay verilmezse
		printf("\nIsleminiz Iptal Edildi!!!");
		getch();
		return '2'; //M�steri Paneline d�n.
	} 
}
/////////////////////////////////////////////////PARA CEKME-YATIRMA//////////////////////////////////////////////////////
char para_cek_yatir(int key, int islemkey){
//para cekme 1 para yatirma 2
	char ustmenu;
	float cekmiktar, yatmiktar;
	char tercih;
	FILE * fptr=NULL;
	menu1:
	temizle;
	if(islemkey==1){//Para cekme islemi
		printf("  Miktari Giriniz...\n\t\t[1] Evet\n_____,00 TL\t[2] Hayir\r");
		scanf("%f",&cekmiktar);// Cekilecek miktar istenir.
		printf("  \t\t[3] Sil\n  \t\t[4] GERI\n");
		tercih=getch();
		if(tercih=='1') {//Evet
			if(musteri[key].bakiye<cekmiktar){//hesapdaki paradan fazla para cekmek isterse
				printf("\nYetersiz Bakiye!!!");
				getch();
				goto menu1;//islemi tekrarlattirir.
			} else {//bakiye yeterli ise 
				musteri[key].bakiye-=cekmiktar;//Bakiyeden cekilen tutar d�s�l�r.
				}
		}
		else if (tercih='2' || tercih=='4') return '2';// Hayir ve Geri (M�steri paneline d�n)
		else if(tercih=='3') goto menu1;//Sil	
	} 
	else if (islemkey==2){//para yatirma islemi
		printf("  Miktari Giriniz...\n\t\t[1] Evet\n_____,00 TL\t[2] Hayir\r");
		scanf("%f",&yatmiktar);// Yatirilacak miktar girilir.
		printf("  \t\t[3] Sil");
		tercih=getch();
		if(tercih=='1') musteri[key].bakiye+=yatmiktar; // Evet
		else if (tercih='2' || tercih=='4') return '2';// Hayir ve Geri (M�steri paneline d�n)
		else if(tercih=='3') goto menu1; // Sil	
	}
	//dosya kayit islemleri
	if(tercih=='1'){//kullanici onaylarsa dosyaya kayit edilir.
		if (islemkey==1){//para cekme ilgili kullanicinin dosyasina islenir.
			dosyaislem(key);
			fptr=fopen(dosyadi,"a");
			fprintf(fptr,"Hesabinizdan %.2f TL cektiniz.\t\t\t\t\t\t BAKIYE : %.2f\n",cekmiktar,musteri[key].bakiye);
			printf("\n kontrol cekme  %s\n", dosyadi);
			fclose(fptr);
		} else if (islemkey==2){//para yatirma ilgili kullanici hesabina islenir.
			dosyaislem(key);
			fptr=fopen(dosyadi,"a");
			fprintf(fptr,"Hesabiniza %.2f TL yatirdiniz.\t\t\t\t\t\t BAKIYE : %.2f\n",yatmiktar,musteri[key].bakiye);
			printf("\n kontrol yatirma  %s\n", dosyadi);
			fclose(fptr);
		}
	}
	temizle;
	printf("Isleminiz Tamamlandi!\n[1] Ana Menu\n[2] Bakiye Goruntuleme\n[3] Cikis\n");
	tercih=getch();
	while (1){
		if(tercih=='1' || ustmenu=='1') return '1'; // M�steri paneline d�n�l�r.
		if(tercih=='2')  { //Bakiye G�r�nt�lemeye gecilir.
			ustmenu=bakiye(key);
			}
		if(tercih=='3' || ustmenu=='3') return '3'; //Admin/m�steri ayrimina d�n.
	}
}
//////////////////////////////////////////////////HESAP-SIFRE KONTROL///////////////////////////////////////////////////
int hspkont(void){
	int musterino, sifre,  j, i,sayac;
	for (i=0;i<3;i++){
		temizle;
		printf("Musteri Numaranizi Giriniz:");
		scanf("%d", &musterino);//M�steri numarasi istenir
		printf("Siferinizi Giriniz:");
		scanf("%d", &sifre);//M�steriden sifresi istenir.
		for(j=0;musteri[j].hesapno!=0;j++){//Dizinin t�m dolu elemanlari kontrol edilir.
			if(musteri[j].hesapno==musterino && musteri[j].sifre==sifre){//hesap no ve sifre eslesirse sayac 1 artar
				return j;//Eslesen hesabin indis degeri d�nd�r�l�r.
			} else if(musteri[j].hesapno==musterino){//hesap blokesi icin eslesen indis tututlur.
				sayac=j;
			}
		}
		if(i<2){//i 2 den k�c�kse ekrana ka� defa deneme hakki oldugu yazdirilir.
			printf("\tSIFRE YA DA HESAP NUMARANIZ\n");
			printf("\t\tHATALI GIRILMISTIR !\n\n");
			printf("\t\t\t%d\n\n",2-i);
			printf("\t\tHAKKINIZ KALDI !\n ");	
			getch();
		} else {//3. defa da hesap ve sifre eslesmesse bloke edilir.
			printf("\t%d NOLU MUSTERI HESABI\n",musteri[sayac].hesapno);
			printf("\t\tBLOKE OLMUSTUR\n\n");
			printf("\tLUTFEN BANKANIZ ILE ILETISIME\n ");	
			printf("\t\tGECINIZ");	
			getch();
		}
	}
	return 101;// Hesap bulunamadigindan bos indis d�nd�r�r�l�r.
}
///////////////////////////////////////////////HESAP KEY TESPIT ////////////////////////////////////////////////////
int hspkeytespit(int musterino){// m�steri numarasini girdi olarak alir ve t�m elemanlar
	int i=0;				// kontrol edilerek eslesme olursa indisi olmaz ise 101 d�nd�r�r.
	for (i;i<100;){
		if(musteri[i].hesapno==musterino){
			return i;
		}
		i++;
	}
	return 101;	
}
//////////////////////////////////////////////////////FATURA �DEME////////////////////////////////////////////////////////
char faturaode(int key){
char fatura[3][20]={"Elektrik", "Su", "A.D.S.L Internet"};// Fatura isimlerini tutan dizi
int i;
FILE *fptr;
char *tercih1, tercih;
float ftmiktar;
	temizle;
   	printf("\n  Odeme Yapmak Istediginiz Fatura Turunu Giriniz.\n\n");
	printf("[1] Elektrik\n[2] Su\n[3] A.D.S.L Internet\n[4] GERI\n ");
	tercih1=(char*)malloc(sizeof(char)*1);// tercih1 icin bellekte yer tahsis edilir.
	*tercih1=getch();// karakter okunur.
	i=atoi(tercih1);//tercih1 karakteri sayisal degere d�nd�r�l�r. fatura indisi icin i'ye atanir.
	if(*tercih1=='4'){
		return '2'; } // Geri (M�steri Paneline d�n)
	menu2:
	temizle;
	printf("\n   Odeme Miktarini Giriniz\n\n");
	printf("\t_________,00 TL\r\t");
	scanf("%f", &ftmiktar);// fatura miktari kullanicidan alinir.
	printf("\n[1] Evet\n[2] Hayir\n[3] Sil ");
	tercih=getch();
	temizle;
	if(tercih=='1'){
		//Kullanici bakiyesi fatura �demeye yeterli mi kontrol edilir.
		if(musteri[key].bakiye<ftmiktar){
			printf("Bakiye Yetersiz.Tekrar Deneyin."); //Bakiye yetersiz ise ikaz edilir.
			getch();
			goto menu2;}// Tekrar tutar girmesi istenilir.
		printf("\n%s Faturasi Olarak Hesabinizdan\n", fatura[i-1] );
		printf("%.2f TL Odeme Yapilacaktir.\n ", ftmiktar);
		printf("\n\tOnayliyormusunuz?\n\n[1] Evet\n[2] Hayir");
		tercih=getch();
		if(tercih=='1'){//Evet
				musteri[key].bakiye-=ftmiktar;//Fatura tutari bakiyeden d�s�l�r.
				//Dosyalama islemi gerceklestirilir.
				dosyaislem(key);// kullanici dosyasini tespit edecek fonk cagirilir.
				fptr=fopen(dosyadi,"a");// dosya a�ilir.
				fprintf(fptr,"Hesabinizdan %.2f TL tutarindaki %s faturasi �dendi.\t\t\t BAKIYE : %.2f\n",ftmiktar,fatura[i-1],musteri[key].bakiye);
				fclose(fptr);// dosya kapatilir.
		} else if (tercih='2'){
			return '2';// Hayir (musteri paneline d�n)
		}
	} else if(tercih=='2'){
		return '2';//Hayir (musteri paneline d�n)
	} else if(tercih=='3'){ //Sil
		goto menu2;}
free(tercih1);// ayrilan bellek alani serbest birakilir.
}
////////////////////////////////////////////////////BAKIYE G�R�NT�LEME//////////////////////////////////////////////////
char bakiye(int key){
	char tercih;
	temizle;
	printf("\n\t%d Nolu Hesapta\t\t[1] Ana Menu\n",musteri[key].hesapno);
	printf("\n%.2f TL Paraniz Bulunmaktadir.\t[2] Cikis ",musteri[key].bakiye);
	tercih=getch();
	if(tercih=='2'){
		return '3';//Ana men�ye d�nmek icin 3 d�nd�r�r�l�r.
	} else return '1'; // diger t�m durumlarda bir �st men�ye gecilir.
}
////////////////////////////////////////////DOSYA ADI OLUSTURACAK//////////////////////////////////////////////////////////
void dosyaislem(int key){
	strcpy(dosyadi,"C:\\ATM Project\\");
	strcat(dosyadi,musteri[key].ad);//M�steri ismi dosyaadi i�ine kopyalanir.
	strcat(dosyadi,musteri[key].soyad);//m�steri isminin sonuna soyadi eklenir.
	strcat(dosyadi,".txt");//m�steri isim ve soysiminin sonuna .txt eklenir.
}
