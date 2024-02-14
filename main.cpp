#include <fstream>
#include <iostream>

using namespace std;

struct ShipRecord {
  int serialNum;
  int shipType;
  string name;
  int year;
  int cap;
  ShipRecord *link;
};

const int SIZE = 10;

class HashMgr {
  ShipRecord *hashTable[SIZE] = {nullptr};

public:
  HashMgr() {
    string line;
    ifstream myfile;
    myfile.open("shipRecords.txt");
    if (myfile.is_open()) {
      while (!myfile.eof()) {
        addInfo(myfile);
      }
      myfile.close();
    }
  }
  ~HashMgr() {
    for (int i = 0; i < SIZE; i++) {
      if (hashTable[i] != nullptr) {
        ShipRecord *ptr = hashTable[i];
        while (ptr != nullptr) {
          ShipRecord *toKill = ptr;
          ptr = ptr->link;
          delete toKill;
        }
      }
    }
  }
  void addInfo(ifstream &myfile) {
    int serial;
    int type;
    string name;
    int year;
    int cap;
    myfile >> serial >> type >> name >> year >> cap;
    int bucket = serial % 10;
    ShipRecord *record = new ShipRecord{serial, type, name, year, cap};
    ShipRecord *ptr = hashTable[bucket];
    if (ptr == nullptr) {
      hashTable[bucket] = record;
    } else {
      while (ptr->link != NULL) {
        ptr = ptr->link;
      }
      ptr->link = record;
    }
  }
  void displayOne(int serialNum) {
    int bucket = serialNum % 10;
    ShipRecord *ptr = hashTable[bucket];
    if (ptr != nullptr) {
      while (ptr->serialNum != serialNum && ptr != nullptr) {
        ptr = ptr->link;
      }
      if (ptr == nullptr) {
        cout << "Record does not exist." << endl;
        return;
      } else {
        cout << "Bucket #" << bucket << endl;
        cout << ptr->serialNum << "   " << ptr->shipType << "   " << ptr->name
             << "   " << ptr->year << "   " << ptr->cap << endl
             << endl;
      }
    } else {
      cout << "Record does not exist." << endl;
      return;
    }
  }
  void displayAll() {
    for (int i = 0; i < SIZE; i++) {
      if (hashTable[i] != nullptr) {
        cout << "Bucket #" << i << endl;
        ShipRecord *ptr = hashTable[i];
        while (ptr != nullptr) {
          cout << ptr->serialNum << "   " << ptr->shipType << "   " << ptr->name
               << "   " << ptr->year << "   " << ptr->cap << endl;
          ptr = ptr->link;
        }
      }
    }
    cout << endl;
  }
  void deleteOne(int serialNum) {

    // initialize important ints and ptrs
    int bucket = serialNum % 10;
    ShipRecord *ptr = hashTable[bucket];
    ShipRecord *prev = ptr;

    if (ptr != nullptr) {
      // Case 1: object to delete is at beginning of list
      if (ptr->serialNum == serialNum) {
        hashTable[bucket] = ptr->link;
        delete ptr;
        ptr = nullptr;
        prev = nullptr;
        return;
      }
      // iterate through loop until match found or linked list ends
      while (ptr->serialNum != serialNum && ptr != nullptr) {
        prev = ptr;
        ptr = ptr->link;
      }
      // in case the list ends, return
      if (ptr == nullptr) {
        cout << "Record does not exist." << endl;
        return;
      } else {
        // case 2: object to delete is at end of list
        if (ptr->link == nullptr) {
          prev->link = nullptr;
          delete ptr;
          ptr = nullptr;
          prev = nullptr;
          return;
        }
        // case 3: object to delete is in the middle of the list
        else {
          prev->link = ptr->link;
          delete ptr;
          ptr = nullptr;
          prev = nullptr;
          return;
        }
      }
    }
    // if no linked list at location, return
    else {
      cout << "Record does not exist." << endl;
      return;
    }
  }
};
int main() {
  HashMgr hm;
  cout << "displayAll()" << endl << endl;
  hm.displayAll();
  cout << "displayOne()" << endl << endl;
  hm.displayOne(1009);
  hm.displayOne(1010);
  hm.displayOne(1019);
  hm.displayOne(1029);
  hm.displayOne(1039);
  hm.displayOne(1014);
  hm.displayOne(1008); /// Prompt a message to that the record does not exist
  hm.deleteOne(1009);
  hm.deleteOne(1039);
  cout << "displayAll()" << endl << endl;
  hm.displayAll();
  return 0;
}
