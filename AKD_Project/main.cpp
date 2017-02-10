//TODO
//Temat: Klasa C++ reprezentująca archiwum ZIP
//	Liczba osób : 2
//	Standard formatu ZIP dopuszcza dwa algorytmy kompresji, które są implementowane
//	za pomocą poznanych bibliotek : zlib oraz libbzip2(patrz pierwsze laboratorium).
//	Tematem projektu jest wykorzystanie obu tych bibliotek do stworzenia klasy
//	języka C++ reprezentującej archiwum ZIP.
//	Klasa powinna udostępniać min.następujące metody :
//		• Create - utworzenie pustego archiwum,																	---GOTOWE
//		   • Open - otwarcie archiwum,																			---GOTOWE
//		   • Close - zamknięcie archiwum,																		---GOTOWE
//		   • ListContents - zwrócenie listy plików(do listy),													---GOTOWE
//		   • GetHeader - zwrócenie nagłówka konkretnego pliku z archiwum,										---GOTOWE
//		   • AddFile - dodanie pliku do archiwum z kompresją(tu się ustala parametry kompresji),				---GOTOWE
//		   • AddFileRaw - dodanie do archiwum pliku skompresowanego(przydaje się kiedy							---GOTOWE
//			   trzeba przenosić plik z archiwum do archiwum - np.przy wyrzucaniu niektó -
//			   rych plików, bądź zastępowaniu nową wersją),
//		   • GetFile - dekompresja pliku z archiwum,															---GOTOWE
//		   • GetFileRaw - zwrócenie z archiwum pliku skompresowanego.											---GOTOWE
//			•  własnej obsługi błędów.																			---GOTOWE

#include "ZlibWrapper.h"
#include <fstream>


using namespace std;


int main(int argc, char **argv)
{
	auto zip = ZlibWrapper();
	zip.Open("nowyZip.zip");

	zip.AddFile("foo2.txt", true, 9);
	zip.AddFile("inwo.txt", true, 9);
	zip.Close();
	zip.Open("nowyZip.zip");
	auto l = zip.ListContents();
	for (auto name : l)
		cout << name << "\n";

	auto header = zip.GetHeader(l[0].c_str());

	cout << zip.GetFile(l[0].c_str()).data();

	auto result = zip.GetRawFile(l[0].c_str());
	std::cout << result.data();

	zip.AddRawFile("Zip2.zip","abc2.txt",result, true,header);	
	zip.Close();
}
