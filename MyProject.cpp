#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

#define MIN_BALANCE 500
class InsufficientBalance
{
};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account(){}
    Account(string fname, string lname, float balance);
    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }

    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};
long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accounNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdaw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAcounts();
    ~Bank();
};
int main()
{
    Bank b;
    Account acc;

    int choice;
    string fname;
    string lname;
    int balance;
    int amount;
    int accountNumber;

    cout << endl
         << "**Welcome to the Banking System**"
         << endl;
    while (choice != 7)
    {
        cout << "Select one option below"
             << "\n\t1. Open an Account"
             << "\n\t2. Balance Enquiry"
             << "\n\t3. Deposit"
             << "\n\t4. Withdrawl"
             << "\n\t5. Close an Account"
             << "\n\t6. Show All Accounts"
             << "\n\t7. Quit"
             << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\nEnter the first Name : ";
            cin >> fname;
            cout << "\nEnter the last Name : ";
            cin >> lname;
            cout << "\nEnter the initial Deposit : ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << "\nYour account is opened" << endl;
            cout << acc;
            break;
        case 2:
            cout << "Enter the account Number for the balance Enquiry";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << acc;
            break;
        case 3:
            cout << "Enter the account Number for the Depositing Money";
            cin >> accountNumber;
            cout << "Enter the Money you want to deposit";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << endl
                 << "Amount is Deposited" << endl;
            cout << acc;
            break;
        case 4:
            cout << "Enter the account Number for the Depositing Money";
            cin >> accountNumber;
            cout << "Enter the Money you want to Withdrawl";
            cin >> amount;
            acc = b.Withdaw(accountNumber, amount);
            cout << endl
                 << "Amount Withdrawn" << endl;
            cout << acc;
            break;
        case 5:
            cout << "Enter the account Number you want to close";
            cin >> accountNumber;
            b.CloseAccount(accountNumber);
            cout << endl
                 << "Account is Closed" << endl;
            cout << acc;
            break;
        case 6:
            b.ShowAllAcounts();
            break;
        case 7:
            break;

        default:
            cout << "\nEnter Correct choice";
            exit(0);
        }
    }
    return 0;
}

Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::Deposit(float amount)
{
    balance += amount;
}

void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
    {
        throw InsufficientBalance();
    }
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}
long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}
ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}
ostream &operator<<(ostream &os, Account &acc)
{
    os << "First Name:" << acc.getFirstName() << endl;
    os << "Last Name:" << acc.getLastName() << endl;
    os << "Account Number:" << acc.getAccNo() << endl;
    os << "Balance:" << acc.getBalance() << endl;
    return os;
}
Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        return;
    }
    while (!infile.eof())
    {
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccountNumber(account.getAccNo());

    infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
    outfile.open("Blank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}
Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    return itr->second;
}
Account Bank::Deposit(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}
Account Bank::Withdaw(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}
void Bank::CloseAccount(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    cout << "Account Deleted" << itr->second;
    accounts.erase(accountNumber);
}
void Bank::ShowAllAcounts()
{
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout << "Account" << itr->first << endl
             << itr->second << endl;
    }
}
Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
}

