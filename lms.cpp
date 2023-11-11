#include <iostream>
#include <cstdlib>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <iomanip>
#include <cstring>

using namespace std;

const string server = "tcp://localhost:3306";
const string username = "root";
const string password = "keshav2004";
const string database = "library";

sql::Driver* driver;
sql::Connection* con;

struct Book {
    char title[50], author[50], publisher[50];
    int year, quantity;
    float price;
};

struct User {
    string username;
    string password;
};

struct Node {
    Book data;
    Node* left;
    Node* right;
};

class BST {
private:
    Node* root;

    // Helper methods
    Node* insert(Node* node, Book data);
    Node* search(Node* node, char* title);

public:
    BST() : root(nullptr) {}
    void insert(Book data);
    Book* search(char* title);

};

// Implement the BST methods
Node* BST::insert(Node* node, Book data) {
    if (node == nullptr) {
        node = new Node;
        node->data = data;
        node->left = node->right = nullptr;
    }
    else if (strcmp(data.title, node->data.title) < 0) {
        node->left = insert(node->left, data);
    }
    else if (strcmp(data.title, node->data.title) > 0) {
        node->right = insert(node->right, data);
    }
    return node;
}

void BST::insert(Book data) {
    root = insert(root, data);
}

Node* BST::search(Node* node, char* title) {
    if (node == nullptr || strcmp(node->data.title, title) == 0) {
        return node;
    }
    else if (strcmp(title, node->data.title) < 0) {
        return search(node->left, title);
    }
    else {
        return search(node->right, title);
    }
}

Book* BST::search(char* title) {
    Node* node = search(root, title);
    return node ? &node->data : nullptr;
}

// Create a global BST object
BST bst;

void loadBooksIntoBST() {
    // Create a statement to select all books
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM books");

    // For each book in the result set, insert it into the BST
    while (res->next()) {
        Book book;
        strcpy_s(book.title, sizeof(book.title), res->getString("title").c_str());
        strcpy_s(book.author, sizeof(book.author), res->getString("author").c_str());
        strcpy_s(book.publisher, sizeof(book.publisher), res->getString("publisher").c_str());
        book.year = res->getInt("year");
        book.quantity = res->getInt("quantity");
        book.price = static_cast<float>(res->getDouble("price"));
        bst.insert(book);
    }

    delete res;
    delete stmt;
}


void search(char* title) {
    // Search for the book in the BST
    loadBooksIntoBST();
    Book* book = bst.search(title);

    // If the book is found, print its details
    if (book) {
        cout << "Book found!" << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Title" << setw(20) << "Author" << setw(20) << "Publisher" << setw(20) << "Year" << setw(20) << "Quantity" << setw(20) << "Price" << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
        cout << left << setw(20) << book->title;
        cout << setw(20) << book->author;
        cout << setw(20) << book->publisher;
        cout << setw(20) << book->year;
        cout << setw(20) << book->quantity;
        cout << setw(20) << book->price << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
    }
    else {
        cout << "Book not found!" << endl;
    }
}




void initializeDatabase() {
    driver = get_driver_instance();
    con = driver->connect(server, username, password);
    con->setSchema(database);

    // Create the 'books' table if it doesn't exist
    sql::Statement* stmt = con->createStatement();
    stmt->execute("CREATE TABLE IF NOT EXISTS books ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "title VARCHAR(50) NOT NULL,"
        "author VARCHAR(50) NOT NULL,"
        "publisher VARCHAR(50) NOT NULL,"
        "year INT NOT NULL,"
        "quantity INT NOT NULL,"
        "price FLOAT NOT NULL"
        ")");
    delete stmt;
    stmt = con->createStatement();
    stmt->execute("CREATE TABLE IF NOT EXISTS admin ("
        "username VARCHAR(50) NOT NULL,"
        "password VARCHAR(50) NOT NULL"
        ")");
    delete stmt;

    // Create the 'student' table if it doesn't exist
    stmt = con->createStatement();
    stmt->execute("CREATE TABLE IF NOT EXISTS student ("
        "username VARCHAR(50) NOT NULL,"
        "password VARCHAR(50) NOT NULL"
        ")");
    delete stmt;
}
bool login(string username, string password, string userType) {
    sql::PreparedStatement* pstmt = con->prepareStatement(
        "SELECT * FROM " + userType + " WHERE username = ? AND password = ?"
    );
    pstmt->setString(1, username);
    pstmt->setString(2, password);
    sql::ResultSet* res = pstmt->executeQuery();
    bool loggedIn = res->next();
    delete res;
    delete pstmt;
    return loggedIn;
}

void registerUser(User user, string userType) {
    sql::PreparedStatement* pstmt = con->prepareStatement(
        "INSERT INTO " + userType + " (username, password) VALUES (?, ?)"
    );
    pstmt->setString(1, user.username);
    pstmt->setString(2, user.password);
    pstmt->execute();
    delete pstmt;
}

bool isBookExists(char* title) {
    sql::PreparedStatement* pstmt = con->prepareStatement(
        "SELECT * FROM books WHERE title = ?"
    );
    pstmt->setString(1, title);
    sql::ResultSet* res = pstmt->executeQuery();
    bool exists = res->next();
    delete res;
    delete pstmt;
    return exists;
}

void insert(Book book) {
    if (!isBookExists(book.title)) {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "INSERT INTO books (title, author, publisher, year, quantity, price) VALUES (?, ?, ?, ?, ?, ?)"
        );
        pstmt->setString(1, book.title);
        pstmt->setString(2, book.author);
        pstmt->setString(3, book.publisher);
        pstmt->setInt(4, book.year);
        pstmt->setInt(5, book.quantity);
        pstmt->setDouble(6, book.price);
        pstmt->execute();
        cout << "Book inserted!" << endl;
        delete pstmt;
    }
    else {
        cout << "Book already exists!" << endl;
    }
}

void display() {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM books");
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(20) << "Title" << setw(20) << "Author" << setw(20) << "Publisher" << setw(20) << "Year" << setw(20) << "Quantity" << setw(20) << "Price" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;

    while (res->next()) {
        cout << left << setw(20) << res->getString("title");
        cout << setw(20) << res->getString("author");
        cout << setw(20) << res->getString("publisher");
        cout << setw(20) << res->getInt("year");
        cout << setw(20) << res->getInt("quantity");
        cout << setw(20) << res->getDouble("price") << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
    }

    delete res;
    delete stmt;
}



void issue(char* title) {
    sql::PreparedStatement* pstmt = con->prepareStatement(
        "UPDATE books SET quantity = quantity - 1 WHERE title = ? AND quantity > 0"
    );
    pstmt->setString(1, title);
    int rows = pstmt->executeUpdate();
    if (rows == 0) {
        cout << "Book not available!" << endl;
    }
    else {
        cout << "Book issued!" << endl;

        // Query the book again to get the updated quantity
        pstmt = con->prepareStatement(
            "SELECT quantity FROM books WHERE title = ?"
        );
        pstmt->setString(1, title);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            cout << "Remaining quantity: " << res->getInt("quantity") << endl;
        }
        delete res;
    }
    delete pstmt;
}


void returnBook(char* title) {
    if (isBookExists(title)) {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE books SET quantity = quantity + 1 WHERE title = ?"
        );
        pstmt->setString(1, title);
        pstmt->executeUpdate();
        cout << "Book is returned" << endl;
        delete pstmt;
    }
    else {
        cout << "Book not found!" << endl;
    }
}

void deleteBook(char* title) {
    if (isBookExists(title)) {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "DELETE FROM books WHERE title = ?"
        );
        pstmt->setString(1, title);
        pstmt->executeUpdate();
        cout << "Book is deleted from database" << endl;
        delete pstmt;
    }
    else {
        cout << "Book not found!" << endl;
    }
}

void update(Book book) {
    if (isBookExists(book.title)) {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE books SET author = ?, publisher = ?, year = ?, quantity = ?, price = ? WHERE title = ?"
        );
        pstmt->setString(1, book.author);
        pstmt->setString(2, book.publisher);
        pstmt->setInt(3, book.year);
        pstmt->setInt(4, book.quantity);
        pstmt->setDouble(5, book.price);
        pstmt->setString(6, book.title);
        pstmt->executeUpdate();
        cout << "Book is updated" << endl;
        delete pstmt;
    }
    else {
        cout << "Book not found!" << endl;
    }
}


int main() {
    initializeDatabase();

    while (true) {
        cout << "\n\n";
        cout << "*******************************************\n";
        cout << "------------------------------------------\n";
        cout << "\tLIBRARY MANAGEMENT SYSTEM\n";
        cout << "------------------------------------------\n";
        cout << "*******************************************\n";
        cout << "1. Login as Admin" << endl;
        cout << "2. Login as Student" << endl;
        cout << "3. Register as Admin" << endl;
        cout << "4. Register as Student" << endl;
        cout << "5. Exit" << endl;
        cout << "-------------------------" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            User user;
            cout << "Enter username: ";
            cin >> user.username;
            cout << "Enter password: ";
            cin >> user.password;
            if (login(user.username, user.password, "admin")) {
                // Admin menu...
                while (true) {
                    cout << "*************************" << endl;
                    cout << "\tADMIN PORTAL" << endl;
                    cout << "*************************" << endl;
                    cout << "1. Add Book" << endl;
                    cout << "2. Display All Books" << endl;
                    cout << "3. Search Book" << endl;
                    cout << "4. Issue Book" << endl;
                    cout << "5. Return Book" << endl;
                    cout << "6. Delete Book" << endl;
                    cout << "7. Update Book" << endl;
                    cout << "8. Logout" << endl;
                    cout << "*************************" << endl;
                    cout << "Enter your choice: ";
                    int adminChoice;
                    cin >> adminChoice;

                    switch (adminChoice) {
                    case 1: {
                        Book book;
                        cout << "Enter title: ";
                        cin.ignore();
                        cin.getline(book.title, 50);
                        cout << "Enter author: ";
                        cin.getline(book.author, 50);
                        cout << "Enter publisher: ";
                        cin.getline(book.publisher, 50);
                        cout << "Enter year: ";
                        cin >> book.year;
                        cout << "Enter quantity: ";
                        cin >> book.quantity;
                        cout << "Enter price: ";
                        cin >> book.price;
                        insert(book);
                        break;
                    }
                    case 2: {
                        display();
                        break;
                    }
                    case 3: {
                        char title[50];
                        cout << "Enter the title of the book you want to search: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        search(title);
                        break;
                    }
                    case 4: {
                        char title[50];
                        cout << "Enter the title of the book you want to issue: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        issue(title);
                        break;
                    }
                    case 5: {
                        char title[50];
                        cout << "Enter the title of the book you want to return: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        returnBook(title);
                        break;
                    }
                    case 6: {
                        char title[50];
                        cout << "Enter the title of the book you want to delete: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        deleteBook(title);
                        break;
                    }
                    case 7: {
                        Book book;
                        cout << "Enter title: ";
                        cin.ignore();
                        cin.getline(book.title, 50);
                        cout << "Enter author: ";
                        cin.getline(book.author, 50);
                        cout << "Enter publisher: ";
                        cin.getline(book.publisher, 50);
                        cout << "Enter year: ";
                        cin >> book.year;
                        cout << "Enter quantity: ";
                        cin >> book.quantity;
                        cout << "Enter price: ";
                        cin >> book.price;
                        update(book);
                        cout << "Book updated." << endl << endl;
                        break;
                    }
                    case 8: {
                        // Logout...
                        break;
                    }
                    default: {
                        cout << "Invalid choice." << endl;
                    }
                    }if (adminChoice == 8) break;
                }
            }
            else {
                cout << "Invalid username or password." << endl;
            }
            break;
        }
        case 2: {
            User user;
            cout << "Enter username: ";
            cin >> user.username;
            cout << "Enter password: ";
            cin >> user.password;
            if (login(user.username, user.password, "student")) {
                // Student menu...
                while (true) {
                    cout << "*************************" << endl;
                    cout << "\tSTUDENT PORTAL" << endl;
                    cout << "*************************" << endl;
                    cout << "1. Search Book" << endl;
                    cout << "2. Issue Book" << endl;
                    cout << "3. Return Book" << endl;
                    cout << "4. Logout" << endl;
                    cout << "*************************" << endl;
                    cout << "Enter your choice: ";
                    int studentChoice;
                    cin >> studentChoice;

                    switch (studentChoice) {
                    case 1: {
                        char title[50];
                        cout << "Enter the title of the book you want to search: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        search(title);
                        break;
                    }
                    case 2: {
                        char title[50];
                        cout << "Enter the title of the book you want to issue: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        issue(title);
                        break;
                    }
                    case 3: {
                        char title[50];
                        cout << "Enter the title of the book you want to return: ";
                        cin.ignore();
                        cin.getline(title, 50);
                        returnBook(title);
                        break;
                    }
                    case 4: {
                        // Logout...
                        break;
                    }
                    default: {
                        cout << "Invalid choice." << endl;
                    }
                    }if (studentChoice == 4) break;
                }
            }
            else {
                cout << "Invalid username or password." << endl;
            }
            break;
        }
        case 3: {
            User user;
            cout << "Enter username: ";
            cin >> user.username;
            cout << "Enter password: ";
            cin >> user.password;
            registerUser(user, "admin");
            cout << "Admin registered." << endl;
            break;
        }
        case 4: {
            User user;
            cout << "Enter username: ";
            cin >> user.username;
            cout << "Enter password: ";
            cin >> user.password;
            registerUser(user, "student");
            cout << "Student registered." << endl;
            break;
        }
        case 5: {
            exit(0);
        }
        default: {
            cout << "Invalid choice." << endl;
        }
        }
    }

    return 0;
}

