#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <cstdlib> // For system()

using namespace std;

// Custom Stack Implementation
template <typename T>
class StackNode {
public:
    T data; // Data stored in the stack node
    StackNode* next; // Pointer to the next node

    StackNode(T value) : data(value), next(nullptr) {} // Constructor
};

template <typename T>
class Stack {
private:
    StackNode<T>* top; // Pointer to the top of the stack

public:
    Stack() : top(nullptr) {} // Constructor to initialize an empty stack

    void push(T value); // Push a value onto the stack
    T pop(); // Pop a value from the stack
    bool isEmpty(); // Check if the stack is empty
};

template <typename T>
void Stack<T>::push(T value) {
    StackNode<T>* temp = new StackNode<T>(value); // Create a new stack node
    temp->next = top; // Link new node to the previous top
    top = temp; // Update top to the new node
}

template <typename T>
T Stack<T>::pop() {
    if (top == nullptr) {
        cout << "Stack underflow" << endl; // Error message for underflow
        return T(); // Return default constructed T
    }
    StackNode<T>* p = top; // Temporarily store the top node
    T x = top->data; // Get the data from the top node
    top = top->next; // Move top to the next node
    delete p; // Delete the old top node
    return x; // Return the popped value
}

template <typename T>
bool Stack<T>::isEmpty() {
    return top == nullptr; // Return true if the stack is empty
}

// Custom Queue Implementation
template <typename T>
class QueueNode {
public:
    T data; // Data stored in the queue node
    QueueNode* next; // Pointer to the next node

    QueueNode(T value) : data(value), next(nullptr) {} // Constructor
};

template <typename T>
class Queue {
private:
    QueueNode<T>* front; // Pointer to the front of the queue
    QueueNode<T>* rear; // Pointer to the rear of the queue

public:
    Queue() : front(nullptr), rear(nullptr) {} // Constructor to initialize an empty queue

    void enqueue(T value); // Add a value to the queue
    T dequeue(); // Remove a value from the queue
    bool isEmpty(); // Check if the queue is empty
};

template <typename T>
void Queue<T>::enqueue(T value) {
    QueueNode<T>* temp = new QueueNode<T>(value); // Create a new queue node
    if (rear) {
        rear->next = temp; // Link the new node at the end of the queue
    }
    rear = temp; // Update the rear to the new node
    if (!front) {
        front = temp; // If the queue was empty, set front to the new node
    }
}

template <typename T>
T Queue<T>::dequeue() {
    if (front == nullptr) {
        cout << "Queue is empty" << endl; // Error message if the queue is empty
        return T(); // Return default constructed T
    }
    QueueNode<T>* p = front; // Temporarily store the front node
    T x = front->data; // Get the data from the front node
    front = front->next; // Move front to the next node
    delete p; // Delete the old front node
    if (front == nullptr) {
        rear = nullptr; // Reset rear if the queue becomes empty
    }
    return x; // Return the dequeued value
}

template <typename T>
bool Queue<T>::isEmpty() {
    return front == nullptr; // Return true if the queue is empty
}

// Node for the Binary Search Tree (BST)
class BudgetEntryNode {
public:
    int id; // Unique identifier for the budget entry
    string date; // Date of the entry
    float amount; // Amount of the entry
    string type; // Type of entry: Income or Expense
    string category; // Category of the entry
    string description; // Description of the entry
    BudgetEntryNode* left; // Pointer to the left child
    BudgetEntryNode* right; // Pointer to the right child

    // Constructor to initialize a budget entry node
    BudgetEntryNode(int id, string date, float amount, string type, string category, string description)
        : id(id), date(date), amount(amount), type(type), category(category), description(description), left(nullptr), right(nullptr) {}

    // Method to display the entry
    void displayEntry() const {
        cout << left << setw(5) << id << setw(15) << date << setw(10) << amount
             << setw(10) << type << setw(15) << category << setw(30) << description << endl;
    }
};

// Enum for Operation Type
enum OperationType { ADD, DELETE, UPDATE };

// Struct for handling undo operations
struct Operation {
    OperationType type; // Type of operation
    BudgetEntryNode* entry; // The budget entry affected
    BudgetEntryNode* oldEntry; // For updates, keep a copy of the old entry
};

// BudgetTracker class using a Binary Search Tree (BST)
class BudgetTracker {
private:
    BudgetEntryNode* root; // Root of the BST
    int entryCount; // Count of entries
    vector<string> expenseCategories; // List of expense categories
    vector<string> incomeCategories; // List of income categories
    Stack<Operation> undoStack; // Stack for undo operations
    Queue<BudgetEntryNode*> recentEntries; // Queue for recent entries

public:
    BudgetTracker() : root(nullptr), entryCount(0) {
        loadEntriesFromFile(); // Load existing entries from file

        // Predefined Expense Categories
        expenseCategories = {
            "Rent", "Utilities", "Groceries", "Transportation", "Entertainment"
        };

        // Predefined Income Categories
        incomeCategories = {
            "Salary", "Freelance", "Investments"
        };
    }

    // Function to clear the input stream
    void clearInputStream() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Function to get a valid amount input
    float getValidAmount() {
        float amount;
        while (true) {
            cin >> amount;
            if (cin.fail() || amount < 0) {
                cout << "Invalid amount. Please enter a valid non-negative number: ";
                clearInputStream();
            } else {
                return amount;
            }
        }
    }

    // Function to get a valid date input
    string getValidDate() {
        string date;
        while (true) {
            cout << "Enter date (YYYY-MM-DD): ";
            cin >> date;
            if (date.length() == 10 && date[4] == '-' && date[7] == '-') {
                return date; // Valid date format
            }
            cout << "Invalid date format. Please enter again in YYYY-MM-DD format.\n";
        }
    }

    // Function to get a valid type input (Income or Expense)
    string getValidType() {
        string type;
        while (true) {
            cout << "Enter type (Income/Expense): ";
            cin >> type;
            if (type == "Income" || type == "Expense") {
                return type; // Valid type
            } else {
                cout << "Invalid type. Please enter 'Income' or 'Expense'.\n";
            }
        }
    }

    // Function to select a category based on entry type
    string selectCategory(const string& type) {
        if (type == "Income") {
            cout << "Select a category for Income:\n";
            for (size_t i = 0; i < incomeCategories.size(); ++i) {
                cout << i + 1 << ". " << incomeCategories[i] << endl; // Display income categories
            }
        } else {
            cout << "Select a category for Expense:\n";
            for (size_t i = 0; i < expenseCategories.size(); ++i) {
                cout << i + 1 << ". " << expenseCategories[i] << endl; // Display expense categories
            }
        }

        int choice;
        while (true) {
            cout << "Enter choice: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > (int)(type == "Income" ? incomeCategories.size() : expenseCategories.size())) {
                cout << "Invalid choice. Please try again.\n";
                clearInputStream();
            } else {
                return type == "Income" ? incomeCategories[choice - 1] : expenseCategories[choice - 1];
            }
        }
    }

    // Function to add a new budget entry
    void addEntry() {
        int id = ++entryCount; // Increment entry count for ID
        string date = getValidDate(); // Get a valid date
        float amount = getValidAmount(); // Get a valid amount
        string type = getValidType(); // Get a valid type
        string category = selectCategory(type); // Select category based on type
        cout << "Enter description (optional): ";
        cin.ignore(); // Clear the newline
        string description;
        getline(cin, description); // Get description input

        // Create new budget entry node
        BudgetEntryNode* newEntry = new BudgetEntryNode(id, date, amount, type, category, description);
        root = insertEntry(root, newEntry); // Insert entry into the BST

        // Add to recent entries queue
        recentEntries.enqueue(newEntry);

        // Push the operation onto the undo stack
        Operation op = { ADD, newEntry, nullptr };
        undoStack.push(op);

        cout << "Budget entry added successfully!\n";
        clearConsole(); // Clear console after operation
    }

    // Function to delete a budget entry by ID
    void deleteEntry() {
        int id;
        cout << "Enter the ID of the entry to delete: ";
        cin >> id;

        BudgetEntryNode* entryToDelete = findEntryById(root, id); // Find entry by ID
        if (entryToDelete) {
            root = deleteEntry(root, id); // Delete entry from the BST

            // Push the operation onto the undo stack
            Operation op = { DELETE, nullptr, entryToDelete };
            undoStack.push(op);

            cout << "Budget entry deleted successfully!\n";
        } else {
            cout << "Entry not found!\n";
        }
        clearConsole(); // Clear console after operation
    }

    // Function to update an existing budget entry
    void updateEntry() {
        int id;
        cout << "Enter the ID of the entry to update: ";
        cin >> id;

        BudgetEntryNode* entryToUpdate = findEntryById(root, id); // Find entry by ID
        if (entryToUpdate) {
            cout << "Updating entry...\n";
            BudgetEntryNode* oldEntry = new BudgetEntryNode(*entryToUpdate); // Keep old entry for undo

            // Get new details for the entry
            string date = getValidDate();
            float amount = getValidAmount();
            string type = getValidType();
            string category = selectCategory(type);
            cout << "Enter new description (optional): ";
            cin.ignore();
            string description;
            getline(cin, description);

            // Update the entry details
            entryToUpdate->date = date;
            entryToUpdate->amount = amount;
            entryToUpdate->type = type;
            entryToUpdate->category = category;
            entryToUpdate->description = description;

            // Push the operation onto the undo stack
            Operation op = { UPDATE, entryToUpdate, oldEntry };
            undoStack.push(op);

            cout << "Budget entry updated successfully!\n";
        } else {
            cout << "Entry not found!\n";
        }
        clearConsole(); // Clear console after operation
    }

    // Function to display all budget entries
    void displayEntries() {
        cout << left << setw(5) << "ID" << setw(15) << "Date" << setw(10) << "Amount"
             << setw(10) << "Type" << setw(15) << "Category" << setw(30) << "Description" << endl;
        displayEntries(root); // Display entries recursively
        clearConsole(); // Clear console after operation
    }

    // Function to undo the last operation
    void undoLastOperation() {
        if (undoStack.isEmpty()) {
            cout << "No operations to undo.\n";
            return;
        }

        Operation lastOp = undoStack.pop(); // Pop the last operation
        switch (lastOp.type) {
            case ADD:
                root = deleteEntry(root, lastOp.entry->id); // Undo Add
                cout << "Last operation undone: Added entry " << lastOp.entry->id << endl;
                break;
            case DELETE:
                root = insertEntry(root, lastOp.oldEntry); // Undo Delete
                cout << "Last operation undone: Deleted entry " << lastOp.oldEntry->id << endl;
                break;
            case UPDATE:
                *lastOp.entry = *lastOp.oldEntry; // Restore old entry for Update
                cout << "Last operation undone: Updated entry " << lastOp.entry->id << endl;
                break;
        }
        clearConsole(); // Clear console after operation
    }

    // Function to load existing entries from a file
    void loadEntriesFromFile() {
        ifstream inFile("budget_entries.txt");
        if (!inFile) {
            cout << "Unable to open file. Starting with an empty budget tracker." << endl;
            return; // File doesn't exist
        }

        // Read entries from file
        while (!inFile.eof()) {
            int id;
            string date;
            float amount;
            string type, category, description;

            inFile >> id >> date >> amount >> type >> category;
            inFile.ignore(); // Ignore the newline character
            getline(inFile, description); // Read description line

            // Create new entry and insert into the BST
            BudgetEntryNode* newEntry = new BudgetEntryNode(id, date, amount, type, category, description);
            root = insertEntry(root, newEntry);
            entryCount = max(entryCount, id); // Keep track of the highest ID
        }
        inFile.close(); // Close the file
    }

    // Function to save entries to a file
    void saveEntriesToFile() {
        ofstream outFile("budget_entries.txt");
        if (!outFile) {
            cout << "Unable to open file for writing." << endl;
            return;
        }

        saveEntriesToFile(root, outFile); // Save entries recursively
        outFile.close(); // Close the file
    }

    // Function to clear the console screen
    void clearConsole() {
        system("CLS"); // For Windows users
        // system("clear"); // Uncomment for Unix-based systems
    }

    // Function for login authentication
    bool login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Simple authentication (you can expand this with a real database or file)
        if (username == "user" && password == "pass") {
            return true; // Successful login
        } else {
            cout << "Invalid username or password. Please try again.\n";
            return false; // Failed login
        }
    }

    // Main function to run the Budget Tracker application
   // Main function to run the Budget Tracker application
void run() {
    while (!login()) {
        cout << "Retrying...\n"; // Loop until successful login
    }

    int choice;
    do {
        clearConsole(); // Clear the console before displaying the menu
        cout << "Budget Tracker Menu:\n";
        cout << "1. Add Entry\n";
        cout << "2. Delete Entry\n";
        cout << "3. Update Entry\n";
        cout << "4. Display Entries\n";
        cout << "5. Display Sorted Entries by Date\n";
        cout << "6. Undo Last Operation\n";
        cout << "7. Save and Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;

        // Check for invalid input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input
            choice = -1; // Set to invalid choice
        } else {
            clearInputStream(); // Clear any leftover input
        }

        switch (choice) {
            case 1:
                addEntry();
                break;
            case 2:
                deleteEntry();
                break;
            case 3:
                updateEntry();
                break;
            case 4:
                displayEntries();
                break;
            case 5:
                displaySortedEntries();
                break;
            case 6:
                undoLastOperation();
                break;
            case 7:
                saveEntriesToFile(); // Save entries before exiting
                cout << "Exiting the application...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        // Prompt before returning to the menu
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); // Wait for user input

    } while (choice != 7); // Continue until the user chooses to exit
};



private:
    // Recursive function to insert a new entry into the BST
    BudgetEntryNode* insertEntry(BudgetEntryNode* node, BudgetEntryNode* newEntry) {
        if (node == nullptr) {
            return newEntry; // Insert new node here
        }
        if (newEntry->id < node->id) {
            node->left = insertEntry(node->left, newEntry); // Insert in left subtree
        } else {
            node->right = insertEntry(node->right, newEntry); // Insert in right subtree
        }
        return node; // Return the unchanged node pointer
    }

    // Function to find an entry by ID
    BudgetEntryNode* findEntryById(BudgetEntryNode* node, int id) {
        if (node == nullptr || node->id == id) {
            return node; // Found entry or reached leaf
        }
        if (id < node->id) {
            return findEntryById(node->left, id); // Search in left subtree
        } else {
            return findEntryById(node->right, id); // Search in right subtree
        }
    }

    // Recursive function to delete an entry from the BST
    BudgetEntryNode* deleteEntry(BudgetEntryNode* node, int id) {
        if (node == nullptr) {
            return nullptr; // Entry not found
        }
        if (id < node->id) {
            node->left = deleteEntry(node->left, id); // Search in left subtree
        } else if (id > node->id) {
            node->right = deleteEntry(node->right, id); // Search in right subtree
        } else {
            // Entry found
            if (node->left == nullptr) {
                BudgetEntryNode* temp = node->right; // Only right child
                delete node; // Free memory
                return temp; // Return new root
            } else if (node->right == nullptr) {
                BudgetEntryNode* temp = node->left; // Only left child
                delete node; // Free memory
                return temp; // Return new root
            }

            // Node with two children: Get the inorder successor
            BudgetEntryNode* temp = node->right;
            while (temp && temp->left != nullptr) {
                temp = temp->left; // Find the smallest in the right subtree
            }
            node->id = temp->id; // Copy the inorder successor's ID
            node->right = deleteEntry(node->right, temp->id); // Delete the inorder successor
        }
        return node; // Return the unchanged node pointer
    }

    // Recursive function to display all entries in the BST
    void displayEntries(BudgetEntryNode* node) const {
        if (node != nullptr) {
            displayEntries(node->left); // Inorder traversal
            node->displayEntry(); // Display current entry
            displayEntries(node->right); // Continue inorder traversal
        }
    }

    // Recursive function to save entries to a file
    void saveEntriesToFile(BudgetEntryNode* node, ofstream& outFile) const {
        if (node != nullptr) {
            saveEntriesToFile(node->left, outFile); // Inorder traversal
            outFile << node->id << " " << node->date << " " << node->amount << " "
                    << node->type << " " << node->category << " " << node->description << endl; // Save entry
            saveEntriesToFile(node->right, outFile); // Continue inorder traversal
        }
    }
    // Function to merge two sorted linked lists
BudgetEntryNode* merge(BudgetEntryNode* left, BudgetEntryNode* right) {
    // If one of the lists is empty, return the other list
    if (!left) return right;
    if (!right) return left;

    // Compare the dates of the two nodes and build the merged list
    if (left->date <= right->date) {
        // Recursively merge the right part
        left->right = merge(left->right, right);
        left->right->left = left; // Maintain the left link
        return left; // Return the merged list starting with 'left'
    } else {
        // Recursively merge the left part
        right->right = merge(left, right->right);
        right->right->left = right; // Maintain the left link
        return right; // Return the merged list starting with 'right'
    }
}

// Function to split the linked list into two halves
void split(BudgetEntryNode* source, BudgetEntryNode** front, BudgetEntryNode** back) {
    BudgetEntryNode* slow = source; // Fast and slow pointer approach
    BudgetEntryNode* fast = source->right;

    // Move 'fast' two nodes and 'slow' one node
    while (fast) {
        fast = fast->right;
        if (fast) {
            slow = slow->right;
            fast = fast->right;
        }
    }

    // Split the linked list into two halves
    *front = source; // The first half starts from 'source'
    *back = slow->right; // The second half starts from 'slow->right'
    slow->right = nullptr; // Break the link to form two separate lists
}

// Merge Sort function
BudgetEntryNode* mergeSort(BudgetEntryNode* head) {
    // Base case: If the list is empty or has one node
    if (!head || !head->right) return head;

    BudgetEntryNode* left;
    BudgetEntryNode* right;

    // Split the list into two halves
    split(head, &left, &right);

    // Recursively sort the two halves
    left = mergeSort(left);
    right = mergeSort(right);

    // Merge the sorted halves and return the sorted list
    return merge(left, right);
}

// Function to display sorted entries by date
void displaySortedEntries() {
    BudgetEntryNode* sortedEntries = mergeSort(root); // Sort entries by date
    // Print the header for the display
    cout << left << setw(5) << "ID" << setw(15) << "Date" << setw(10) << "Amount"
         << setw(10) << "Type" << setw(15) << "Category" << setw(30) << "Description" << endl;
    
    // Display the sorted entries
    displayEntries(sortedEntries);
    clearConsole(); // Clear console after operation
}

};

// Main function
int main() {
    BudgetTracker tracker; // Create a BudgetTracker instance
    tracker.run(); // Start the application
    return 0; // Exit the program
}
