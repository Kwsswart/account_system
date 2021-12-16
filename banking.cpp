#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;

using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;

class AccountQuery
{
private:
    char account_number[20];
    char firstName[10];
    char lastName[10];
    float totalBalance;
public:
    void read_data();
    void show_data();
    void read_record();
    void write_record();
    void search_record();
    void edit_record();
    void delete_record();
};

void AccountQuery::read_data() {
    cout << "\nEnter Account Number: ";
    cin>>account_number;
    cout << "Enter First Name: ";
    cin>>firstName;
    cout << "Enter Last Name: ";
    cin>>lastName;
    cout << "Enter Initial Balance: ";
    cin>>totalBalance;
}

void AccountQuery::show_data() {
    cout << "Account Number: " << account_number << endl;
    cout << "First Name: " << firstName << endl;
    cout << "Last Name: " << lastName << endl;
    cout << "Total Balance: " << totalBalance << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
}

void AccountQuery::write_record() {
    ofstream outfile;
    outfile.open("record.bank", ios::binary|ios::app);
    read_data();
    outfile.write(reinterpret_cast<char*> (this), sizeof(*this));
    outfile.close();
}

void AccountQuery::read_record() {
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "Error: File Not Found." << endl;
        return;
    }
    cout << "\n**** Data from file ****" << endl;
    while(!infile.eof()) {
        if (infile.read(reinterpret_cast<char*>(this), sizeof(*this)))
        {
            show_data();
        }
    }
    infile.close();
}

void AccountQuery::search_record() {
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "Error: File Not Found." << endl;
        return;
    }
    infile.seekg(0,ios::end);
    int count = infile.tellg()/sizeof(*this);
    cout << "\n There are " << count << " records in this file." << endl;
    cout << "Enter Record Number to search: ";
    cin >> n;
    infile.seekg((n-1)*sizeof(*this));
    infile.read(reinterpret_cast<char*> (this), sizeof(*this));
    show_data();
}

void AccountQuery::edit_record() {
    int n;
    fstream iofile;
    iofile.open("record.bank", ios::in|ios::binary);
    if (!iofile) {
        cout << "Error: File Not Found." << endl;
        return;
    }
    iofile.seekg(0,ios::end);
    int count = iofile.tellg()/sizeof(*this);
    cout << "\n There are " << count << " records in this file." << endl;
    cout << "Enter Record Number to edit: ";
    cin >> n;
    iofile.seekg((n-1)*sizeof(*this));
    iofile.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "The Record " << n << " contains the following information:" << endl;
    show_data();
    iofile.close();
    iofile.open("record.bank", ios::out|ios::in|ios::binary);
    iofile.seekg((n-1)*sizeof(*this));
    cout << "\nEnter the data to modify " << endl;
    read_data();
    iofile.write(reinterpret_cast<char*>(this), sizeof(*this));
}

void AccountQuery::delete_record() {
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if (!infile) {
        cout << "Error: File Not Found." << endl;
        return;
    }
    infile.seekg(0, ios::end);
    int count = infile.tellg()/sizeof(*this);
    cout << "\n There are " << count << " records in this file." << endl;
    cout << "Enter the Record Number to Delete: ";
    cin >> n;
    fstream tempFile;
    tempFile.open("tmpfile.bank", ios::out|ios::binary);
    infile.seekg(0);
    for (int i=0; i<count; i++){
        infile.read(reinterpret_cast<char*>(this), sizeof(*this));
        if (i == (n-1)){
            continue;
        }
        tempFile.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
    infile.close();
    tempFile.close();
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
}

int main()
{
    AccountQuery account;
    int choice;
    cout << "************** Bank Account Information System **************" << endl;
    while (true)
    {
        cout << "Select an option below: " << endl;
        cout << "\t1 --> Add Record." << endl;
        cout << "\t2 --> Display Record." << endl;
        cout << "\t3 --> Seach Record." << endl;
        cout << "\t4 --> Update Record." << endl;
        cout << "\t5 --> Delete Record." << endl;
        cout << "\t6 --> Quit." << endl;

        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                account.write_record();
                break;
            case 2:
                account.read_record();
                break;
            case 3:
                account.search_record();
                break;
            case 4:
                account.edit_record();
                break;
            case 5:
                account.delete_record();
                break;
            case 6:
                exit(0);
                break;
            default:
                cout <<"\n Error: Invalid choice entered. Exiting System.";
                exit(0);
        }
    }
    system("pause");
    return 0;
}