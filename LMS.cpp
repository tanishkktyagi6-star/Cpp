#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

const string FILE_NAME = "books.txt";

class Book {
private:
    int id;
    string title;
    string author;
    bool issued;

public:
    Book() : id(0), issued(false) {}

    Book(int bookId, string bookTitle, string bookAuthor, bool status = false) {
        id = bookId;
        title = bookTitle;
        author = bookAuthor;
        issued = status;
    }

    int getId() const {
        return id;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool isIssued() const {
        return issued;
    }

    void setTitle(const string &newTitle) {
        title = newTitle;
    }

    void setAuthor(const string &newAuthor) {
        author = newAuthor;
    }

    void setIssued(bool status) {
        issued = status;
    }

    void display() const {
        cout << left
             << setw(10) << id
             << setw(30) << title
             << setw(25) << author
             << setw(15) << (issued ? "Issued" : "Available")
             << '\n';
    }

    void saveToFile(ofstream &file) const {
        file << id << '|'
             << title << '|'
             << author << '|'
             << issued << '\n';
    }
};

class LibraryManagementSystem {
private:
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool bookExists(int bookId) {
        ifstream file(FILE_NAME);
        string line;

        while (getline(file, line)) {
            size_t position = line.find('|');

            if (position != string::npos) {
                int storedId = stoi(line.substr(0, position));

                if (storedId == bookId) {
                    return true;
                }
            }
        }

        return false;
    }

    Book parseBook(const string &line) {
        size_t first = line.find('|');
        size_t second = line.find('|', first + 1);
        size_t third = line.find('|', second + 1);

        int id = stoi(line.substr(0, first));
        string title = line.substr(first + 1, second - first - 1);
        string author = line.substr(second + 1, third - second - 1);
        bool issued = stoi(line.substr(third + 1));

        return Book(id, title, author, issued);
    }

public:
    void showMenu() {
        cout << "\n========================================\n";
        cout << "       LIBRARY MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Add Book\n";
        cout << "2. View All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Update Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Exit\n";
        cout << "========================================\n";
    }

    void addBook() {
        int id;
        string title;
        string author;

        cout << "\n========== ADD BOOK ==========\n";
        cout << "Enter book ID: ";

        if (!(cin >> id)) {
            cout << "Invalid book ID.\n";
            clearInput();
            return;
        }

        clearInput();

        if (id <= 0) {
            cout << "Book ID must be greater than zero.\n";
            return;
        }

        if (bookExists(id)) {
            cout << "A book with this ID already exists.\n";
            return;
        }

        cout << "Enter book title: ";
        getline(cin, title);

        cout << "Enter author name: ";
        getline(cin, author);

        if (title.empty() || author.empty()) {
            cout << "Title and author cannot be empty.\n";
            return;
        }

        Book book(id, title, author);

        ofstream file(FILE_NAME, ios::app);

        if (!file) {
            cout << "Unable to open the book file.\n";
            return;
        }

        book.saveToFile(file);
        file.close();

        cout << "Book added successfully.\n";
    }

    void viewBooks() {
        ifstream file(FILE_NAME);
        string line;
        int totalBooks = 0;

        cout << "\n==================== ALL BOOKS ====================\n";

        if (!file) {
            cout << "No books found.\n";
            return;
        }

        cout << left
             << setw(10) << "ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(15) << "Status"
             << '\n';

        cout << string(80, '-') << '\n';

        while (getline(file, line)) {
            if (!line.empty()) {
                Book book = parseBook(line);
                book.display();
                totalBooks++;
            }
        }

        file.close();

        if (totalBooks == 0) {
            cout << "No books found.\n";
        } else {
            cout << string(80, '-') << '\n';
            cout << "Total books: " << totalBooks << '\n';
        }
    }

    void searchBook() {
        int searchId;
        string line;
        bool found = false;

        cout << "\n========== SEARCH BOOK ==========\n";
        cout << "Enter book ID: ";

        if (!(cin >> searchId)) {
            cout << "Invalid book ID.\n";
            clearInput();
            return;
        }

        clearInput();

        ifstream file(FILE_NAME);

        if (!file) {
            cout << "No books found.\n";
            return;
        }

        while (getline(file, line)) {
            if (!line.empty()) {
                Book book = parseBook(line);

                if (book.getId() == searchId) {
                    cout << "\nBook found:\n\n";

                    cout << left
                         << setw(10) << "ID"
                         << setw(30) << "Title"
                         << setw(25) << "Author"
                         << setw(15) << "Status"
                         << '\n';

                    cout << string(80, '-') << '\n';
                    book.display();

                    found = true;
                    break;
                }
            }
        }

        file.close();

        if (!found) {
            cout << "Book with ID " << searchId << " not found.\n";
        }
    }

    void updateBook() {
        int searchId;
        string line;
        string newTitle;
        string newAuthor;
        bool found = false;

        cout << "\n========== UPDATE BOOK ==========\n";
        cout << "Enter book ID to update: ";

        if (!(cin >> searchId)) {
            cout << "Invalid book ID.\n";
            clearInput();
            return;
        }

        clearInput();

        ifstream inputFile(FILE_NAME);
        ofstream tempFile("temp.txt");

        if (!inputFile || !tempFile) {
            cout << "Unable to open the required files.\n";
            return;
        }

        while (getline(inputFile, line)) {
            if (line.empty()) {
                continue;
            }

            Book book = parseBook(line);

            if (book.getId() == searchId) {
                found = true;

                cout << "Current title: " << book.getTitle() << '\n';
                cout << "Enter new title or press Enter to keep it: ";
                getline(cin, newTitle);

                cout << "Current author: " << book.getAuthor() << '\n';
                cout << "Enter new author or press Enter to keep it: ";
                getline(cin, newAuthor);

                if (!newTitle.empty()) {
                    book.setTitle(newTitle);
                }

                if (!newAuthor.empty()) {
                    book.setAuthor(newAuthor);
                }
            }

            book.saveToFile(tempFile);
        }

        inputFile.close();
        tempFile.close();

        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());

        if (found) {
            cout << "Book updated successfully.\n";
        } else {
            cout << "Book with ID " << searchId << " not found.\n";
        }
    }

    void deleteBook() {
        int searchId;
        string line;
        bool found = false;

        cout << "\n========== DELETE BOOK ==========\n";
        cout << "Enter book ID to delete: ";

        if (!(cin >> searchId)) {
            cout << "Invalid book ID.\n";
            clearInput();
            return;
        }

        clearInput();

        ifstream inputFile(FILE_NAME);
        ofstream tempFile("temp.txt");

        if (!inputFile || !tempFile) {
            cout << "Unable to open the required files.\n";
            return;
        }

        while (getline(inputFile, line)) {
            if (line.empty()) {
                continue;
            }

            Book book = parseBook(line);

            if (book.getId() == searchId) {
                found = true;
            } else {
                book.saveToFile(tempFile);
            }
        }

        inputFile.close();
        tempFile.close();

        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());

        if (found) {
            cout << "Book deleted successfully.\n";
        } else {
            cout << "Book with ID " << searchId << " not found.\n";
        }
    }

    void issueBook() {
        changeIssueStatus(true);
    }

    void returnBook() {
        changeIssueStatus(false);
    }

    void changeIssueStatus(bool issueStatus) {
        int searchId;
        string line;
        bool found = false;
        bool operationCompleted = false;

        cout << "\nEnter book ID: ";

        if (!(cin >> searchId)) {
            cout << "Invalid book ID.\n";
            clearInput();
            return;
        }

        clearInput();

        ifstream inputFile(FILE_NAME);
        ofstream tempFile("temp.txt");

        if (!inputFile || !tempFile) {
            cout << "Unable to open the required files.\n";
            return;
        }

        while (getline(inputFile, line)) {
            if (line.empty()) {
                continue;
            }

            Book book = parseBook(line);

            if (book.getId() == searchId) {
                found = true;

                if (issueStatus && book.isIssued()) {
                    cout << "This book is already issued.\n";
                } else if (!issueStatus && !book.isIssued()) {
                    cout << "This book has not been issued.\n";
                } else {
                    book.setIssued(issueStatus);
                    operationCompleted = true;
                }
            }

            book.saveToFile(tempFile);
        }

        inputFile.close();
        tempFile.close();

        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());

        if (!found) {
            cout << "Book with ID " << searchId << " not found.\n";
        } else if (operationCompleted) {
            if (issueStatus) {
                cout << "Book issued successfully.\n";
            } else {
                cout << "Book returned successfully.\n";
            }
        }
    }

    void run() {
        int choice;

        do {
            showMenu();
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input. Enter a number from 1 to 8.\n";
                clearInput();
                continue;
            }

            clearInput();

            switch (choice) {
                case 1:
                    addBook();
                    break;

                case 2:
                    viewBooks();
                    break;

                case 3:
                    searchBook();
                    break;

                case 4:
                    updateBook();
                    break;

                case 5:
                    deleteBook();
                    break;

                case 6:
                    issueBook();
                    break;

                case 7:
                    returnBook();
                    break;

                case 8:
                    cout << "\nThank you for using the Library System.\n";
                    break;

                default:
                    cout << "Invalid choice. Select from 1 to 8.\n";
            }

        } while (choice != 8);
    }
};

int main() {
    LibraryManagementSystem library;
    library.run();

    return 0;
}