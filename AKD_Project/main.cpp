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

	auto header = zip.GetHeader(l[0]);

	cout << zip.GetFile(l[0]).data();

	auto result = zip.GetRawFile(l[0]);
	std::cout << result.data();

	zip.AddRawFile("Zip2.zip","abc2.txt",result, true,header);	
	zip.Close();
}
