#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

// Abstract Document
class Document {
public:
    virtual void open() = 0;
    virtual void save() = 0;
    virtual std::string getType() const = 0;
    virtual ~Document() = default;
};

// Concrete Documents
class PDFDocument : public Document {
    std::string filename;
public:
    explicit PDFDocument(std::string name) : filename(std::move(name)) {}

    void open() override {
        std::cout << "Opening PDF document: " << filename << std::endl;
        std::cout << "Initializing PDF reader...\n";
        std::cout << "Rendering PDF content\n";
    }

    void save() override {
        std::cout << "Saving PDF document: " << filename << std::endl;
        std::cout << "Compressing PDF...\n";
        std::cout << "PDF saved successfully\n";
    }

    std::string getType() const override {
        return "PDF";
    }
};

class WordDocument : public Document {
    std::string filename;
public:
    explicit WordDocument(std::string name) : filename(std::move(name)) {}

    void open() override {
        std::cout << "Opening Word document: " << filename << std::endl;
        std::cout << "Loading Word processor...\n";
        std::cout << "Displaying document content\n";
    }

    void save() override {
        std::cout << "Saving Word document: " << filename << std::endl;
        std::cout << "Updating document format...\n";
        std::cout << "Word document saved successfully\n";
    }

    std::string getType() const override {
        return "Word";
    }
};

class SpreadsheetDocument : public Document {
    std::string filename;
public:
    explicit SpreadsheetDocument(std::string name) : filename(std::move(name)) {}

    void open() override {
        std::cout << "Opening Spreadsheet: " << filename << std::endl;
        std::cout << "Loading spreadsheet engine...\n";
        std::cout << "Calculating formulas\n";
    }

    void save() override {
        std::cout << "Saving Spreadsheet: " << filename << std::endl;
        std::cout << "Validating cell data...\n";
        std::cout << "Spreadsheet saved successfully\n";
    }

    std::string getType() const override {
        return "Spreadsheet";
    }
};

// Document Factory
class DocumentFactory {
public:
    static std::unique_ptr<Document> createDocument(const std::string& type, const std::string& filename) {
        if (type == "pdf") {
            return std::make_unique<PDFDocument>(filename);
        } else if (type == "word") {
            return std::make_unique<WordDocument>(filename);
        } else if (type == "spreadsheet") {
            return std::make_unique<SpreadsheetDocument>(filename);
        }
        throw std::runtime_error("Unknown document type: " + type);
    }
};

int main() {
    try {
        // Create different types of documents
        std::vector<std::unique_ptr<Document>> documents;
        
        documents.push_back(DocumentFactory::createDocument("pdf", "report.pdf"));
        documents.push_back(DocumentFactory::createDocument("word", "letter.docx"));
        documents.push_back(DocumentFactory::createDocument("spreadsheet", "budget.xlsx"));

        // Use the documents
        for (const auto& doc : documents) {
            std::cout << "\nWorking with " << doc->getType() << " document:\n";
            doc->open();
            doc->save();
        }

        // Try to create an invalid document type
        try {
            auto invalidDoc = DocumentFactory::createDocument("invalid", "test.xyz");
        } catch (const std::runtime_error& e) {
            std::cout << "\nExpected error: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
