//TODO
//Temat: Klasa C++ reprezentująca archiwum ZIP
//	Liczba osób : 2
//	Standard formatu ZIP dopuszcza dwa algorytmy kompresji, które są implementowane
//	za pomocą poznanych bibliotek : zlib oraz libbzip2(patrz pierwsze laboratorium).
//	Tematem projektu jest wykorzystanie obu tych bibliotek do stworzenia klasy
//	języka C++ reprezentującej archiwum ZIP.
//	Klasa powinna udostępniać min.następujące metody :
//	   • Create - utworzenie pustego archiwum,																	---BRAK!!!!!!!!!!
//		   • Open - otwarcie archiwum,																			---GOTOWE
//		   • Close - zamknięcie archiwum,																		---GOTOWE
//		   • ListContents - zwrócenie listy plików(do listy),													---GOTOWE
//		   • GetHeader - zwrócenie nagłówka konkretnego pliku z archiwum,										---GOTOWE
//		   • AddFile - dodanie pliku do archiwum z kompresją(tu się ustala parametry kompresji),				---GOTOWE
//		   • AddFileRaw - dodanie do archiwum pliku skompresowanego(przydaje się kiedy
//			   trzeba przenosić plik z archiwum do archiwum - np.przy wyrzucaniu niektó -
//			   rych plików, bądź zastępowaniu nową wersją),
//		   • GetFile - dekompresja pliku z archiwum,															---GOTOWE
//		   • GetFileRaw - zwrócenie z archiwum pliku skompresowanego.											---GOTOWE
//		   Klasa powinna także umożliwiać podpięcie przez użytkownika własnego paska postępu					
//		   (np.poprzez odziedziczenie po jakiejś klasie bazowej) i własnej obsługi błędów.

#include "ZlibWrapper.h"
#include "Bzip2.h"
#include <fstream>





int main(int argc, char **argv)
{

	//auto zip = akdzlib::zipper();

	/*Bzip2 bzip;

	auto unzip2 = akdzlib::unzipper();


	bzip.compress();
	unzip2.open("foo2.zip");
	bzip.decompress();*/


	//zip.open("mojZip2.zip");

	///*std::ifstream ifs;

	//ifs.open("foo2.txt", std::ifstream::in);
	//zip.addEntry("foo2.txt");
	//zip << ifs;*/

	//

	//auto unzip = akdzlib::unzipper();


	//unzip.open("mojZip.zip");
	//unzip.openEntry("foo2.txt");
	//unzip.getEntrySize();

	//unzip.close();
	//zip.close();



	auto zip = ZlibWrapper();
	zip.Open("pkZip2.zip");

	zip.AddFile("foo2.txt", true, 9);
	zip.Close();
	zip.Open("pkZip2.zip");
	auto l = zip.ListContents();
	auto header = zip.GetHeader("foo2.txt");

	auto result = zip.GetRawFile(l[0].c_str());
	cout << result.data();

	zip.AddRawFile("abc2.txt",result.data(),result.size(), true);

	//zip.AddFile("foo2.txt");
	/*zip.AddFile("inwo.txt");
	zip.AddFile("plik2.txt");*/
	
	zip.Close();
}
