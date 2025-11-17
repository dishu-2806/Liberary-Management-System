#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
using namespace std;

// 1️⃣ Base Class: Book
class Book {
protected:
    int id;
    string title;
    string author;
    bool isIssued;

public:
    static int totalBooks;

    Book(int i, const string& t, const string& a)
        : id(i), title(t), author(a), isIssued(false) {
        ++totalBooks;
    }

    virtual ~Book() {
        --totalBooks;
    }

    int getId() const { return id; }
    string getTitle() const { return title; }

    virtual void display() const {
        cout << left << setw(6) << id
             << setw(25) << title
             << setw(20) << author
             << setw(10) << (isIssued ? "Issued" : "Available") << endl;
    }

    virtual double getFineRate() const = 0;

    void issueBook() {
        if (isIssued)
            throw runtime_error("Book already issued!");
        isIssued = true;
    }

    void returnBook() {
        if (!isIssued)
            throw runtime_error("Book was not issued!");
        isIssued = false;
    }
};
int Book::totalBooks = 0;

// 2️⃣ Derived Classes (Inheritance + Virtual Functions)
class Novel : public Book {
public:
    Novel(int i, const string& t, const string& a) : Book(i, t, a) {}
    double getFineRate() const override { return 2.0; } 
};

class Science : public Book {
public:
    Science(int i, const string& t, const string& a) : Book(i, t, a) {}
    double getFineRate() const override { return 3.5; }
};

class History : public Book {
public:
    History(int i, const string& t, const string& a) : Book(i, t, a) {}
    double getFineRate() const override { return 1.5; }
};

// 3️⃣ Class: Library (acts like Inventory)

class Library {
    vector<Book*> books;

public:
    Library() = default;

    ~Library() {
        for (Book* b : books) delete b;
        books.clear();
    }

    void addBook(Book* b) {
        if (!b) throw invalid_argument("Null book");
        if (search(b->getId()) != nullptr)
            throw runtime_error("Book ID already exists!");
        books.push_back(b);
    }

    // Function Overloading: Search by ID or Title
    Book* search(int id) const {
        for (Book* b : books)
            if (b->getId() == id)
                return b;
        return nullptr;
    }

    Book* search(const string& title) const {
        for (Book* b : books)
            if (b->getTitle() == title)
                return b;
        return nullptr;
    }

    bool removeBook(int id) {
        auto it = remove_if(books.begin(), books.end(),
                            [id](Book* b) { return b->getId() == id; });
        if (it != books.end()) {
            delete *it;
            books.erase(it, books.end());
            return true;
        }
        return false;
    }

    void showAll() const {
        cout << "\n=================== Library Books ===================\n";
        cout << left << setw(6) << "ID" << setw(25) << "Title"
             << setw(20) << "Author" << setw(10) << "Status" << endl;
        cout << "-----------------------------------------------------\n";
        for (Book* b : books) b->display();
        cout << "-----------------------------------------------------\n";
        cout << "Total Books (Static Count): " << Book::totalBooks << "\n";
    }

    vector<Book*>& getBooksRef() { return books; }
};

// 4️⃣ Operator Overloading: Fine Calculation
class Fine {
    double amount;
public:
    Fine(double a = 0.0) : amount(a) {}

    Fine operator+(const Fine& f) const {
        return Fine(amount + f.amount);
    }

    void display() const {
        cout << "Total Fine Amount: Rs " << fixed << setprecision(2) << amount << "\n";
    }
};


// 5️⃣ Templates: Generic Report Saver
template <typename T>
class ReportGenerator {
public:
    void generateReport(const T& obj, const string& filename) {
        ofstream fout(filename, ios::app);
        if (!fout) throw runtime_error("Cannot open file: " + filename);
        fout << obj << "\n";
        fout.close();
    }
};


// 6️⃣ File Handling Helper: Save Issued Books

ostream& operator<<(ostream& os, const Book& b) {
    os << left << setw(6) << b.getId()
       << setw(25) << b.getTitle()
       << setw(20) << "Issued Book Saved";
    return os;
}

// 7️⃣ Main Function (Menu-driven)

int main() {
    Library lib;
    ReportGenerator<Book> reporter;

    try {
        
        lib.addBook(new Novel(101, "Pride_and_Prejudice", "Jane Austen"));
        lib.addBook(new Science(201, "Physics_Fundamentals", "H.C. Verma"));
        lib.addBook(new History(301, "World_History", "K. Roberts"));
    } catch (const exception& e) {
        cerr << "Initialization error: " << e.what() << "\n";
    }

    int choice;
    do {
        cout << "\n=========== LIBRARY MANAGEMENT SYSTEM ===========\n";
        cout << "1. Display All Books\n";
        cout << "2. Add Book\n";
        cout << "3. Remove Book\n";
        cout << "4. Search Book by ID\n";
        cout << "5. Search Book by Title\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Save Issued Book Report\n";
        cout << "9. Fine Calculation (Operator Overload)\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            if (choice == 1) {
                lib.showAll();
            } 
            else if (choice == 2) {
                int id, type;
                string title, author;
                cout << "Enter Book ID: "; cin >> id;
                cout << "Enter Title: "; cin >> title;
                cout << "Enter Author: "; cin >> author;
                cout << "Type (1.Novel 2.Science 3.History): "; cin >> type;
                if (type == 1)
                    lib.addBook(new Novel(id, title, author));
                else if (type == 2)
                    lib.addBook(new Science(id, title, author));
                else if (type == 3)
                    lib.addBook(new History(id, title, author));
                else
                    cout << "Invalid Type!\n";
            } 
            else if (choice == 3) {
                int id; cout << "Enter Book ID to remove: "; cin >> id;
                if (lib.removeBook(id)) cout << "Book removed.\n";
                else cout << "Book not found.\n";
            } 
            else if (choice == 4) {
                int id; cout << "Enter Book ID to search: "; cin >> id;
                Book* b = lib.search(id);
                if (b) b->display(); else cout << "Not found.\n";
            } 
            else if (choice == 5) {
                string title; cout << "Enter Title to search: "; cin >> title;
                Book* b = lib.search(title);
                if (b) b->display(); else cout << "Not found.\n";
            } 
            else if (choice == 6) {
                int id; cout << "Enter Book ID to issue: "; cin >> id;
                Book* b = lib.search(id);
                if (!b) cout << "Book not found.\n";
                else {
                    b->issueBook();
                    cout << "Book issued successfully!\n";
                }
            } 
            else if (choice == 7) {
                int id; cout << "Enter Book ID to return: "; cin >> id;
                Book* b = lib.search(id);
                if (!b) cout << "Book not found.\n";
                else {
                    b->returnBook();
                    cout << "Book returned successfully!\n";
                }
            } 
            else if (choice == 8) {
                int id; cout << "Enter Book ID to save report: "; cin >> id;
                Book* b = lib.search(id);
                if (b) {
                    reporter.generateReport(*b, "issued_books.txt");
                    cout << "Book details saved to file.\n";
                } else cout << "Book not found.\n";
            } 
            else if (choice == 9) {
                double f1, f2;
                cout << "Enter fine1 and fine2 amounts: ";
                cin >> f1 >> f2;
                Fine total = Fine(f1) + Fine(f2);
                total.display();
            } 
            else if (choice == 0) {
                cout << "Exiting system...\n";
            } 
            else {
                cout << "Invalid choice!\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;

}
