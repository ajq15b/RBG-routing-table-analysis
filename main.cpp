/****************************************************
my name						    *
Instructor @ name				    *
Project 1					    *
Analyzing Internet Network Connectivity		    *
--analyze BGP routing tables to obtain the	    *
--internet connectivity information, and determine  *
--the set of neighboring Ases for each AS	    *
****************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;


void GetInput(vector<vector<int>>&);
void VectorToMap(const vector<vector<int>>&, map<int, set<int>>&);
void RemoveDuplicates(vector<int>&);
bool CustomSort(pair<int, set<int>> v1, pair<int, set<int>> v2);
void PrintResults(const vector<pair<int, set<int>>>&);


int main(){
	vector<vector<int>> inputVec;
	map<int, set<int>> numMap;

	GetInput(inputVec); 
	VectorToMap(inputVec, numMap);	//utalize map to find and place neighbors
	vector<pair<int, set<int>>> outVec(numMap.begin(), numMap.end());	//copy map to a vector by construction
	sort(outVec.begin(), outVec.end(), CustomSort);   
	PrintResults(outVec);

	return 0;
}

//********************************************
//Get input from user			     *
//Place input into vector of vectors	     *
//remove any unwanted duplicates             *
//********************************************
void GetInput(vector<vector<int>>& in){
	vector<int> tempVec;
	char c = cin.peek(), digit;
	int number = 0, count = 0;

	while (!cin.eof())
	{
		cin.get(c);
		if (c == '|') count++; //count occurences of '|' to know where to begin extracting data

		//part of the input that matters 
		while (count == 6){
			if (cin.peek() == '[') {	//if a ASSET occurs, dont count it
				count++;				//add to counter to exit while
			}
			else{
				digit = cin.peek();     //create the number from chars
				number *= 10;
				number += digit - 48;
			}
			cin.get(c);

			//if there is a space, add the number to vector, reset number
			if (cin.peek() == ' '){
				tempVec.push_back(number);
				number = 0;
				cin.get(c);
			}
			//else add number to vector and get read for a new line of input
			else if (cin.peek() == '|' || cin.peek() == '[') {
				tempVec.push_back(number);
				number = 0;
				count++;
			}
		}

		//new line occurence, prepare for next line of input
		if (c == '\n') {
			count = 0;	
			RemoveDuplicates(tempVec);
			in.push_back(tempVec);  //put temp vector into vector
			tempVec.clear();		//clear out tempVec
		}
	}
} //end GetInput

//**********************************************************************
//Iterate through the vector of vectors, accessing each inner vector   *
//Create or access a key for each individual number		       *				   *
//while placing its neighbor values in a set(to make unique neighbors) *
//**********************************************************************
void VectorToMap(const vector<vector<int>>& vec, map<int, set<int>>& mp){
	for (auto it = vec.begin(); it != vec.end(); it++){
		vector<int> temp;
		for (auto jt = it->begin(); jt != it->end(); jt++){
			mp[*jt];
			temp.push_back(*jt);
		}
		//create and/or insert into map pair
		if (temp.size() > 2){
			mp[temp.at(0)].insert(temp.at(1));							   //left most, one neighbor
			mp[temp.at(temp.size() - 1)].insert(temp.at(temp.size() - 2)); //right most, one neighbor
			for (int i = 1; i < temp.size() - 1; ++i){			//loop for numbers with 2 neighbors
				mp[temp.at(i)].insert(temp.at(i - 1));
				mp[temp.at(i)].insert(temp.at(i + 1));
			}
		}
		else if (temp.size() > 1){
			mp[temp.at(0)].insert(temp.at(1));   //beginning gets only neighbor to right
			mp[temp.at(1)].insert(temp.at(0));   //middle gets right neighbor
		}
	}
}

//************************************************
//This utilizes a set to make the numbers unique *
//then compare against vector and drop duplicates*
//************************************************
void RemoveDuplicates(vector<int>& inVec){
	set<int> temp; // create set to compare
	auto it = begin(inVec);
	while (it != end(inVec)) //loop the vector
	{
		if (temp.find(*it) != end(temp)) 
			it = inVec.erase(it); 
		else{
			temp.insert(*it);
			it++;
		}
	}
} //end RemoveDuplicates

//***********************************************
//Custom sorting				*
//sort by number of neighbors, descending	*
//secondary sort by the numbers value, asceding *
//***********************************************
bool CustomSort(pair<int, set<int>> v1, pair<int, set<int>> v2){
	if (v1.second.size() == v2.second.size()){
		return v1.first < v2.first;
	}
	return v1.second.size() > v2.second.size();
}//end CustomSort

//*********************
//print final output  *
//*********************
void PrintResults(const vector<pair<int, set<int>>>& out){
	for (auto rit = out.begin(); rit != out.end(); ++rit){
		cout << rit->first << ' ' << rit->second.size() << ' ';
		for (auto it = rit->second.begin(); it != rit->second.end()--; ++it){
			if (it == --rit->second.end()) cout << *it;
			else cout << *it << '|';
		}
		cout << endl;
	}
}
	
