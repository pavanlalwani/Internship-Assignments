#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
using namespace std;

/*
We have implemented this Aadhar card database and phone number allocation problem primarily by making use of Inheritance.
O(1) access time is achieved by using Hashing.
The "Aadhar" class has been defined to encapsulate together the structure "aadharUserData" (which contains atrributes for a single record) 
and several other related functions.
The "PhoneCompany" class inherits from the "Aadhar" class and manipulates the data.
 */

struct aadharUserData{
        int uniqueAadharID;
        string firstName;
        string lastName;
        string guardianName;
        bool isBlacklist;
};

class Aadhar{
        aadharUserData *table;
        int *chain;
        int numberOfEntries;
        int generateAadharID();
        int hashEntry(int ID);
        void getData(aadharUserData &temp);
protected:
        int tableSize;
public:
        Aadhar();
        void createTable();
        void addEntry();
        int searchEntry(int);
        void deleteEntry(int);
        void modifyEntry(int);
        int verifyEntry(int,string);
        void blacklistEntry(int);
	void unBlacklistEntry(int);
        void displayAll();
};

Aadhar::Aadhar(){
        tableSize=0;
        numberOfEntries=0;
        table=NULL;
        chain=NULL;
}

void Aadhar::createTable(){
        cout<<"Enter Size of Table :: ";
        cin>>tableSize;
        table = new aadharUserData[tableSize];
        chain = new int[tableSize];
        for(int i = 0;i < tableSize; i++){
                table[i].uniqueAadharID =- 1;
                table[i].firstName = "";
                table[i].lastName = "";
                table[i].guardianName = "";
                table[i].isBlacklist = 0;
                chain[i] = -1;
        }
}

int Aadhar::generateAadharID(){
        return 10000000 + (rand()%90000000);  //generate 8 digit ID
}

int Aadhar::hashEntry(int ID){
        return ID%tableSize;
}

void Aadhar::getData(aadharUserData &temp){
        temp.uniqueAadharID = generateAadharID();
        cout<<"Enter First Name :: ";
        cin>>temp.firstName;
        cout<<"Enter Last Name :: ";
        cin>>temp.lastName;
        cout<<"Enter Guardian Name :: ";
        cin>>temp.guardianName;
        temp.isBlacklist = 0;
        cout<<endl;
}

void Aadhar::addEntry(){
        aadharUserData tempEntry;
        if(numberOfEntries >= tableSize){
                cout<<"Table Full"<<endl;
                return;
        }
        getData(tempEntry);
        int index = hashEntry(tempEntry.uniqueAadharID);
        int temp = index;
        if(table[index].uniqueAadharID == -1){
                table[index] = tempEntry;
                numberOfEntries++;
                return;
        }
        if(hashEntry(table[index].uniqueAadharID) == index){
                while(table[temp].uniqueAadharID != -1){
                        temp = (temp+1)%tableSize;
                }
                table[temp] = tempEntry;
                while(chain[index] != -1){
                        index = chain[index];
                }
                chain[index] = temp;
                numberOfEntries++;
                return;
        }
        int xchg = hashEntry(table[index].uniqueAadharID);
        while(table[xchg].uniqueAadharID != table[index].uniqueAadharID){
                xchg = chain[xchg];
        }
        while(table[temp].uniqueAadharID != -1){
                temp = (temp+1)%tableSize;
        }
        table[temp] = table[index];
        chain[xchg] = temp;
        table[index] = tempEntry;
        numberOfEntries++;
}

void Aadhar::displayAll(){
        cout<<"===================================================================="<<endl;
	cout<<"S.No."<<setw(20)<<"Aadhar ID"<<setw(20)<<"First Name"<<setw(10)<<"Chain"<<endl;
	cout<<"===================================================================="<<endl;
        for(int i = 0;i < tableSize; i++){
                cout<<i;
                cout<<setw(20)<<table[i].uniqueAadharID;
                if(table[i].isBlacklist){
                        cout<<setw(20)<<"Entry Blacklisted"<<endl;
                        continue;
                }
                cout<<setw(20)<<table[i].firstName;
                cout<<setw(10)<<chain[i]<<endl;
        }
	cout<<"===================================================================="<<endl;
}

int Aadhar::searchEntry(int ID){
        int pos;
        pos = hashEntry(ID);
        while(table[pos].uniqueAadharID!=ID&&pos!=-1)
                pos = chain[pos];
        if(pos == -1){
                cout<<"Entry Not Found"<<endl;
                return -1;
        }
        if(table[pos].isBlacklist){
                cout<<"Entry is Blacklisted"<<endl;
                return -2;
        }
        cout<<"Entry Found :: "<<endl;
        cout<<"Name :: "<<table[pos].firstName<<" "<<table[pos].lastName<<endl;
        cout<<"Guardian :: "<<table[pos].guardianName<<endl;
        cout<<"Aadhar ID :: "<<table[pos].uniqueAadharID<<endl;
        return pos;
}

void Aadhar::deleteEntry(int ID){
        int index = searchEntry(ID);
        if(index < 0)
                return;
        if(chain[index] == -1){
                table[index].uniqueAadharID = -1;
                table[index].firstName = "";
                table[index].lastName = "";
                table[index].guardianName = "";
                table[index].isBlacklist = 0;
                chain[index] = -1;
                cout<<"Entry Deleted Successfully"<<endl;
                numberOfEntries--;
                return;
        }
        table[index]=table[chain[index]];
        chain[index]=chain[chain[index]];
        table[chain[index]].uniqueAadharID = -1;
        table[chain[index]].firstName = "";
        table[chain[index]].lastName = "";
        table[chain[index]].guardianName = "";
        table[chain[index]].isBlacklist = 0;
        cout<<"Entry Deleted Successfully"<<endl;
        numberOfEntries--;
}

void Aadhar::modifyEntry(int ID){
        int index = searchEntry(ID);
        if(index < 0)
                return;
        cout<<"Enter First Name :: ";
        cin>>table[index].firstName;
        cout<<"Enter Last Name :: ";
        cin>>table[index].lastName;
        cout<<"Enter Guardian Name :: ";
        cin>>table[index].guardianName;
}

int Aadhar::verifyEntry(int ID,string name){
        int index=searchEntry(ID);
        if(index < 0)
                return -1;
        try{
                if(name.compare(table[index].firstName) == 0){
                        cout<<"Entry Verified"<<endl;
                        return index;
                }
                else
                        throw table[index];
        }catch(aadharUserData &tempEntry){
                cout<<"Data Mismatch"<<endl;
                return-1;
        }
}

void Aadhar::blacklistEntry(int aadharID)
{
	int pos;
	pos = searchEntry(aadharID);
	if(pos == -1)
		return;
	if(table[pos].isBlacklist){
		cout<<"Entry Already Blacklisted"<<endl;
		return;
	}
	table[pos].isBlacklist = true;
	cout<<"Entry Successfully Blacklisted"<<endl;
}

void Aadhar::unBlacklistEntry(int aadharID)
{
	int pos;
	pos = hashEntry(aadharID);
	while(table[pos].uniqueAadharID != aadharID && pos != -1){
		pos = chain[pos];
	}
	if(pos == -1)
	{
		cout<<"Entry Not Found"<<endl;
		return;
	}
	if(!table[pos].isBlacklist)
	{
		cout<<"Entry Already  Un-Blacklisted"<<endl;
		return;
	}
	table[pos].isBlacklist = false;
	cout<<"Entry Successfully Un-Blacklisted"<<endl;
}

class PhoneCompany : public Aadhar{
        long *phoneTable;
public:
        PhoneCompany();
        void generatePhoneNumber(int,string);
        void checkPhoneNumber(int,string);
};

PhoneCompany::PhoneCompany(){
        createTable();
        phoneTable=new long[tableSize];
}

void PhoneCompany::generatePhoneNumber(int ID,string name){
        int pos=verifyEntry(ID,name);
        if(pos<0){
                cout<<"Cannot Allocate Phone Number"<<endl;
                return;
        }
        phoneTable[pos] = 9000000000 + (rand()%1000000000);
        cout<<"Number Allocated :: "<<phoneTable[pos]<<endl;
}

void PhoneCompany::checkPhoneNumber(int ID,string name){
        int pos=verifyEntry(ID,name);
        if(pos<0){
                cout<<"Cannot Access Number"<<endl;
                return;
        }
        cout<<"Number Allocated :: "<<phoneTable[pos]<<endl;
}

int main()
{
        int choice,aadharID,option;
        string name;
        PhoneCompany obj;
        do{
                cout<<"\n:: MENU ::"<<endl;
                cout<<"1.Add Entry"<<endl;
                cout<<"2.Display All"<<endl;
                cout<<"3.Search Entry"<<endl;
                cout<<"4.Modify Entry"<<endl;
                cout<<"5.Blacklist Entry"<<endl;
                cout<<"6.Un-Blacklist Entry"<<endl;
                cout<<"7.Delete Entry"<<endl;
                cout<<"8.Generate Phone Number"<<endl;
                cout<<"9.Check Phone Number"<<endl;
                cout<<"0.Exit"<<endl;
                cout<<"Enter Your Choice :: ";
                cin>>choice;
                cout<<endl;
                switch(choice){
                case 1:
                        obj.addEntry();
                        obj.displayAll();
                        break;
                case 2:
                        obj.displayAll();
                        break;
                case 3:
                        cout<<"Enter the AADHAR ID to be searched :: ";
                        cin>>aadharID;
                        obj.searchEntry(aadharID);
                        break;
                case 4:
                        cout<<"Enter the AADHAR ID to be modified :: ";
                        cin>>aadharID;
                        obj.modifyEntry(aadharID);
                        obj.displayAll();
                        break;
                case 5:
                        cout<<"Enter the AADHAR ID to be blacklisted :: ";
                        cin>>aadharID;
                        obj.blacklistEntry(aadharID);
                        break;
                case 6:
                        cout<<"Enter the AADHAR ID to be un-blacklisted :: ";
                        cin>>aadharID;
                        obj.unBlacklistEntry(aadharID);
                        break;
                case 7:
                        cout<<"Enter the AADHAR ID which is to be deleted :: ";
                        cin>>aadharID;
                        obj.deleteEntry(aadharID);
                        obj.displayAll();
                        break;
                case 8:
                        cout<<"Enter the AADHAR ID to generate phone number :: ";
                        cin>>aadharID;
                        cout<<"Enter Name :: ";
                        cin>>name;
                        obj.generatePhoneNumber(aadharID,name);
                        break;
                case 9:
                        cout<<"Enter the AADHAR ID whose phone number is to be checked :: ";
                        cin>>aadharID;
                        cout<<"Enter Name :: ";
                        cin>>name;
                        obj.checkPhoneNumber(aadharID,name);
                        break;
                case 0:
                        cout<<"EXIT SUCCESS"<<endl;
                        break;
                }
        }while(choice);
        return 0;
}
