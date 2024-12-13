
#include <iostream>
#include <fstream>
#include <cstdlib>
/*These are the header files included in the program. iostream is the standard input/output stream library, and fstream is the file stream library.*/

using namespace std;
/*This line allows you to use names from the std namespace without explicitly specifying it.*/
//Function deaclarations
/*These lines declare the functions used in the program. Each function is declared with its return type, name, and parameter list.*/
void write_on_file();
void display_menu();
void add_contact();
void remove_contact();
void modify_contact();
void convert_to_lower(string &tempname);
bool validate_name(const string& name);
bool validate_phone(string number);
void run_menu(int choice_for_menu);
const int MAX_CONTACTS = 100;
int nextID = 1;
/*This is a structure declaration. It defines a structure named user_info with member variables fname (first name), lname (last name), phone_number, gender, and id. An object users_information of this structure is also declared.*/
struct user_info{
    string fname,lname,phone_number;
    char gender;
    int id;
}users_information;


/*This function takes a string tempname as a parameter and converts each character in the string to lowercase using the std::tolower function.*/
void convert_to_lower(string &tempname){
    
    for (char& c : tempname)
        {
            c = std::tolower(c); // Convert each character to lowercase using std::tolower
        }
}

/*This function takes a string name as a parameter and checks if all characters in the string are alphabets using the isalpha function. It returns true if all characters are alphabets, and false otherwise.*/
bool validate_name(const string& name) {
    for (char c : name) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

/*This function takes a character gender as a parameter and checks if it is either 'M', 'm', 'F', or 'f'. It returns true if the gender is valid, and false otherwise.*/
bool validate_gender(char gender) {
    if (gender != 'M' && gender != 'm' && gender != 'f' && gender != 'F') {
        return false;
        
    }
    return true;
}
/*This function takes a string number as a parameter and validates it as a phone number. It checks if all characters in the string are digits using the isdigit function and if the length of the string is 10. It returns true if the number is valid, and false otherwise.*/
bool validate_phone(string number) {
    // Check if the number is empty or contains non-digit characters
    for (char c : number) {
        if (!isdigit(c)) {
            return false;
        }
    }

    // Check if the number has a specific length (e.g., 10 digits)
    if (number.length() != 10) {
        return false;
    }

    
    return true;
}

/*This function is used to add a contact. It prompts the user to enter the first name, last name, gender, and phone number. It performs validation checks using the validate_name, validate_gender, and validate_phone functions. If any input is invalid, an appropriate error message is displayed, and the user is prompted again. Once valid input is received, the write_on_file function is called to write the contact information to a file, and then the display_menu function is called to show the main menu.*/

void add_contact() {
   
   static int id2 = 1;
 
    
    
    bool userchoice1 =true;
    string user_choice3;
    while(userchoice1){
       users_information.id = id2;
        id2 += 1;
        bool error_found = true;
        while (error_found) {
            cout << "Enter first name:" << endl;
            cin >> users_information.fname;
            
            cout << "Enter last name:" << endl;
            cin >> users_information.lname;
            
            cout << "Enter gender (M/F):" << endl;
            cin >> users_information.gender;
            
            cout << "Enter phone number:" << endl;
            cin >> users_information.phone_number;
            
            
            if (!validate_name(users_information.fname) || !validate_name(users_information.lname)) {
                cout << "Invalid name (only alphabets)" << endl;
                continue;
            }
            if (!validate_gender(users_information.gender)) {
                cout << "Invalid gender (M/F)" << endl;
                continue;
            }
            if (!validate_phone(users_information.phone_number)) {
                cout << "Invalid number (10 digits and numbers only)" << endl;
                continue;
            }
            
            error_found = false;  // Set the flag to false to exit the loop
        }
        
        write_on_file();
        cout<<"Do you want to add contact? Press Y/y to continue other key to exit"<<endl;
        
        cin>>user_choice3;
        if(user_choice3=="Y"||user_choice3 =="y"){
            userchoice1=true;
        }
        else{
            userchoice1=false;
            system("clear");
            display_menu();
        }
        
    }
}

/*This function is used to view the contact information stored in the "help.dat" file. It opens the file in binary mode using an `ifstream` object `file`.

If the file fails to open, an error message is displayed, and the `display_menu` function is called to show the main menu.

If the file is successfully opened, the function enters a loop using the `while` statement. Inside the loop, the `read` function is used to read the contact information from the file into the `users_information` structure. The `sizeof(users_information)` is used to specify the number of bytes to read.

The contact information is then displayed on the console, including the ID, full name, gender, and phone number. The loop continues until there is no more data to read from the file.

Finally, the file is closed using the `close` function, and the `display_menu` function is called to show the main menu.

These are the main functions and sections of code in the provided program. Each function serves a specific purpose, such as adding a contact, writing to a file, and viewing contact information. The program utilizes file handling, input validation, and struct data types to manage and store contact information.*/
void write_on_file(){
    ofstream file;
    file.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary | ios::app);
    if(file.is_open()){
        cout<<"File opened successfully"<<endl;
        file.write((char *) &users_information,sizeof(users_information));
    }
    else if(!file.is_open()){
        cout<<"File not opened successfully"<<endl;
    }

   
    file.close();
}

void view_contact(){
    
        ifstream file;
        file.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary);
        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            display_menu();
            return;
        }

        while (file.read((char*)&users_information, sizeof(users_information))) {
            cout << "______________________________" << endl;
            cout << "ID:                " << users_information.id << endl;
            cout << "______________________________" << endl;
            cout << "Full name:         " << users_information.fname << " " << users_information.lname << endl;
            cout << "Gender:            " << users_information.gender << endl;
            cout << "Phone number:      " << users_information.phone_number << endl;
        }
        
        file.close();
        display_menu();
    }
   
/*
 10. The `search_contact` function prompts the user to enter a full name to search for. It reads the input using `getline` and converts both the input and the stored contact names to lowercase using the `convert_to_lower` function. It then compares the names and if a match is found, it displays the contact information. If no match is found, it displays a "Contact not found" message.*/
/*void search_contact() {
    ifstream file3;
    string tempname;
    string user_choice3;
    bool userchoice =true;
    while(userchoice){
        cout << "Enter your full name: " << endl;
        cin.ignore();
        getline(cin, tempname);
        
        file3.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary);
        
        bool contactFound = false;  // Add a flag to track if the contact is found
        
        while (file3.read((char*)&users_information, sizeof(users_information))) {
            string tempfullname = users_information.fname +" "+ users_information.lname;
            convert_to_lower(tempfullname);
            convert_to_lower(tempname);
            
            if (tempname == tempfullname) {
                cout << "Contact found" << endl;
                cout << "______________________________" << endl;
                cout << "ID:                " << users_information.id << endl;
                cout << "______________________________" << endl;
                cout << "Full name:         " << users_information.fname << " " << users_information.lname << endl;
                cout << "Gender:            " << users_information.gender << endl;
                cout << "Phone number:      " << users_information.phone_number << endl;
                contactFound = true;  // Set the flag to true since the contact is found
                break;
            }
        }
        
        file3.close();
        
        if (!contactFound) {
            cout << "Contact not found" << endl;
        }
        cout<<"Do you want to search again ? Press Y/y to continue other key to exit"<<endl;
        
        cin>>user_choice3;
        if(user_choice3=="Y"||user_choice3 =="y"){
            userchoice=true;
        }
        else{
            userchoice=false;
            display_menu();
        }
        
    }
}*/
void search_contact() {
    ifstream file3;
    string tempname;
    string user_choice3;
    bool userchoice = true;
      // Maximum number of contacts to store
    user_info matchingContacts[MAX_CONTACTS];  // Array to store matching contacts
    int numMatches = 0;  // Count of matching contacts
    
    while (userchoice) {
        cout << "Enter your full name: " << endl;
        cin.ignore();
        getline(cin, tempname);
        
        file3.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary);
        
        while (file3.read((char*)&users_information, sizeof(users_information))) {
            string tempfullname = users_information.fname + " " + users_information.lname;
            convert_to_lower(tempfullname);
            convert_to_lower(tempname);
            
            if (tempname == tempfullname) {
                matchingContacts[numMatches] = users_information;  // Store the matching contact in the array
                numMatches++;
                if (numMatches >= MAX_CONTACTS) {
                    break;  // Break out of the loop if the maximum number of contacts is reached
                }
            }
        }
        
        file3.close();
        
        if (numMatches > 0) {
            cout << "Contacts found:" << endl;
            for (int i = 0; i < numMatches; i++) {
                cout << "______________________________" << endl;
                cout << "ID:                " << matchingContacts[i].id << endl;
                cout << "______________________________" << endl;
                cout << "Full name:         " << matchingContacts[i].fname << " " << matchingContacts[i].lname << endl;
                cout << "Gender:            " << matchingContacts[i].gender << endl;
                cout << "Phone number:      " << matchingContacts[i].phone_number << endl;
            }
        } else {
            cout << "No contacts found" << endl;
        }
        
        cout << "Do you want to search again? Press Y/y to continue or any other key to exit" << endl;
        cin >> user_choice3;
        
        if (user_choice3 == "Y" || user_choice3 == "y") {
            userchoice = true;
            numMatches = 0;  // Reset the count of matching contacts
        } else {
            userchoice = false;
            display_menu();
        }
    }
}
/*This function is used to modify a contact in a phonebook. It prompts the user to enter an ID and stores it in the variable tempid.*/
void modify_contact() {
    bool error_found = true;
    int tempid;
    bool contactfound = false;
    cout << "Enter ID: ";
    cin >> tempid;
/*It opens two file streams: file6 for reading the original contact data and tempfile2 for writing the modified contact data*/
    ifstream file6;
    ofstream tempfile2;
    file6.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary | ios::in);
    tempfile2.open("/Users/zeilphone/Desktop/Lab groupwork2/Lab_groupwork 2/tp.dat", ios::binary | ios::out);
/*If either of the file streams fails to open, it displays an error message and returns from the function.*/
    if (!file6 || !tempfile2) {
        cout << "Error opening file." << endl;
        return;
    }
/*It reads the contact information from file6 and checks if the entered tempid matches the ID of any contact. If a match is found, it displays the contact information and sets the contactfound flag to true. The loop breaks after finding the contact.*/
    while (file6.read(reinterpret_cast<char*>(&users_information), sizeof(users_information))) {
        if (tempid == users_information.id) {
            cout << "Contact found" << endl;
            cout << "______________________________" << endl;
            cout << "ID:                " << users_information.id << endl;
            cout << "______________________________" << endl;
            cout << "Full name:         " << users_information.fname << " " << users_information.lname << endl;
            cout << "Gender:            " << users_information.gender << endl;
            cout << "Phone number:      " << users_information.phone_number << endl;

            contactfound = true;  // Set the flag to true since the contact is found
            break;
        }
    }

    file6.close();
/*This loop allows the user to edit the contact information. It displays a menu of editing options and based on the user's choice, it prompts for the corresponding information to be edited (first name, last name, gender, phone number). If the user chooses option 5, it calls the display_menu() function. The loop continues until the user chooses to exit (option 5).*/
    while(contactfound && error_found) {
        int choice;
        cout << "Enter what you want to edit" << endl;
        cout << "1. Edit first name" << endl;
        cout << "2. Edit last name" << endl;
        cout << "3. Edit gender" << endl;
        cout << "4. Edit phone number" << endl;
        cout << "5. Exit" << endl;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "Enter first name: ";
                cin >> users_information.fname;
                break;
            }
            case 2: {
                cout << "Enter last name: ";
                cin >> users_information.lname;
                break;
            }
            case 3: {
                cout << "Enter gender: ";
                cin >> users_information.gender;
                break;
            }
            case 4: {
                cout << "Enter phone number: ";
                cin >> users_information.phone_number;
                break;
            }
            case 5: {
               system("clear");
                display_menu();
                break;
            }
        }
        
        /*After the user has finished editing the contact information, it performs validation checks on the edited data. If any validation fails, it displays an error message and continues the loop. Otherwise, it sets the error_found flag to false, indicating that no errors were found and the loop should be exited.*/
        if (!validate_name(users_information.fname) || !validate_name(users_information.lname)) {
            cout << "Invalid name (only alphabets)" << endl;
            continue;
        }
        if (!validate_gender(users_information.gender)) {
            cout << "Invalid gender (M/F)" << endl;
            continue;
        }
        if (!validate_phone(users_information.phone_number)) {
            cout << "Invalid number (10 digits and numbers only)" << endl;
            continue;
        }

        error_found = false;  // Set the flag to false to exit the loop
    }

    

        tempfile2.write(reinterpret_cast<char*>(&users_information), sizeof(users_information));
        file6.close();
        tempfile2.close();
/*It removes the original contact file and renames the temporary file to replace it with the modified contact data.*/
        remove("/Users/zeilphone/Desktop/help/help.dat");
        rename("/Users/zeilphone/Desktop/Lab groupwork2/Lab_groupwork 2/tp.dat", "/Users/zeilphone/Desktop/help/help.dat");

        cout << "Contact modified successfully." << endl;
    /*If contactfound is false, it means the contact with the entered ID was not found. It closes the file streams, removes the temporary file, and displays a message indicating that the contact was not found. Finally, it calls the display_menu() function.*/
     if(!contactfound){
        file6.close();
        tempfile2.close();

        remove("/Users/zeilphone/Desktop/Lab groupwork2/Lab_groupwork 2/tp.dat");
        cout << "Contact not found." << endl;
    }
    system("clear");

    display_menu();
}

void remove_contact() {
    // Function to remove a contact from the phonebook
       // It reads the contact data from a file, removes the specified contact, and updates the file
       // It also displays messages to indicate if the contact was removed successfully or not
    ifstream file;
    ofstream tempFile;
    file.open("/Users/zeilphone/Desktop/help/help.dat", ios::binary);
    tempFile.open("/Users/zeilphone/Desktop/Lab groupwork2/tempfile.dat", ios::binary);
    if (!file || !tempFile) {
        cout << "Error opening file." << endl;
        return;
    }

    int tempid;
    cout << "Enter the ID of the contact to remove: ";
    cin >> tempid;

   
    bool contactFound = false;

    while (file.read((char*)&users_information, sizeof(users_information))) {
        if (users_information.id != tempid) {
            tempFile.write((char*)&users_information, sizeof(users_information));
        } else {
            contactFound = true;
        }
    }

    file.close();
    tempFile.close();

    if (contactFound) {
        remove("/Users/zeilphone/Desktop/help/help.dat");
        rename("/Users/zeilphone/Desktop/Lab groupwork2/tempfile.dat", "/Users/zeilphone/Desktop/help/help.dat");
        cout << "Contact removed successfully." << endl;
    } else {
        remove("/Users/zeilphone/Desktop/Lab groupwork2/tempfile.dat");
        cout << "Contact not found." << endl;
    }
    system("clear");

    display_menu();
}





void run_menu(int choice_for_menu){
    // Function to run the menu based on the user's choice
        // It calls corresponding functions based on the user's choice to perform different operations
    switch(choice_for_menu){
            
        case 1:
            system("clear");

            view_contact();
            break;
        case 2:
            system("clear");

            search_contact();
            break;
        case 3:
            system("clear");

            add_contact();
            break;
        case 4:
            system("clear");

            modify_contact();
            break;
        case 5:
            system("clear");

            remove_contact();
            break;
        
         
    }
    
}

void display_menu(){
    // Function to display the menu options to the user
      // It prompts the user to choose an option and calls the `run_menu` function with the chosen option
    int choice_for_menu;
    cout<<"Please choose: "<<endl;
    cout<<"  1) View all contacts"<<endl;
    cout<<"  2) Search for a contact"<<endl;
    cout<<"  3) Add a new contact"<<endl;
    cout<<"  4) Modify existing account"<<endl;
    cout<<"  5) Remove existing account"<<endl;
    cin>>choice_for_menu;
    while(choice_for_menu < 1 || choice_for_menu>5){
        cout<<"Invalid input.Try again    ";
        cin>>choice_for_menu;
    }
    run_menu(choice_for_menu);
}

int main(){
    // The main function
        // It displays a welcome message and calls the `display_menu` function to start the phonebook systemw3
    cout<<"                       WELCOME TO OUR PHONEBOOK SYSTEM"<<endl;
    cout<<"****************************************************************************"<<endl;
    cout<<"****************************************************************************"<<endl;
    display_menu();
    
    return 0;
}


