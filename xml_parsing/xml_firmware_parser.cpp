#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <pugixml.hpp>

// Structure to hold translated text in different languages
struct TranslatedText {
    std::map<std::string, std::string> translations;
    
    std::string get(const std::string& lang = "en") const {
        auto it = translations.find(lang);
        return (it != translations.end()) ? it->second : "";
    }
};

// Structure to hold operating system information
struct OperatingSystem {
    std::string key;
    std::string name;
};

// Structure to hold division information
struct Division {
    std::string key;
    TranslatedText name;
};

// Structure to hold product information
struct Product {
    std::string rom;
};

// Structure to hold version information
struct Version {
    std::string value;
    std::string revision;
    std::string type_of_change;
};

// Structure to hold revision information
struct Revision {
    Version version;
    TranslatedText enhancements;
    TranslatedText fixes;
};

// Structure to hold firmware device information
struct FirmwareDevice {
    std::string deviceName;
    std::string fileName;
    std::string target;
    std::string version;
    int installDurationSec;
    bool resetRequired;
    bool directFlashOk;
    bool uefiFlashable;
};

// Main structure to hold all package information
struct FirmwarePackage {
    std::string version;
    std::string filename;
    std::vector<Division> divisions;
    std::vector<OperatingSystem> operatingSystems;
    std::vector<Product> products;
    TranslatedText category;
    std::string id;
    TranslatedText name;
    TranslatedText altName;
    Version version_info;
    TranslatedText manufacturerName;
    TranslatedText languages;
    struct {
        int year, month, day, hour, minute, second;
    } releaseDate;
    TranslatedText description;
    TranslatedText prerequisiteNotes;
    TranslatedText installationNotes;
    std::vector<Revision> revisionHistory;
    std::vector<FirmwareDevice> devices;
    
    // Print a summary of the package
    void printSummary() const {
        std::cout << "=== Firmware Package Summary ===" << std::endl;
        std::cout << "Version: " << version << std::endl;
        std::cout << "Filename: " << filename << std::endl;
        std::cout << "Name: " << name.get("en") << std::endl;
        std::cout << "Manufacturer: " << manufacturerName.get("en") << std::endl;
        std::cout << "Release Date: " << releaseDate.year << "-" 
                  << releaseDate.month << "-" << releaseDate.day << std::endl;
        std::cout << "Description: " << description.get("en") << std::endl;
        
        std::cout << "\nSupported Operating Systems:" << std::endl;
        for (const auto& os : operatingSystems) {
            std::cout << "  - " << os.name << std::endl;
        }
        
        std::cout << "\nDevices:" << std::endl;
        for (const auto& device : devices) {
            std::cout << "  - " << device.deviceName << " (v" << device.version << ")" << std::endl;
            std::cout << "    File: " << device.fileName << std::endl;
            std::cout << "    Install Duration: " << device.installDurationSec << " seconds" << std::endl;
            std::cout << "    Reset Required: " << (device.resetRequired ? "Yes" : "No") << std::endl;
        }
        
        std::cout << "\nRevision History:" << std::endl;
        for (const auto& rev : revisionHistory) {
            std::cout << "  - Version " << rev.version.value;
            if (!rev.version.revision.empty()) {
                std::cout << "(" << rev.version.revision << ")";
            }
            std::cout << std::endl;
            
            if (!rev.enhancements.get().empty()) {
                std::cout << "    Enhancements: " << rev.enhancements.get() << std::endl;
            }
            
            if (!rev.fixes.get().empty()) {
                std::cout << "    Fixes: " << rev.fixes.get() << std::endl;
            }
        }
    }
};

// Function to parse translated text nodes
TranslatedText parseTranslatedText(pugi::xml_node parent_node, const std::string& child_name) {
    TranslatedText result;
    
    for (pugi::xml_node node : parent_node.children(child_name.c_str())) {
        std::string lang = node.attribute("lang").as_string();
        std::string text = node.text().get();
        // Clean up CDATA content
        if (text.find("<![CDATA[") != std::string::npos) {
            size_t start = text.find("<![CDATA[") + 9;
            size_t end = text.find("]]>");
            if (end != std::string::npos) {
                text = text.substr(start, end - start);
            }
        }
        result.translations[lang] = text;
    }
    
    return result;
}

// Function to parse divisions
std::vector<Division> parseDivisions(pugi::xml_node divisions_node) {
    std::vector<Division> result;
    
    for (pugi::xml_node div_node : divisions_node.children("division")) {
        Division div;
        div.key = div_node.attribute("key").as_string();
        div.name = parseTranslatedText(div_node, "division_xlate");
        result.push_back(div);
    }
    
    return result;
}

// Function to parse operating systems
std::vector<OperatingSystem> parseOperatingSystems(pugi::xml_node os_node) {
    std::vector<OperatingSystem> result;
    
    for (pugi::xml_node node : os_node.children("operating_system")) {
        OperatingSystem os;
        os.key = node.attribute("key").as_string();
        os.name = node.child("operating_system_xlate").text().get();
        result.push_back(os);
    }
    
    return result;
}

// Function to parse products
std::vector<Product> parseProducts(pugi::xml_node products_node) {
    std::vector<Product> result;
    
    for (pugi::xml_node node : products_node.children("product")) {
        Product product;
        product.rom = node.attribute("rom").as_string();
        result.push_back(product);
    }
    
    return result;
}

// Function to parse version
Version parseVersion(pugi::xml_node version_node) {
    Version version;
    version.value = version_node.attribute("value").as_string();
    version.revision = version_node.attribute("revision").as_string();
    version.type_of_change = version_node.attribute("type_of_change").as_string();
    return version;
}

// Function to parse revision history
std::vector<Revision> parseRevisionHistory(pugi::xml_node history_node) {
    std::vector<Revision> result;
    
    for (pugi::xml_node rev_node : history_node.children("revision")) {
        Revision revision;
        revision.version = parseVersion(rev_node.child("version"));
        revision.enhancements = parseTranslatedText(rev_node, "revision_enhancements_xlate");
        revision.fixes = parseTranslatedText(rev_node, "revision_fixes_xlate");
        result.push_back(revision);
    }
    
    return result;
}

// Function to parse firmware devices
std::vector<FirmwareDevice> parseDevices(pugi::xml_node devices_node) {
    std::vector<FirmwareDevice> result;
    
    for (pugi::xml_node device_node : devices_node.children("Device")) {
        FirmwareDevice device;
        device.deviceName = device_node.child_value("DeviceName");
        device.version = device_node.child_value("Version");
        device.target = device_node.child_value("Target");
        
        pugi::xml_node images = device_node.child("FirmwareImages");
        if (images) {
            device.fileName = images.child_value("FileName");
            device.installDurationSec = images.child("InstallDurationSec").text().as_int();
            device.resetRequired = std::string(images.child_value("ResetRequired")) == "True";
            device.directFlashOk = std::string(images.child_value("DirectFlashOk")) == "True";
            device.uefiFlashable = std::string(images.child_value("UefiFlashable")) == "True";
        }
        
        result.push_back(device);
    }
    
    return result;
}

// Main function to parse the XML data
FirmwarePackage parseXmlData(const std::string& xml_data) {
    FirmwarePackage package;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());
    
    if (!result) {
        std::cerr << "XML parsing error: " << result.description() << std::endl;
        return package;
    }
    
    pugi::xml_node root = doc.child("cpq_package");
    package.version = root.attribute("version").as_string();
    package.filename = root.child_value("filename");
    
    // Parse divisions
    pugi::xml_node divisions_node = root.child("divisions");
    if (divisions_node) {
        package.divisions = parseDivisions(divisions_node);
    }
    
    // Parse operating systems
    pugi::xml_node os_node = root.child("operating_systems");
    if (os_node) {
        package.operatingSystems = parseOperatingSystems(os_node);
    }
    
    // Parse products
    pugi::xml_node products_node = root.child("products");
    if (products_node) {
        package.products = parseProducts(products_node);
    }
    
    // Parse category
    pugi::xml_node category_node = root.child("category");
    if (category_node) {
        package.category = parseTranslatedText(category_node, "category_xlate");
    }
    
    // Parse ID
    package.id = root.child_value("id");
    
    // Parse name and alt_name
    package.name = parseTranslatedText(root.child("name"), "name_xlate");
    package.altName = parseTranslatedText(root.child("alt_name"), "alt_name_xlate");
    
    // Parse version info
    pugi::xml_node version_node = root.child("version");
    if (version_node) {
        package.version_info = parseVersion(version_node);
    }
    
    // Parse manufacturer name
    package.manufacturerName = parseTranslatedText(root.child("manufacturer_name"), "manufacturer_name_xlate");
    
    // Parse languages
    package.languages = parseTranslatedText(root.child("languages"), "languages_xlate");
    
    // Parse release date
    pugi::xml_node date_node = root.child("release_date");
    if (date_node) {
        package.releaseDate.year = date_node.attribute("year").as_int();
        package.releaseDate.month = date_node.attribute("month").as_int();
        package.releaseDate.day = date_node.attribute("day").as_int();
        package.releaseDate.hour = date_node.attribute("hour").as_int();
        package.releaseDate.minute = date_node.attribute("minute").as_int();
        package.releaseDate.second = date_node.attribute("second").as_int();
    }
    
    // Parse description
    package.description = parseTranslatedText(root.child("description"), "description_xlate");
    
    // Parse prerequisite notes
    package.prerequisiteNotes = parseTranslatedText(root.child("prerequisite_notes"), "prerequisite_notes_xlate");
    
    // Parse installation notes
    package.installationNotes = parseTranslatedText(root.child("installation_notes"), "installation_notes_xlate");
    
    // Parse revision history
    pugi::xml_node history_node = root.child("revision_history");
    if (history_node) {
        package.revisionHistory = parseRevisionHistory(history_node);
    }
    
    // Parse devices
    pugi::xml_node payload_node = root.child("payload");
    if (payload_node) {
        pugi::xml_node devices_node = payload_node.child("Devices");
        if (devices_node) {
            package.devices = parseDevices(devices_node);
        }
    }
    
    return package;
}

// Function to save XML data to a file
bool saveXmlToFile(const std::string& xml_data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    
    file << xml_data;
    file.close();
    return true;
}

// Main function
int main() {
    // XML data string (would be loaded from a file in a real application)
    std::string xml_data = R"(
<cpq_package version="2.0.1.0">
<filename>cp037722.exe</filename>
<divisions>
<division key="68">
<division_xlate lang="en">ServerOptions</division_xlate>
<division_xlate lang="cn">服务器选项</division_xlate>
<division_xlate lang="ja">サーバ オプション</division_xlate>
<division_xlate lang="ko">서버 옵션</division_xlate>
<division_xlate lang="tw">ServerOptions</division_xlate>
</division>
<!-- More divisions... -->
</divisions>
<!-- More XML data... -->
</cpq_package>
    )";
    
    // Save XML data to a file (in a real application, you'd load this from an external source)
    saveXmlToFile(xml_data, "firmware_data.xml");
    
    // Parse the XML data
    FirmwarePackage package = parseXmlData(xml_data);
    
    // Print summary of the parsed data
    package.printSummary();
    
    std::cout << "\nTo use this program with your full XML data:" << std::endl;
    std::cout << "1. Save your XML data to a file named 'firmware_data.xml'" << std::endl;
    std::cout << "2. Modify the main function to load from this file instead of using the hardcoded string" << std::endl;
    std::cout << "3. Recompile and run the program" << std::endl;
    
    return 0;
}
