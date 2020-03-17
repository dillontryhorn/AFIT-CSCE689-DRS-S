#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

using namespace std;

/*
 * Created by Dillon Tryhorn
 * Proper usage: ./convert2csv <file_to_convert> <name_of_csv_destination>
*/

int main(int argc, char* argv[]) 
{
	ifstream conversion_file(argv[1]);
	ofstream output_csv(argv[2]);
	
	if(conversion_file.is_open() && output_csv.is_open())
	{
		string line;
		while(getline(conversion_file, line))
		{
			stringstream ss(line);
			istream_iterator<string> begin(ss);
			istream_iterator<string> end;
			vector<string> vstrings(begin, end);
			for(int i = 0; i < vstrings.size(); i++)
			{
				if(i == 1) 
				{
					output_csv << vstrings.at(i) << ",";
				}
				if(i == 5)
				{
					output_csv << vstrings.at(i) << ",";
					output_csv << endl;
				}
				
			}
		}
		conversion_file.close();
		output_csv.is_open();
	}
	return 0;
}