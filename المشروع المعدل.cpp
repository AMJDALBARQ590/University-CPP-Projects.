 #include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// [3.3] استخدام Namespace مخصص للمشروع
namespace StorageSystem {

    // [3.1] هيكل المستخدم (User)
    struct User {
        int id;             
        string name;
        string username;
        string password;
        string type;        
        string phone;
        bool status;        
        int activityCount;  
    };

    // [3.1] هيكل السجل (Record)
    struct Record {
        string name;
        double price;
        int quantity;
        string extraData;   
    };

    const int MAX = 100;
    User users[MAX];
    Record records[MAX];
    int userCount = 0;
    int recordCount = 0;
}

using namespace StorageSystem;

// --- [3.3] استخدام متغيرات Static ---
void systemStatusInfo() {
    static int systemLaunchCount = 0;
    systemLaunchCount++;
    cout << "\n[ System Session Info ]" << endl;
    cout << "Login attempts in this session: " << systemLaunchCount << endl;
}

// --- [3.2] وظائف التعامل مع الملفات (مفصلة لزيادة الحجم) ---

void saveUsersData() {
    ofstream f("users.txt");
    if (f.is_open()) {
        for (int i = 0; i < userCount; i++) {
            f << users[i].id << " " << users[i].name << " " << users[i].username << " "
              << users[i].password << " " << users[i].type << " " << users[i].phone << " "
              << users[i].status << " " << users[i].activityCount << endl;
        }
        f.close();
    }
}

void loadUsersData() {
    ifstream f("users.txt");
    userCount = 0;
    if (f.is_open()) {
        while (f >> users[userCount].id >> users[userCount].name >> users[userCount].username 
                 >> users[userCount].password >> users[userCount].type >> users[userCount].phone 
                 >> users[userCount].status >> users[userCount].activityCount) {
            userCount++;
        }
        f.close();
    }
}

void saveRecordsData() {
    ofstream f("records.txt");
    if (f.is_open()) {
        for (int i = 0; i < recordCount; i++) {
            f << records[i].name << " " << records[i].price << " " 
              << records[i].quantity << " " << records[i].extraData << endl;
        }
        f.close();
    }
}

void loadRecordsData() {
    ifstream f("records.txt");
    recordCount = 0;
    if (f.is_open()) {
        while (f >> records[recordCount].name >> records[recordCount].price 
                 >> records[recordCount].quantity >> records[recordCount].extraData) {
            recordCount++;
        }
        f.close();
    }
}

void addToActivityLog(string uName, string op) {
    ofstream f("activities.txt", ios::app);
    if (f.is_open()) {
        f << "Time: [Auto] | User: " << uName << " | Action: " << op << endl;
        f.close();
    }
}

// --- [2.2] وظائف المدير (Admin) - مفصلة جداً ---

void printAdminHeader() {
    cout << "\n========================================" << endl;
    cout << "      ADMINISTRATION CONTROL PANEL      " << endl;
    cout << "========================================" << endl;
}

void addNewUser() {
    printAdminHeader();
    if (userCount >= MAX) {
        cout << "Error: System storage for users is full!" << endl;
        return;
    }
    int newId;
    cout << "-> Enter User Unique ID: "; cin >> newId;
    for(int i=0; i<userCount; i++) {
        if(users[i].id == newId) { 
            cout << "!! Error: This ID already exists in the system." << endl; 
            return; 
        }
    }
    users[userCount].id = newId;
    cout << "-> Enter Full Name: "; cin.ignore(); getline(cin, users[userCount].name);
    cout << "-> Enter Username: "; cin >> users[userCount].username;
    cout << "-> Enter Password: "; cin >> users[userCount].password;
    cout << "-> Type (Admin/Employee): "; cin >> users[userCount].type;
    cout << "-> Enter Phone Number: "; cin >> users[userCount].phone;
    users[userCount].status = true; 
    users[userCount].activityCount = 0;
    userCount++;
    saveUsersData();
    cout << ">> Result: New account created successfully." << endl;
}

void searchForUser() {
    cout << "\n[ Search Tool ]" << endl;
    string target;
    cout << "Enter username to search for: "; cin >> target;
    bool found = false;
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == target) {
            cout << "---------------------------" << endl;
            cout << "ID: " << users[i].id << endl;
            cout << "Name: " << users[i].name << endl;
            cout << "Status: " << (users[i].status ? "Active (On)" : "Disabled (Off)") << endl;
            cout << "---------------------------" << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "!! No user found with this name." << endl;
}

void editUserAccount() {
    int targetId;
    cout << "Enter User ID to modify: "; cin >> targetId;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == targetId) {
            cout << "1. Change Password\n2. Change Phone\n3. Change Name\nChoice: ";
            int subChoice; cin >> subChoice;
            if(subChoice == 1) { cout << "New Password: "; cin >> users[i].password; }
            else if(subChoice == 2) { cout << "New Phone: "; cin >> users[i].phone; }
            else if(subChoice == 3) { cout << "New Name: "; cin.ignore(); getline(cin, users[i].name); }
            saveUsersData();
            cout << "Data updated." << endl;
            return;
        }
    }
    cout << "User ID not found." << endl;
}

void deleteUserAccount() {
    int targetId;
    cout << "Enter ID to permanently delete: "; cin >> targetId;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == targetId) {
            cout << "WARNING: Are you sure you want to delete [" << users[i].name << "]? (y/n): ";
            char confirm; cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < userCount - 1; j++) {
                    users[j] = users[j + 1];
                }
                userCount--;
                saveUsersData();
                cout << ">> Success: User removed from system." << endl;
            } else {
                cout << "Operation cancelled." << endl;
            }
            return;
        }
    }
    cout << "!! User ID not found." << endl;
}

void changeUserStatus(bool newState) {
    int targetId;
    cout << "Enter User ID: "; cin >> targetId;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == targetId) {
            users[i].status = newState;
            saveUsersData();
            cout << ">> Account status has been updated." << endl;
            return;
        }
    }
    cout << "!! User ID not found." << endl;
}

void generateFullReport() {
    int active = 0, disabled = 0;
    cout << "\n" << setfill('=') << setw(50) << "=" << endl;
    cout << left << setw(10) << "ID" << setw(15) << "Username" << setw(12) << "Type" << "Status" << endl;
    cout << setfill('-') << setw(50) << "-" << endl;
    for (int i = 0; i < userCount; i++) {
        cout << left << setw(10) << users[i].id << setw(15) << users[i].username 
             << setw(12) << users[i].type << (users[i].status ? "ON" : "OFF") << endl;
        if (users[i].status) active++; else disabled++;
    }
    cout << setfill('=') << setw(50) << "=" << endl;
    cout << "Summary: " << active << " Active Users | " << disabled << " Disabled Users." << endl;
}

// --- [2.3] وظائف الموظف (Employee) - مفصلة جداً ---

// [3.3] استخدام المؤشرات (Pointers) لتعديل السجلات
void modifyRecordDetails(Record* ptr) {
    cout << "Current Item: " << ptr->name << endl;
    cout << "Enter New Price: "; cin >> ptr->price;
    cout << "Enter New Quantity: "; cin >> ptr->quantity;
    saveRecordsData();
}

void addNewRecord(string currentU) {
    if (recordCount >= MAX) {
        cout << "Error: Warehouse is full!" << endl;
        return;
    }
    cout << "\n--- Inventory Entry ---" << endl;
    cout << "Item Name: "; cin >> records[recordCount].name;
    cout << "Price Per Unit: "; cin >> records[recordCount].price;
    cout << "Initial Quantity: "; cin >> records[recordCount].quantity;
    cout << "Extra Details (Model/Type): "; cin.ignore(); getline(cin, records[recordCount].extraData);
    
    // تحديث نشاط المستخدم
    for(int i=0; i<userCount; i++) {
        if(users[i].username == currentU) users[i].activityCount++;
    }
    recordCount++;
    addToActivityLog(currentU, "Added_New_Item");
    saveRecordsData();
    cout << ">> Record saved to database." << endl;
}

void findRecord() {
    string searchName;
    cout << "Enter item name to locate: "; cin >> searchName;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].name == searchName) {
            cout << "\n--- Item Found ---" << endl;
            cout << "Name: " << records[i].name << "\nPrice: " << records[i].price 
                 << "\nStock: " << records[i].quantity << "\nDetails: " << records[i].extraData << endl;
            return;
        }
    }
    cout << "!! Item not found in inventory." << endl;
}

void sortInventoryItems() {
    // Bubble Sort لترتيب السجلات (حسب الاسم تصاعدياً)
    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (records[j].name > records[j + 1].name) {
                Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    cout << ">> Success: Inventory sorted alphabetically." << endl;
}

// --- [منطق القوائم] ---

void displayAdminMenu(string u) {
    int choice;
    do {
        cout << "\n************************************" << endl;
        cout << "         ADMIN MAIN MENU            " << endl;
        cout << "************************************" << endl;
        cout << "1. Add New System User" << endl;
        cout << "2. Modify Existing User" << endl;
        cout << "3. Remove User (Delete)" << endl;
        cout << "4. Search for User Info" << endl;
        cout << "5. Enable User (Status ON)" << endl;
        cout << "6. Disable User (Status OFF)" << endl;
        cout << "7. View Full Users Report" << endl;
        cout << "0. Exit Admin Panel" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addNewUser(); break;
            case 2: editUserAccount(); break;
            case 3: deleteUserAccount(); break;
            case 4: searchForUser(); break;
            case 5: changeUserStatus(true); break;
            case 6: changeUserStatus(false); break;
            case 7: generateFullReport(); break;
            case 0: cout << "Logging out from admin..." << endl; break;
            default: cout << "Invalid selection!" << endl;
        }
    } while (choice != 0);
}

void displayEmployeeMenu(string u) {
    int choice;
    do {
        cout << "\n====================================" << endl;
        cout << "       EMPLOYEE OPERATIONS          " << endl;
        cout << "====================================" << endl;
        cout << "1. Register New Item Record" << endl;
        cout << "2. Edit Record (Using Pointers)" << endl;
        cout << "3. Search for Item Record" << endl;
        cout << "4. Show Total Items Count" << endl;
        cout << "5. Sort All Items by Name" << endl;
        cout << "0. Exit Employee Panel" << endl;
        cout << "Your Selection: ";
        cin >> choice;

        if (choice == 1) addNewRecord(u);
        else if (choice == 2) {
            string n; cout << "Item Name to edit: "; cin >> n;
            for(int i=0; i<recordCount; i++) if(records[i].name == n) modifyRecordDetails(&records[i]);
        }
        else if (choice == 3) findRecord();
        else if (choice == 4) cout << ">> Total items in database: " << recordCount << endl;
        else if (choice == 5) sortInventoryItems();
    } while (choice != 0);
}

// --- [الدالة الرئيسية] ---

int main() {
    // تحميل البيانات من الملفات عند بدء التشغيل [3.2]
    loadUsersData();
    loadRecordsData();
    
    // استدعاء دالة الـ Static لعرض معلومات الجلسة [3.3]
    systemStatusInfo();

    // إنشاء مستخدم مدير افتراضي إذا كان النظام جديداً
    if (userCount == 0) {
        users[0].id = 101;
        users[0].name = "Default Admin";
        users[0].username = "admin";
        users[0].password = "123";
        users[0].type = "Admin";
        users[0].phone = "000000";
        users[0].status = true;
        users[0].activityCount = 0;
        userCount = 1;
        saveUsersData();
    }

    while (true) {
        cout << "\n------------------------------------" << endl;
        cout << "    IT PROJECT: STORAGE SYSTEM      " << endl;
        cout << "------------------------------------" << endl;
        cout << "1. System Login" << endl;
        cout << "2. Quick User Register" << endl;
        cout << "3. Shutdown System" << endl;
        cout << "Choice: ";
        int mainM; cin >> mainM;

        if (mainM == 1) {
            string u, p;
            cout << "Enter Username: "; cin >> u;
            cout << "Enter Password: "; cin >> p;

            bool authenticated = false;
            for (int i = 0; i < userCount; i++) {
                if (users[i].username == u && users[i].password == p) {
                    if (!users[i].status) {
                        cout << "!! Access Denied: This account is disabled." << endl;
                        authenticated = true; // وجدنا الحساب لكنه معطل
                        break;
                    }
                    authenticated = true;
                    addToActivityLog(u, "System_Login");
                    
                    if (users[i].type == "Admin") displayAdminMenu(u);
                    else displayEmployeeMenu(u);
                    
                    addToActivityLog(u, "System_Logout");
                    break;
                }
            }
            if (!authenticated) cout << "!! Error: Invalid username or password." << endl;
        } 
        else if (mainM == 2) addNewUser();
        else if (mainM == 3) break;
    }

    cout << "\nClosing System... Data has been saved." << endl;
    return 0;
}