//TODO
//Temat: Klasa C++ reprezentująca archiwum ZIP
//	Liczba osób : 2
//	Standard formatu ZIP dopuszcza dwa algorytmy kompresji, które są implementowane
//	za pomocą poznanych bibliotek : zlib oraz libbzip2(patrz pierwsze laboratorium).
//	Tematem projektu jest wykorzystanie obu tych bibliotek do stworzenia klasy
//	języka C++ reprezentującej archiwum ZIP.
//	Klasa powinna udostępniać min.następujące metody :
//	   • Create - utworzenie pustego archiwum,
//		   • Open - otwarcie archiwum,
//		   • Close - zamknięcie archiwum,
//		   • ListContents - zwrócenie listy plików(do listy),
//		   • GetHeader - zwrócenie nagłówka konkretnego pliku z archiwum,
//		   • AddFile - dodanie pliku do archiwum z kompresją(tu się ustala parametry kompresji),
//		   • AddFileRaw - dodanie do archiwum pliku skompresowanego(przydaje się kiedy
//			   trzeba przenosić plik z archiwum do archiwum - np.przy wyrzucaniu niektó -
//			   rych plików, bądź zastępowaniu nową wersją),
//		   • GetFile - dekompresja pliku z archiwum,
//		   • GetFileRaw - zwrócenie z archiwum pliku skompresowanego.
//		   Klasa powinna także umożliwiać podpięcie przez użytkownika własnego paska postępu
//		   (np.poprzez odziedziczenie po jakiejś klasie bazowej) i własnej obsługi błędów.



using namespace std;




int main(int ac, char* av[])
{
	return 0;
}