#include <iostream>
#include <iomanip> 
#include "windows.h"
#include <sstream>//labirenti string formatında yazabilmek için kütüphane
#include <conio.h>
#define YUKSEKLIK 20 
#define GENISLIK 50 

using namespace std;

template <typename Object>
class Node{   //her bir yığını temsil eden Node sınıfı
    public:
    Object item;    //düğümde saklanan veri
    Node<Object>*next;  // bir sonraki düğüme işaret ediyor
    Node(const Object& item ,Node<Object>*next=NULL){// yeni bir düğüm oluşturur
        this->item=item;
        this->next=next;
    }

};
template<typename Object>
class Stack{
    private:
    Node<Object>*topOfStack; // oluşturulan yığının tepe noktasını gösterir
    public:
    Stack(){
        topOfStack=NULL;
    }
    bool isEmpty()const{ // yığın boş mu diye kontrol eder
        return topOfStack==NULL;
    }
    void push(const Object&item){ // yığına eleman ekler 
        topOfStack    =new Node<Object>(item,topOfStack);
    }
    void pop(){ //yığının tepesindeki elemanı çıkarır yığın boşsa hata yollar
        if(isEmpty())throw "Stack is Empty";
        Node<Object> *tmp=topOfStack;
        topOfStack=topOfStack->next;
        delete tmp;
    }
    const Object& top()const{ // yığının tepesindeki elemanı döndürür
        if(isEmpty())throw "Stack is Empty";
        return topOfStack->item;
    }
    void makeEmpty(){ // yığındaki tüm elemanları siler
        while(!isEmpty())pop();
    }
    ~Stack(){ //nesne yok edilirken tüm elemanları siler 
        makeEmpty();
    }
};

typedef enum{ASAGI,SOL,YUKARI,SAG}Yon;
struct Konum
{
    int x,y;
    Yon yon;
    Konum(int,int,Yon); // x,y ve yon verileri ile bir konum oluşturduk
    Konum(int,int); // varsayılan olarak yönü ASAGI olan bir konum oluşturduk
    Konum();         // kordinatlar (0,0) ve yön ASAGI
    Konum AyniYon(); // konumdan aynı yönde bir adım ileri giden konumu döndürdük
    Konum SaatYonu(Yon);// konumu saat yönüne 90 derece döndürdük
    Konum Asagi();
    Konum Yukari();
    Konum Sol();
    Konum Sag();
    Yon TersYon();
};


// her constructor metot , Konum nesnesini başlatır

Konum::Konum(int x,int y,Yon yon){ // yönü ve kordinatı ayarlar

	this->x = x;
	this->y = y;
	this->yon = yon;
}
Konum::Konum(int x,int y){//yön yoksa ASAGI yapar
	this->x = x;
	this->y = y;
	this->yon = ASAGI;
}
Konum::Konum(){ // verilen kordinatlar ve yön ile nesne oluşturur 
	x=0;
	y=0;
	yon=ASAGI;
}
Konum Konum::AyniYon(){ // olan konumdan yön doğrultusunda bir adım ileri giderek yeni bir konum döndürür
	switch(yon){
		case YUKARI:
			return Yukari();
		case SAG:
			return Sag();
			case ASAGI:
			return Asagi();
		default:
			return Sol();
	}
} 
// * verilen yön (yn) için saat yönündeki bir sonraki konumu döndürmek.
Konum Konum::SaatYonu(Yon yn){
	switch(yn){
		case YUKARI: // yn yukarıysa (YUKARI), bir sonraki yön sağdır (Sag()).
			return Sag();
		case SAG: // yn sağsa (SAG), bir sonraki yön aşağıdır (Asagi()).
			return Asagi();
		case ASAGI: // yn aşağıysa (ASAGI), bir sonraki yön soldur (Sol()).
			return Sol();
		default: // başka bir durum varsa (örnek SOL), bir sonraki yön yukarıdır (Yukari()).
			return Yukari();
	}
}
Konum Konum::Asagi(){
	return Konum(x+1,y,ASAGI);
}
Konum Konum::Yukari(){
	return Konum(x-1,y,YUKARI);
}
Konum Konum::Sol(){
	return Konum(x,y-1,SOL);
}
Konum Konum::Sag(){
	return Konum(x,y+1,SAG);
}	
Yon Konum::TersYon(){
	if(yon == ASAGI) return YUKARI;
	if(yon == YUKARI) return ASAGI;
	if(yon == SOL) return SAG;
	if(yon == SAG) return SOL;
	return YUKARI;
}

class Labirent{
	public:
		char harita[YUKSEKLIK][GENISLIK]; // labirenti temsil eden 2 boyutlu karakter dizisi 
		int x,y; // mevcut konumun kordinatları
		Yon yon; // konumun yönü(SAG, SOL, YUKARI, ASAGI)
		Konum bitis; // çıkış noktasını temsil eden konum
		Stack<Konum> *yigit; // hareket geçmişini tutan bir Stack<Konum> nesnesi	
		Labirent(Konum, Konum); // labirentin başlangıç ve bitiş noktaları atanır
		Konum mevcutKonum(); // mevcut konum nesnesini döndürür
		bool adimAt(Konum, Konum); // İleriye gitmeyi dener engel yoksa gider
		void ayarla(Konum,Yon);  // konumu günceller ve haritayı yeniden çizer
		bool CikisaGeldimi();  // labirentin çıkış noktasına gelinip gelinmediğine kontrol eder
		bool EngelVarmi(Konum);		// belirli bir konumda engel olup olmadığını kontrol eder
		string yaz(); // labirenti string olarak döndürür
};

// * labirenti temsil eden harita (Harita.txt) dosyadan okunur ve yüklenir
// * başlangıç ve bitiş konumları atanır
// * hareket geçmişini tutmak için yığıt başlatılır 
Labirent::Labirent(Konum baslangic, Konum bitis){
	FILE* fp = fopen("Harita.txt", "r");
	unsigned char karakter = 0;
	int satir=0,sutun=0;
	x = baslangic.x;
	y = baslangic.y;
	this->bitis = bitis;
	yon = ASAGI;
	yigit = new Stack<Konum>();
	yigit->push(Konum(-1,-1,yon)); // ilk hareket başlamadan önce bir varsayılan konum stacke eklenir 
	while (!feof(fp)) {
	   karakter = getc(fp);
	   if(karakter == 255) break;
	   if(karakter == 10){
		 satir++;
		 sutun=0;
	   }
	   else{
			harita[satir][sutun] = karakter;
			sutun++;
	   }
	}
}
// * mevcut koordinat (x, y) ve yönü (yon) bir Konum nesnesi olarak döndürür
Konum Labirent::mevcutKonum(){
	return Konum(x,y,yon);
}
// *ileri bir konuma geçiş yapmayı denemek
bool Labirent::adimAt(Konum mevcut, Konum ileri){
	if(!EngelVarmi(ileri)){ // bir sonraki gidilecek konumda bir engel olup olmadığını kontrol eder.
		yigit->push(mevcut); // mevcut konum yığına eklenir
		ayarla(ileri,ileri.yon); // yeni konuma geçiş yapılır
		return true;
	}
	return false; // engel varsa, hareket olmaz ve false döner.
} 
// * hareket sonrasında mevcut konumu güncellemek ve haritayı yeniden çizmek
void Labirent::ayarla(Konum konum,Yon yon){
	system("cls"); //  konsol ekranını komple temizler
	// * yeni konumun koordinatları ve yönü güncellenir
	this->x = konum.x;
	this->y = konum.y;
	this->yon = yon;
	harita[konum.x][konum.y] = '-'; // hareket edilen kısım işaretlenir
	cout<<yaz(); // labirenti konsola yazdırır
	Sleep(30); // uyutma komutu, çıkan görseli yavaşlatır
}
// * bulunan konum (x, y), bitiş konumu (bitis.x, bitis.y) ile karşılaştırılır. Eğer aynı ise true döner
bool Labirent::CikisaGeldimi(){
	return x == bitis.x && y == bitis.y;
}
// * ileri gidilecek konumda bir engel olup olmadığını kontrol etmek
bool Labirent::EngelVarmi(Konum konum){			
	if(konum.x >= YUKSEKLIK || konum.x < 0 || konum.y >= GENISLIK || konum.y < 0) return false; // konum sınırların dışındaysa (x veya y negatif veya harita boyutlarının ötesinde), false döner.
	return harita[konum.x][konum.y] == '#' || harita[konum.x][konum.y] == '-'; // ilgili hücre # (duvar) veya '-' (ziyaret edilmiş) ise true döner.

}
// * 	Labirenti string olarak çizmek
string Labirent::yaz(){
	char YonChr[] = {'v','<','^','>'}; // bunlar yön simgeleri sırasıyla (aşağı, sol, yukarı, sağ)
	stringstream ss; // çizimi string içinde tutmak için.
	for(int satir=0;satir<YUKSEKLIK;satir++){
		ss<<setw(10);
		for(int sutun=0;sutun<GENISLIK;sutun++){
			if(satir == x && sutun == y){
				ss<<YonChr[yon]; // mevcut konumda bir yön simgesi (<, >, v, ^) yazdırılır.
			}
			else{
				 if(harita[satir][sutun] == '-') ss<<' '; // üstünden geçilen kısımlar ('-') boşluk (' ') olarak gösterilir.
				 else ss<<harita[satir][sutun];
			}
		}	
		ss<<endl;				
	}
	return ss.str();
}

int main(){
	Labirent *labirent = new Labirent(Konum(0,20),Konum(19,11)); // labirenti temsil eden labirent adında bir nesne oluşturduk
	
	labirent->yigit->push(Konum(labirent->x,labirent->y,ASAGI));
	labirent->ayarla(Konum(labirent->x,labirent->y,ASAGI).Asagi(),ASAGI);	
	while(!labirent->CikisaGeldimi()){
		int denemeYonSayisi=1;
		Konum oncekiKonum = labirent->yigit->top();
		Konum mevcutKonum = labirent->mevcutKonum();
		
		Konum ileri = mevcutKonum.AyniYon();
		
		if(!labirent->adimAt(mevcutKonum,ileri)){
			int i=0;
			bool sonuc = false;
			Konum yeni = mevcutKonum;
			while(!sonuc && denemeYonSayisi<5){	
				yeni = mevcutKonum.SaatYonu((Yon)((mevcutKonum.yon+i)%4));		
								
				i++;
				denemeYonSayisi++;
				if(yeni.yon == mevcutKonum.TersYon()){
					sonuc=false;
				}
				else{
					sonuc = labirent->adimAt(mevcutKonum,yeni);
				}
			}
			if(denemeYonSayisi == 5){
				labirent->yigit->pop();
				labirent->ayarla(oncekiKonum,oncekiKonum.TersYon());
			}
		}
		
	}
	
	cout<<"CIKISA GELDI"<<endl;
	
	getchar();
	
	delete labirent;
	return 0;
}