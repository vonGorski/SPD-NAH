#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <sstream>
using namespace std;
//****************************************
//definicje globalne
//****************************************
// definicja typu danych
#define DATA_TYPE int
#define HOW_MANY_DATAS 120
// sciezka pliku
string PATH = "./neh.data.txt";
string SOLUTION_PATH = "./solution.txt";
// rozpoznawanie zestawu danych
string DATAN = "data.";
//****************************************
// MACRO pomocnicze
//****************************************
template <typename T>
string NumberToString(T Number)
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}
//****************************************
//klasa pomocnicza
//****************************************
template<typename TYPE> class DATA
{
public:
	vector<TYPE> Task;
	//vector<TYPE> Task_ENDTIME;
	int id;
	// konstruktor
	DATA( int i)
	{
		id = i;
	}
	// wypisanie elementow klasy
	void PRINT()
	{
		cout << "task number:  "<< id+1 << endl;
		
		for (int i = 0; i < Task.size(); i++)
		{
			cout << Task[i] << "  ";
		}	
		cout << endl;
	}
	void ADD_NEW_TASK(TYPE new_value)
	{
		Task.push_back(new_value);
		//Task_ENDTIME.push_back(0);
	}
	TYPE WAGE()
	{
		TYPE wage=0;
		for (int i = 0; i < Task.size(); i++)
		{
			wage += Task[i];
		}	
		return wage;
	}
};

//****************************************
// FUNKCJE
//****************************************
//wypisanie wszystkich elementow wektora
 void print_from_vector(vector<DATA<DATA_TYPE>> data)
 {

     for (vector<DATA<DATA_TYPE>>::iterator itera = data.begin(); itera != data.end(); itera++)
     {
         itera->PRINT();
		 cout << endl;
     }
 }
 //wypisanie w kolejnosci zgodnej z kolejka 
 void print_queue(vector<DATA<DATA_TYPE>> data, vector<int>&Q)
 {

	 for (vector<int>::iterator itera = Q.begin(); itera != Q.end(); itera++)
	 {
		 data[*itera].PRINT();
	 }
 }
 //wypisywanie kolejki
 void print_Q(vector<DATA<DATA_TYPE>> data, vector<int>& Q)
 {

	 for (vector<int>::iterator itera = Q.begin(); itera != Q.end(); itera++)
	 {
		 data[*itera].PRINT();
	 }
 }
 // sortowanie bombelkowe wag
 void sort(vector<DATA<DATA_TYPE>>& data, vector<int>& Q)
 {
	 for (int i = 0; i < Q.size() - 2; i++)
	 {
		 // Last i elements are already in place
		 for (int j = 0; j < Q.size() - i - 1; j++)
		 {
			 if (data[Q[j]].WAGE() < data[Q[j + 1]].WAGE())
				 iter_swap(Q.begin() + j, Q.begin() + j+1);
		 }
	 }
		
	 //vector<DATA<DATA_TYPE>>& data, vector<int>& Q
 }
 // funkcja odczytu pakietu danych z pliku
void read_from_file(int I, vector<DATA<DATA_TYPE>>&data, vector<int>&Q)
{
	ifstream file(PATH);
	string s, data_number_name;
	if (I < 100)
	{
		if (I < 10)
		{
			data_number_name = DATAN + "00" + NumberToString(I) + ":";
		}
		else data_number_name = DATAN + "0" + NumberToString(I) + ":";
	}
	else data_number_name = DATAN + NumberToString(I) + ":";
	// pomojaj znaki do momentu napotkania definicji paczki data.i
	while (s != data_number_name) file >> s;
	int temp, parts_number, task_number;
	file >> parts_number;
	file >> task_number;
	for (int i = 0; i < parts_number; i++)
	{
		DATA<DATA_TYPE> temp_DATA(i);
		for (int j = 0; j < task_number; j++)
		{
			file >> temp;
			temp_DATA.ADD_NEW_TASK(temp);
		}
		data.push_back(temp_DATA);
		Q.push_back(i);
	}
	sort(data, Q);
}
// obliczanie cmax
DATA_TYPE CMAX(vector<DATA<DATA_TYPE>> data, vector<int>Q)
{

	// tworzenie pomocniczej tablicy
	int s1 = Q.size();
	int s2 = data.begin()->Task.size();
	//int temp_size = Q.size()*(data.begin()->Task.size());
	DATA_TYPE ** TIME= new DATA_TYPE * [s1];
	for (int i = 0; i < s1; i++)
	{
		TIME[i] = new DATA_TYPE[s2];
	}
	for (int i = 0; i < s1; i++)
	{
		for (int j = 0; j < s2; j++) { TIME[i][j] = 0; }
	}
	for (int i=0; i < s1; i++)
	{
		for (int j=0; j< s2; j++ )
		{
			if (i == 0 && j == 0)
			{
				
				TIME[i][j] = data[Q[i]].Task[j];
			//	cout << TIME[i][j] << " - ";
			}
			else if (i == 0 && j != 0)
			{
			//	cout << max(TIME[i][j - 1], TIME[i][j]) + data[Q[i]].Task[j] << " - ";
				TIME[i][j] = max(TIME[i][j - 1], TIME[i][j]) + data[Q[i]].Task[j];
			}
			else 
			{
			//	cout << max(TIME[i][j - 1], TIME[i-1][j]) + data[Q[i]].Task[j] << " - ";
				TIME[i][j] = max(TIME[i][j - 1], TIME[i-1][j]) + data[Q[i]].Task[j];
			}
		}
	//	cout << endl;
	}
	return TIME[s1-1][s2-1];
}

void NEH(vector<DATA<DATA_TYPE>> data, vector<int>&Q)
{
	vector<int> NEW_Q;
	NEW_Q.push_back(Q[0]);
	for (int i = 1; i < Q.size(); i++)
	{;
		DATA_TYPE BEST_CMAX = 9999999;
		int BEST_ITERATOR;
		for (int j = 0; j<=NEW_Q.size();j++)
		{
			NEW_Q.insert(NEW_Q.begin() + j, Q[i]);
			DATA_TYPE tempCMAX = CMAX(data, NEW_Q);
			if (tempCMAX < BEST_CMAX)
			{				
				BEST_CMAX = tempCMAX;
				BEST_ITERATOR = j;
				if (j == NEW_Q.size() - 1) { NEW_Q.pop_back();
				}
				else {
					NEW_Q.erase(NEW_Q.begin() + j);
				}
			}
			else
			{
				if (j == NEW_Q.size() - 1) {NEW_Q.pop_back();}
				else { NEW_Q.erase(NEW_Q.begin() + j); }
			}
		}
		NEW_Q.insert(NEW_Q.begin() + BEST_ITERATOR, Q[i]);
	}
	Q = NEW_Q;
}

int main()
{
	vector<DATA<DATA_TYPE>> data[HOW_MANY_DATAS];
	vector<int> queues[HOW_MANY_DATAS];

	int x=1;
	read_from_file(x, data[x], queues[x]);


	//cout << "cmax: " << CMAX(data[x], queues[x]);
	//cout << endl;		
	//	print_queue(data[x], queues[x]);
	//
	//cout << "cmax: " << CMAX(data[x], queues[x]);
	//cout << endl;
	//	print_queue(data[x], queues[x]);
	NEH(data[x], queues[x]);
	cout << "cmax: " << CMAX(data[x], queues[x]);
	cout << endl;
	print_queue(data[x], queues[x]);

}
