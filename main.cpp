#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <map>
#include <tuple>
#include <cmath>


using namespace std;
void clearScreen() {
    system("clear");
}

bool userExists(const string& username) {
    ifstream userFile("users.txt");
    string u, p;
    while (userFile >> u >> p) {
        if (u == username) {
            return true;
        }
    }
    return false;
}
void wait(int ms) {

}
enum SplitType {
    EQUAL,
    EXACT,
    PERCENTAGE
};

class User {
private:
    string username;
    string password;
    const string userFilePath = "users.txt";


    static string hashPassword(const string &pw) {
        hash<string> hasher;
        return to_string(hasher(pw));
    }


    static bool isStrongPassword(const string &pw) {
        if (pw.length() < 8) {
            return false;
        }
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        for (const char ch : pw) {
            if (isupper(ch)) {
                hasUpper = true;
            }
            else if (islower(ch)) {
                hasLower = true;
            }
            else if (isdigit(ch)) {
                hasDigit = true;
            }
            else {
                hasSpecial = true;
            }
        }
        return hasUpper && hasLower && hasDigit && hasSpecial;
    }
public:
    string getUsername() const {
        return username;
    }
    bool signUp() {
        clearScreen();
        cout << "-----  Sign Up  -----\n";
        cout << "Enter a new username: ";
        cin >> username;

        cout << "Enter a new password: ";
        cin >> password;

        if (!isStrongPassword(password)) {
            cout << "\n Password must be at least 8 characters long and contain:\n"
                 << "- one uppercase letter\n"
                 << "- one lowercase letter\n"
                 << "- one digit\n"
                 << "- one special character\n";
            wait(2500);
            return false;
        }
        ifstream fin(userFilePath);
        string u, p;
        if (userExists(username)) {
            cout << "\n Username already exists\n";
            wait(2000);
            return false;
        }
        fin.close();
        ofstream fout(userFilePath, ios::app);
        fout << username << " " << hashPassword(password) << "\n";
        fout.close();


        string userInfoFile = username + ".txt";
        ofstream userOut(userInfoFile);
        userOut << "Username: " << username << "\n";
        userOut << "Groups:\n";
        userOut.close();
        cout << " Signup successful!\n";
        wait(1500);
        return true;
    }


    bool login() {
        clearScreen();
        cout << "-----  Login  -----\n";
        cout << "Enter username: ";
        cin >> username;
        ifstream fin(userFilePath);
        if (!fin.is_open()) {
            cout << " User data not found. Please sign up first.\n";
            wait(2000);
            return false;
        }
        string u, p, storedHash;
        bool userFound = false;
        while (fin >> u >> p) {
            if (u == username) {
                storedHash = p;
                userFound = true;
                break;
            }
        }
        fin.close();

        if (!userFound) {
            cout << " Username not found. Please sign up first.\n";
            wait(2000);
            return false;
        }

        int attempts = 3;
        while (attempts--) {
            cout << "Enter password: ";
            cin >> password;
            if (hashPassword(password) == storedHash) {
                cout << "Login successful!\n";
                wait(1500);
                return true;
            } else {
                if (attempts > 0)
                    cout << " Incorrect password. " << attempts << " attempt(s) left.\n";
            }
        }

        cout << " Login failed. Too many incorrect attempts.\n";
        wait(2000);
        return false;
    }
};


class Group {
    private:
        string groupId;
        string groupName;
        string admin;
        vector<string> members;

        string generateGroupId() {
            string path = "last_group_id.txt";
            int lastId = 1000;
            ifstream fin(path);
            if (fin.is_open()) {
                fin >> lastId;
                fin.close();
            }

            int newId = lastId + 1;
            ofstream fout(path);
            fout << newId;
            fout.close();
            return "GRP" + to_string(newId);
        }

        vector<string> searchUsersRabinKarp(string& pattern) {
            vector<string> matches;
            ifstream userFile("users.txt");
            string line;
            const int prime = 101; // A prime number
            int patternHash = 0;
            int patternLength = pattern.length();
            for (char c : pattern) {
                patternHash = (patternHash * 256 + c) % prime;
            }

            while (getline(userFile, line)) {
                string username = line.substr(0, line.find(' '));
                int textHash = 0;
                int textLength = username.length();
                if (username==admin) {
                    continue;
                }
                for (int i = 0; i < patternLength; i++) {
                    textHash = (textHash * 256 + username[i]) % prime;
                }
                for (int i = 0; i <= textLength - patternLength; i++) {
                    if (patternHash == textHash) {
                        bool match = true;
                        for (int j = 0; j < patternLength; j++) {
                            if (pattern[j] != username[i + j]) {
                                match = false;
                                break;
                            }
                        }
                        if (match) {
                            matches.push_back(username);
                            break;
                        }
                    }
                    if (i < textLength - patternLength) {
                        textHash = (256 * (textHash - username[i] * pow(256, patternLength-1)) + username[i + patternLength]);
                        textHash %= prime;
                        if (textHash < 0) {
                            textHash += prime;
                        }
                    }
                }
            }
            return matches;
        }
    public:

        Group(const string& adminName){
            admin = adminName;
        }
        string getAdmin() {
            return admin;
        }
        void setGroupId(const string& groupID) {
            groupId = groupID;
        }
        string getGroupID() {
            return groupId;
        }
        void createGroup() {
            clearScreen();
            cout << "----- CREATE NEW GROUP -----\n";
            cout << "Enter group name: ";
            cin.ignore();
            getline(cin, groupName);

            groupId = generateGroupId();
            members.push_back(admin);
            ofstream groupFile(groupId + ".txt");
            groupFile << "ID:" << groupId << "\n";
            groupFile << "Name:" << groupName << "\n";
            groupFile << "Admin:" << admin << "\n";
            groupFile << "Members:\n";
            for (const auto& member : members) {
                groupFile << member << "\n";
            }
            groupFile.close();
            ofstream adminFile(admin + ".txt", ios::app);
            adminFile << "Group:" << groupId << ":" << groupName << "\n";
            adminFile.close();

            cout << "\nGroup created successfully!\n";
            cout << "Group ID: " << groupId << "\n";
            wait(2000);
        }
        void openGroup() {
            clearScreen();
            cout << "Enter group id: ";
            cin.ignore();
            string gname;
            cin >> gname;
            ifstream groupFile(gname + ".txt");
            if (!groupFile) {
                cout << "Group not found!\n";
                wait(1500);
                return;
            }
            setGroupId(gname);
        }
        void addMember() {
            clearScreen();
            cin.ignore();
            cout << "----- ADD GROUP MEMBER -----\n";
            cout << "Enter group name: ";
            string gname;
            cin >> gname;

            ifstream groupFile(gname + ".txt");
            if (!groupFile) {
                cout << "Group not found!\n";
                wait(1500);
                return;
            }

            string line, currentAdmin, groupNameValue;
            while (getline(groupFile, line)) {
                if (line.find("Admin:") == 0) {
                    currentAdmin = line.substr(6);
                }
                else if (line.find("Name:") == 0) {
                    groupNameValue = line.substr(5);
                }
            }
            groupFile.close();

            if (currentAdmin != admin) {
                cout << "Only group admin can add members!\n";
                wait(1500);
                return;
            }

            cout << "Enter username pattern to search: ";
            string pattern;
            cin >> pattern;

            vector<string> matches = searchUsersRabinKarp(pattern);

            if (matches.empty()) {
                cout << "No matching users found.\n";
            } else {
                cout << "\nMatching Users:\n";
                for (int i = 0; i < matches.size(); i++) {
                    cout << i+1 << ". " << matches[i] << endl;
                }

                cout << "Enter number to add (0 to cancel): ";
                int choice;
                cin >> choice;

                if (choice > 0 && choice <= matches.size()) {
                    string newMember = matches[choice-1];
                    groupFile.open(gname + ".txt");
                    bool isMember = false;
                    while (getline(groupFile, line)) {
                        if (line == newMember) {
                            isMember = true;
                            break;
                        }
                    }
                    groupFile.close();

                    if (!isMember) {
                        ofstream outFile(gname + ".txt", ios::app);
                        outFile << newMember << "\n";
                        outFile.close();
                        ofstream userFile(newMember + ".txt", ios::app);
                        userFile << "Group:" << gname << ":"<< groupNameValue<<"\n";
                        userFile.close();

                        cout << newMember << " added successfully!\n";
                    } else {
                        cout << "User is already a member!\n";
                    }
                }
            }
            wait(2000);
        }

        void viewGroupDetails() {
            cout << "----- GROUP DETAILS -----\n";
            cout << "Enter group ID: ";
            string gid;
            cin >> gid;

            ifstream groupFile(gid + ".txt");
            if (!groupFile) {
                cout << "Group not found!\n";
                wait(1500);
                return;
            }

            string line;
            cout << "\n=== Group Information ===\n";
            while (getline(groupFile, line)) {
                if (line.find("ID:") == 0) {
                    cout << "Group ID: " << line.substr(3) << "\n";
                }
                else if (line.find("Name:") == 0) {
                    cout << "Group Name: " << line.substr(5) << "\n";
                }
                else if (line.find("Admin:") == 0) {
                    cout << "Admin: " << line.substr(6) << "\n";
                }
                else if (line == "Members:") {
                    cout << "\nMembers:\n";
                }
                else if (!line.empty() && line != "Members:" && line.find("EXPENSE") == string::npos) {
                    cout << "- " << line << "\n";
                }
            }
            cout << "=======================\n";
            wait(5000);
            clearScreen();
        }

        void listUserGroups() {
            clearScreen();
            cout << "----- YOUR GROUPS -----\n";

            ifstream userFile(admin + ".txt");
            if (!userFile) {
                cout << "No groups found!\n";
                wait(1500);
                return;
            }

            string line;
            bool found = false;
            cout << "\n";
            while (getline(userFile, line)) {
                if (line.find("Group:") == 0) {
                    size_t pos1 = line.find(":");
                    size_t pos2 = line.find(":", pos1+1);
                    string gid = line.substr(pos1+1, pos2-pos1-1);
                    string gname = line.substr(pos2+1);

                    cout << "ID: " << gid << " | Name: " << gname;
                    cout << "\n";
                    found = true;
                }
            }

            if (!found) {
                cout << "You are not in any groups yet.\n";
            }
            wait(5000);
        }

        void deleteGroup() {
            clearScreen();
            cout << "----- DELETE GROUP -----\n";
            cout << "Enter group ID: ";
            string gid;
            cin >> gid;

            ifstream groupFile(gid + ".txt");
            if (!groupFile) {
                cout << "Group not found!\n";
                wait(1500);
                return;
            }

            string line, currentAdmin, groupName;
            vector<string> groupMembers;

            while (getline(groupFile, line)) {
                if (line.find("Admin:") == 0) {
                    currentAdmin = line.substr(6);
                }
                else if (line.find("Name:") == 0) {
                    groupName = line.substr(5);
                }
                else if (line != "Members:" && !line.empty() && line.find("ID:") != 0 &&
                        line.find("EXPENSE:") != 0 && line.find("Admin:") != 0 && line.find("Name:") != 0) {
                    groupMembers.push_back(line);
                }
            }
            groupFile.close();

            if (currentAdmin != admin) {
                cout << "Only group admin can delete a group!\n";
                wait(1500);
                return;
            }

            cout << "\nAre you sure you want to delete group '" << groupName << "' (" << gid << ")? (y/n): ";
            char confirm;
            cin >> confirm;

            if (tolower(confirm) != 'y') {
                cout << "Deletion cancelled.\n";
                wait(1500);
                return;
            }

            // Remove the group reference from all member files
            for (const auto& member : groupMembers) {
                string userFilePath = member + ".txt";
                string tempFilePath = member + "_temp.txt";

                ifstream userFile(userFilePath);
                ofstream tempFile(tempFilePath);

                string userLine;
                while (getline(userFile, line)) {
                    if (line.find("Group:" + gid + ":") != 0) {
                        tempFile << line << "\n";
                    }
                }

                userFile.close();
                tempFile.close();

                remove(userFilePath.c_str());
                rename(tempFilePath.c_str(), userFilePath.c_str());
            }

            // Delete the group file
            if (remove((gid + ".txt").c_str()) != 0) {
                cout << "Error deleting group file!\n";
            } else {
                cout << "Group deleted successfully!\n";
            }

            wait(2000);
        }
    };
class DebtGraph {
private:
    map<string, map<string, double>> adjacencyList;  // From user -> To user -> Amount

public:
    static bool comp(const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    }
    void addDebt(const string& from, const string& to, double amount) {
        if (adjacencyList[from].count(to) > 0) {
            adjacencyList[from][to] += amount;
        } else {
            adjacencyList[from][to] = amount;
        }
    }
    map<string, double> getNetBalances() const {
        map<string, double> netBalances;

        for (const auto& [from, toMap] : adjacencyList) {
            for (const auto& [to, amount] : toMap) {
                netBalances[from] -= amount;
                netBalances[to] += amount;
            }
        }

        return netBalances;
    }

    vector<tuple<string, string, double>> calculateMinimumTransactions() {
        map<string, double> netBalances;

        for (const auto& [from, toMap] : adjacencyList) {
            for (const auto& [to, amount] : toMap) {
                netBalances[from] -= amount;
                netBalances[to] += amount;
            }
        }
        vector<tuple<string, string, double>> transactions;
        vector<pair<string, double>> negative, positive;

        for (const auto& [user, balance] : netBalances) {
            if (balance < 0) {
                negative.emplace_back(user, -balance);
            } else if (balance > 0) {
                positive.emplace_back(user, balance);
            }
        }

        while (!negative.empty() && !positive.empty()) {
            sort(negative.begin(), negative.end(),comp);
            sort(positive.begin(), positive.end(),comp);

            string fromUser = negative[0].first;
            double& fromAmount = negative[0].second;

            string toUser = positive[0].first;
            double& toAmount = positive[0].second;
            double transferAmount = min(fromAmount, toAmount);
            transactions.emplace_back(fromUser, toUser, transferAmount);

            fromAmount -= transferAmount;
            toAmount -= transferAmount;

            if (fromAmount < 0.01) {
                negative.erase(negative.begin());
            }
            if (toAmount < 0.01) {
                positive.erase(positive.begin());
            }
        }

        return transactions;
    }
};
class Expense {
private:
    string expenseId;
    string description;
    double amount;
    string paidBy;
    vector<string> participants;
    SplitType splitType;
    map<string, double> amountPerUser;

    void generateId() {
        expenseId = "EXP_" + to_string(time(nullptr)) + "_" + to_string(rand() % 1000);
    }

public:
    Expense(const string& currentUser, ofstream& groupFile) {
        clearScreen();
        cout << "----- CREATE EXPENSE -----\n";

        cout << "Description: ";
        cin.ignore();
        getline(cin, description);

        cout << "Amount: ";
        while (!(cin >> amount) || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Enter positive value: ";
        }
        paidBy = currentUser;
        a:
        cout << "Add participants (space-separated usernames):\n> ";
        string line;
        cin.ignore();
        getline(cin, line);
        stringstream ss(line);
        string participant;
        while (ss >> participant) {
            if (userExists(participant)) {
                participants.push_back(participant);
            }
            else {
                cout << participant << " does not exist!\n";
                cout << "Please try again!\n";
                goto a;
            }
        }
        int splitChoice;
        cout << "\nSplit type:\n";
        cout << "1. Equal split\n";
        cout << "2. Exact amounts\n";
        cout << "3. Percentage split\n";
        cout << "Choice: ";
        cin >> splitChoice;

        switch (splitChoice) {
            case 1:
                splitType = EQUAL;
                for (const auto& p : participants) {
                    amountPerUser[p] = amount / participants.size();
                }
                break;

            case 2: {
                splitType = EXACT;
                double totalEntered = 0;
                for (const auto& p : participants) {
                    if (p == paidBy) continue;

                    double userAmount;
                    cout << "Amount for " << p << ": ";
                    cin >> userAmount;
                    amountPerUser[p] = userAmount;
                    totalEntered += userAmount;
                }
                amountPerUser[paidBy] = amount - totalEntered;
                if (amountPerUser[paidBy] < 0) {
                    cout << "Warning: Sum of exact amounts exceeds total. Adjusting...\n";
                    amountPerUser[paidBy] = 0;
                }
                break;
            }

            case 3: {
                splitType = PERCENTAGE;
                double totalPercentage = 0;
                for (const auto& p : participants) {
                    if (p == paidBy) continue;
                    double percentage;
                    cout << "Percentage for " << p << " (%): ";
                    cin >> percentage;
                    amountPerUser[p] = (percentage / 100.0) * amount;
                    totalPercentage += percentage;
                }
                amountPerUser[paidBy] = ((100 - totalPercentage) / 100.0) * amount;
                if (totalPercentage > 100) {
                    cout << "Warning: Total percentage exceeds 100%. Adjusting...\n";
                    amountPerUser[paidBy] = 0;
                }
                break;
            }
            default:
                cout << "Invalid choice. Using equal split.\n";
                splitType = EQUAL;
                for (const auto& p : participants) {
                    amountPerUser[p] = amount / participants.size();
                }
                break;
        }

        generateId();

        groupFile << "EXPENSE:" << expenseId << ":"
                 << description << ":"
                 << amount << ":"
                 << paidBy << ":";
        groupFile << splitType << ":";
        for (const auto& p : participants) {
            groupFile << p << ",";
        }
        groupFile << ":";
        for (const auto& [user, userAmount] : amountPerUser) {
            groupFile << user << "=" << userAmount << ",";
        }
        groupFile << "\n";
    }

    Expense(string expenseData) {
        vector<string> parts;
        size_t start = 8;
        size_t end = expenseData.find(':', start);

        while (end != string::npos) {
            parts.push_back(expenseData.substr(start, end - start));
            start = end + 1;
            end = expenseData.find(':', start);
        }
        parts.push_back(expenseData.substr(start));
        expenseId = parts[0];
        description = parts[1];
        amount = stod(parts[2]);
        paidBy = parts[3];
        splitType = static_cast<SplitType>(stoi(parts[4]));
        string participantsStr = parts[5];
        size_t commaPos = 0;
        while ((commaPos = participantsStr.find(',')) != string::npos) {
            participants.push_back(participantsStr.substr(0, commaPos));
            participantsStr.erase(0, commaPos + 1);
        }

        string amountsStr = parts[6];
        commaPos = 0;
        while ((commaPos = amountsStr.find(',')) != string::npos) {
            string userAmount = amountsStr.substr(0, commaPos);
            size_t equalPos = userAmount.find('=');
            if (equalPos != string::npos) {
                string user = userAmount.substr(0, equalPos);
                double amount = stod(userAmount.substr(equalPos + 1));
                amountPerUser[user] = amount;
            }
            amountsStr.erase(0, commaPos + 1);
        }
    }
    static void viewExpenses(const string& groupFilename) {
        clearScreen();
        cout << "----- EXPENSE HISTORY -----\n";

        ifstream file(groupFilename);
        string line;
        bool found = false;

        while (getline(file, line)) {
            if (line.find("EXPENSE:") == 0) {
                found = true;
                Expense expense(line);

                cout << "\nID: " << expense.expenseId
                     << "\nDesc: " << expense.description
                     << "\nAmount: ₹" << fixed << setprecision(2) << expense.amount
                     << "\nPaid by: " << expense.paidBy
                     << "\nSplit type: ";

                switch (expense.splitType) {
                    case EQUAL: cout << "Equal"; break;
                    case EXACT: cout << "Exact amounts"; break;
                    case PERCENTAGE: cout << "Percentage"; break;
                }

                cout << "\nParticipants: ";
                for (const auto& p : expense.participants) {
                    cout << p << " ";
                }

                cout << "\nAmount per user:";
                for (const auto& [user, userAmount] : expense.amountPerUser) {
                    cout << "\n  " << user << ": ₹" << fixed << setprecision(2) << userAmount;
                }
                cout << "\n";
            }
        }

        if (!found) {
            cout << "No expenses found.\n";
        }

        cout << "\nPress enter to continue...";
        cin.ignore();
        cin.get();
    }

    static void viewExpenseSummary(const string& groupFilename) {
        clearScreen();
        cout << "----- EXPENSE SUMMARY -----\n";
        DebtGraph debtGraph;
        ifstream file(groupFilename);
        string line;
        bool found = false;

        while (getline(file, line)) {
            if (line.find("EXPENSE:") == 0) {
                found = true;
                Expense expense(line);
                string payer = expense.getPayer();
                for (const auto& [user, amount] : expense.getAmountPerUser()) {
                    if (user != payer && amount > 0) {
                        debtGraph.addDebt(user, payer, amount);
                    }
                }
            }
        }

        if (!found) {
            cout << "No expenses found.\n";
            return;
        }
        map<string, double> balances = debtGraph.getNetBalances();
        cout << "\nCurrent balances:\n";
        for (const auto& [user, balance] : balances) {
            cout << user << ": ₹" << fixed << setprecision(2) << balance;
            if (balance > 0) {
                cout << " (to receive)";
            } else if (balance < 0) {
                cout << " (to pay)";
            } else {
                cout << " (settled)";
            }
            cout << "\n";
        }
        cout << "\nSettlement plan (minimum transactions):\n";
        auto transactions = debtGraph.calculateMinimumTransactions();

        for (const auto& [from, to, amount] : transactions) {
            cout << from << " pays ₹" << fixed << setprecision(2) << amount
                 << " to " << to << "\n";
        }

        cout << "\nPress enter to continue...";
        cin.ignore();
        cin.get();
    }

    string getId() const { return expenseId; }
    string getDesc() const { return description; }
    double getAmount() const { return amount; }
    string getPayer() const { return paidBy; }
    const map<string, double>& getAmountPerUser() const {
        return amountPerUser;
    }
};


void expenseSubMenu(const string& currentUser, const string& groupId) {
    int choice;
    do {
        clearScreen();
        cout << "----- EXPENSE MENU -----\n"
             << "1. Create Expense\n"
             << "2. View Expenses\n"
             << "3. View Expense Summary\n"
             << "4. Exit\n"
             << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                ofstream groupFile(groupId + ".txt", ios::app);
                Expense(currentUser, groupFile);
                groupFile.close();
                cout << "Expense added!\n";
                wait(1500);
                break;
            }
            case 2:
                Expense::viewExpenses(groupId + ".txt");
                break;
            case 3:
                Expense::viewExpenseSummary(groupId + ".txt");
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice\n";
                wait(1000);
        }
    } while (true);
}

void showLogo() {
    cout << "=====================================================\n";
    cout << "               .__  .__  __          .__               \n";
    cout << "  ____________ |  | |__|/  |___  _  _|__| ______ ____  \n";
    cout << " /  ___/\\____ \\|  | |  \\   __\\ \\/ \\/ /  |/  ___// __ \\ \n";
    cout << " \\___ \\ |  |_> >  |_|  ||  |   \\     /|  |\\___ \\\\  ___/ \n";
    cout << " /____  >|   __/|____/__||__|    \\/\\_/ |__/____  >\\___  >\n";
    cout << "     \\/ |__|                                 \\/     \\/ \n";
    cout << "=====================================================\n";
}


void showMainMenu() {
    clearScreen();
    showLogo();
    cout << "\n            Welcome to SplitWise \n";
    cout << "-----------------------------------------------------\n";
    cout << "                  1. Sign Up\n";
    cout << "                  2. Login\n";
    cout << "                  3. Exit\n";
    cout << "-----------------------------------------------------\n";
    cout << "Enter your choice: ";
}


void loggedInMenu(User& user) {
    Group group(user.getUsername());
    int choice;

    while (true) {
        clearScreen();
        cout << "----- MAIN MENU (" << user.getUsername() << ") -----\n";
        cout << "1. Create New Group\n";
        cout << "2. Add Member to Group\n";
        cout << "3. View Group Details\n";
        cout << "4. List Your Groups\n";
        cout << "5. Open Group\n";
        cout << "6. Delete Group\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;


        switch (choice) {
            case 1: group.createGroup(); break;
            case 2: {
                group.listUserGroups();
                group.addMember();
                break;
            }
            case 3: {
                group.listUserGroups();
                group.viewGroupDetails();
                break;
            }
            case 4: group.listUserGroups(); break;
            case 5: {
                group.listUserGroups();
                group.openGroup();
                expenseSubMenu(group.getAdmin(),group.getGroupID());
                break;
            }
            case 6: {
                group.listUserGroups();
                group.deleteGroup();
                break;
            }
            case 7: return;
            default:
                cout << "Invalid choice!\n";
                wait(1000);
        }
    }
}

int main() {
    User user;
    int choice;
    while (true) {
        showMainMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                user.signUp();
                break;
            case 2:
                if (user.login()) {
                    loggedInMenu(user);
                }
                break;
            case 3:
                cout << " Exiting... Goodbye!\n";
                wait(1000);
                return 0;
            default:
                cout << " Invalid choice. Try again.\n";
                wait(1500);
        }
    }
}