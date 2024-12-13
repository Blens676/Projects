#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

const int MAX_BOOKS = 100;
const int MAX_MEMBERS = 100;

string books[MAX_BOOKS];
bool availability[MAX_BOOKS];
bool loaned[MAX_BOOKS];
string borrowers[MAX_BOOKS];
time_t dueDates[MAX_BOOKS];
string members[MAX_MEMBERS];
string contactInfo[MAX_MEMBERS];
string isbns[MAX_BOOKS];
string authors[MAX_MEMBERS];
string returner[MAX_BOOKS];
bool referenceMaterial[MAX_BOOKS];
int numCheckouts[MAX_BOOKS] = {0};
int numActiveMembers[MAX_MEMBERS] = {0};

int numBooks = 0;
int numMembers = 0;
double lateFinePerDay = 0.5;  // Fine amount per day for late returns
double lostDamagedBookFee = 10.0;  // Fee for lost or damaged books

int main() {

    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // Perform authentication logic here
    if (username != "admin" || password != "123") {
        cout << "Invalid USERNAME" << endl;
        return 0;
    } else {
        system("cls");
    }

    while (true) {
        cout << "  \n ************ Library App ************" << endl;
        cout << "     1. Add New Book" << endl;
        cout << "     2. Remove Book" << endl;
        cout << "     3. Search for Book" << endl;
        cout << "     4. Check Out Book" << endl;
        cout << "     5. Return Book" << endl;
        cout << "     6. View Member's Checked-out Books" << endl;
        cout << "     7. Add Member" << endl;
        cout << "     8. Modify Member Details" << endl;
        cout << "     9. Generate Overdue Notices" << endl;
        cout << "     10. Generate Reports" << endl;
        cout << "     11. Exit" << endl;
        cout << " *******************************************" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {

        case 1: {
            if (numBooks >= MAX_BOOKS) {
                cout << "Library is full. Cannot add more books." << endl;
            } else {
                string bookTitle;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, bookTitle);

                string isbn;
                cout << "Enter ISBN: ";
                getline(cin, isbn);

                string author;
                cout << "Enter author: ";
                getline(cin, author);

                bool isReference;
                cout << "Is it a reference material? (0 = No, 1 = Yes): ";
                cin >> isReference;

                books[numBooks] = bookTitle;
                isbns[numBooks] = isbn;
                authors[numBooks] = author;
                referenceMaterial[numBooks] = isReference;

                availability[numBooks] = true;
                loaned[numBooks] = false;
                borrowers[numBooks] = "";
                numBooks++;
                system("cls");
                cout << "\nBook added successfully." << endl;
            }
            break;
        }

        case 2: {
            if (numBooks == 0) {
                system("cls");
                cout << "\nLibrary is empty. No books to remove." << endl;
            } else {
                string bookTitle;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, bookTitle);

                bool found = false;
                for (int i = 0; i < numBooks; i++) {
                    if (books[i] == bookTitle) {
                        found = true;
                        if (availability[i]) {
                            for (int j = i; j < numBooks - 1; j++) {
                                books[j] = books[j + 1];
                                isbns[j] = isbns[j + 1];
                                authors[j] = authors[j + 1];
                                availability[j] = availability[j + 1];
                                loaned[j] = loaned[j + 1];
                                borrowers[j] = borrowers[j + 1];
                                referenceMaterial[j] = referenceMaterial[j + 1];
                            }
                            numBooks--;
                            system("cls");
                            cout << "\nBook removed successfully." << endl;
                        } else {
                            system("cls");
                            cout << "\nBook is currently checked out. Cannot remove." << endl;
                        }
                        break;
                    }
                }
                if (!found) {
                    system("cls");
                    cout << "\nBook not found in the library." << endl;
                }
            }
            break;
        }

        case 3: {
            if (numBooks == 0) {
                system("cls");
                cout << "\nLibrary is empty. No books to search." << endl;
            } else {
                int searchOption;
        cout << "Search by:" << endl;
        cout << "1. Book Title" << endl;
        cout << "2. ISBN" << endl;
        cout << "3. Author" << endl;
        cout << "Enter your choice: ";
        cin >> searchOption;

        bool found = false;
        string searchValue;

        switch (searchOption) {
            case 1:
                system("cls");
                cout << "Enter book title to search: ";
                cin.ignore();
                getline(cin, searchValue);
                for (int i = 0; i < numBooks; i++) {
                    if (books[i] == searchValue) {
                        found = true;
                        system("cls");
                        cout << "\nBook details:" << endl;
                        cout << "Title: " << books[i] << endl;
                        cout << "ISBN: " << isbns[i] << endl;
                        cout << "Author: " << authors[i] << endl;
                        cout << "Availability: " << (availability[i] ? "Available" : "Checked Out") << endl;
                        cout << "Reference Material: " << (referenceMaterial[i] ? "Yes" : "No") << endl;
                        break;
                    }
                }
                    case 2:
                system("cls");
                cout << "Enter ISBN to search: ";
                cin.ignore();
                getline(cin, searchValue);
                for (int i = 0; i < numBooks; i++) {

                    if (isbns[i] == searchValue) {
                        found = true;
                         system("cls");
                        cout << "\nBook details:" << endl;
                        cout << "Title: " << books[i] << endl;
                        cout << "ISBN: " << isbns[i] << endl;
                        cout << "Author: " << authors[i] << endl;
                        cout << "Availability: " << (availability[i] ? "Available" : "Checked Out") << endl;
                        cout << "Reference Material: " << (referenceMaterial[i] ? "Yes" : "No") << endl;
                        break;
                    }
                }


            break;

            case 3:
                system("cls");
                cout << "Enter author to search: ";
                cin.ignore();
                getline(cin, searchValue);
                for (int i = 0; i < numBooks; i++) {
                    if (authors[i] == searchValue) {
                        found = true;
                        system("cls");
                        cout << "\nBook details:" << endl;
                        cout << "Title: " << books[i] << endl;
                        cout << "ISBN: " << isbns[i] << endl;
                        cout << "Author: " << authors[i] << endl;
                        cout << "Availability: " << (availability[i] ? "Available" : "Checked Out") << endl;
                        cout << "Reference Material: " << (referenceMaterial[i] ? "Yes" : "No") << endl;
                        break;
                    }


        }
                }
                if (!found) {
                    system("cls");
                    cout << "\nBook not found in the library." << endl;
                }
            }
        }


        case 4: {
            if (numBooks == 0) {
                system("cls");
                cout << "\nLibrary is empty. No books to check out." << endl;
            } else {
                string bookTitle;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, bookTitle);

                bool found = false;
                for (int i = 0; i < numBooks; i++) {
                    if (books[i] == bookTitle && availability[i] && !loaned[i]) {
                        found = true;

                        if (referenceMaterial[i]) {
                    system("cls");
                    cout << "\nReference materials cannot be checked out." << endl;
                    break;
                }

                        string memberName;
                        cout << "Enter member name: ";
                        getline(cin, memberName);

                        bool memberFound = false;
                        int memberIndex = 0;
                        for (int j = 0; j < numMembers; j++) {
                            if (members[j] == memberName) {
                                memberFound = true;
                                memberIndex = j;
                                break;
                            }
                        }

                        if (!memberFound) {
                            system("cls");
                            cout << "\nMember not found. Please add the member before checking out a book." << endl;
                            break;
                        }

                        time_t currentTime = time(nullptr);
                        dueDates[i] = currentTime + (1); // 14 days from current time
                        borrowers[i] = memberName;
                        availability[i] = false;
                        loaned[i] = true;
                        numCheckouts[i]++;
                        numActiveMembers[memberIndex]++;
                        system("cls");
                        cout << "\nBook checked out successfully." << endl;
                        break;
                    } else if (books[i] == bookTitle && !availability[i]) {
                        system("cls");
                        cout << "\nBook is currently checked out. Cannot check out." << endl;
                        break;
                    }
                }
                if (!found) {
                    system("cls");
                    cout << "\nBook not found in the library." << endl;
                }
            }
            break;
        }

        case 5: {
            if (numBooks == 0) {
                system("cls");
                cout << "\nLibrary is empty. No books to return." << endl;
            } else {
                string bookTitle;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, bookTitle);

                bool found = false;
                for (int i = 0; i < numBooks; i++) {
                    if (books[i] == bookTitle && loaned[i]) {
                        found = true;

                        time_t currentTime = time(nullptr);
                        double secondsLate = difftime(currentTime, dueDates[i]);
                        int daysLate = static_cast<int>(secondsLate / (24 * 60 * 60));

                        if (daysLate > 0) {
                            double fine = daysLate * lateFinePerDay;
                            cout << "\nBook returned " << daysLate << " day(s) late." << endl;
                            cout << "Late fine: $" << fixed << setprecision(2) << fine << endl;
                        }

                        borrowers[i] = "";
                        availability[i] = true;
                        loaned[i] = false;
                        returner[i] = "";
                        numActiveMembers[i]--;
                        system("cls");
                        cout << "\nBook returned successfully." << endl;
                        break;
                    } else if (books[i] == bookTitle && !loaned[i]) {
                        system("cls");
                        cout << "\nBook is already returned." << endl;
                        break;
                    }
                }
                if (!found) {
                    system("cls");
                    cout << "\nBook not found in the library." << endl;
                }
            }
            break;
        }

        case 6: {
            if (numMembers == 0) {
                system("cls");
                cout << "\nNo members found." << endl;
            } else {
                string memberName;
                cout << "Enter member name: ";
                cin.ignore();
                getline(cin, memberName);

                bool found = false;
                for (int i = 0; i < numBooks; i++) {
                    if (borrowers[i] == memberName) {
                        found = true;
                        system("cls");
                        cout << "\nChecked-out books for member " << memberName << ":" << endl;
                        cout << "Book: " << books[i] << endl;
                        cout << "Due Date: " << asctime(localtime(&dueDates[i]));
                    }
                }
                if (!found) {
                    system("cls");
                    cout << "\nMember not found or no books checked out by the member." << endl;
                }
            }
            break;
        }

        case 7: {
            if (numMembers >= MAX_MEMBERS) {
                system("cls");
                cout << "\nMaximum member limit reached. Cannot add more members." << endl;
            } else {
                string memberName;
                cout << "Enter member name: ";
                cin.ignore();
                getline(cin, memberName);

                string contact;
                cout << "Enter contact info: ";
                getline(cin, contact);

                members[numMembers] = memberName;
                contactInfo[numMembers] = contact;
                numMembers++;
                system("cls");
                cout << "\nMember added successfully." << endl;
            }
            break;
        }

        case 8: {
            if (numMembers == 0) {
                system("cls");
                cout << "\nNo members found. Cannot modify member details." << endl;
            } else {
                string memberName;
                cout << "Enter member name: ";
                cin.ignore();
                getline(cin, memberName);

                bool found = false;
                for (int i = 0; i < numMembers; i++) {
                    if (members[i] == memberName) {
                        found = true;

                        string contact;
                        cout << "Enter new contact info: ";
                        getline(cin, contact);

                        contactInfo[i] = contact;
                        system("cls");
                        cout << "\nMember details modified successfully." << endl;
                        break;
                    }
                }
                if (!found) {
                    system("cls");
                    cout << "\nMember not found." << endl;
                }
            }
            break;
        }

        case 9: {
            // Generate overdue notices logic here
                time_t currentTime = time(nullptr);
                system("cls");
                cout << "Overdue Notices:" << endl;
                cout << "----------------" << endl;

                for (int i = 0; i < numBooks; i++) {
                    if (loaned[i] && dueDates[i] < currentTime) {
                        int memberIndex = -1;
                        for (int j = 0; j < numMembers; j++) {
                            if (borrowers[i] == members[j]) {
                                memberIndex = j;
                                break;
                            }
                        }

                        if (memberIndex != -1) {
                            cout << "Book: " << books[i] << endl;
                            cout << "Borrower: " << members[memberIndex] << endl;
                            cout << "Contact Info: " << contactInfo[memberIndex] << endl;
                            cout << "------------------------------" << endl;
                        }
                    }
                }

            break;
        }

        case 10: {
            // Calculate and display most popular materials
            int maxCheckouts = 0;
            string popularMaterial;
            for (int i = 0; i < numBooks; i++) {
                if (numCheckouts[i] > maxCheckouts) {
                    maxCheckouts = numCheckouts[i];
                    popularMaterial = books[i];
                }
            }
            cout << "Most Popular Material: " << popularMaterial << endl;

            // Calculate and display most active members
            int maxCheckoutsByMember = 0;
            string activeMember;
            for (int i = 0; i < numMembers; i++) {
                if (numActiveMembers[i] > maxCheckoutsByMember) {
                    maxCheckoutsByMember = numActiveMembers[i];
                    activeMember = members[i];
                }
            }
            cout << "Most Active Member: " << activeMember << endl;
            break;
        }

        case 11: {
            return 0;
        }

        default:
            system("cls");
            cout << "\nInvalid choice. Please try again." << endl;
            break;
        }
    }
    return 0;
}
