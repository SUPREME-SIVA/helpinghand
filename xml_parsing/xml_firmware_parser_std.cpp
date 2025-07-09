#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <regex>

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
    int installDurationSec = 0;
    bool resetRequired = false;
    bool directFlashOk = false;
    bool uefiFlashable = false;
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
        int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
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
        
        std::cout << "\nProducts:" << std::endl;
        for (const auto& product : products) {
            std::cout << "  - ROM: " << product.rom << std::endl;
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
        }
    }
};

// Simple XML parser class
class SimpleXmlParser {
private:
    std::string xml;
    size_t pos = 0;
    
    // Skip whitespace
    void skipWhitespace() {
        while (pos < xml.size() && std::isspace(xml[pos])) {
            pos++;
        }
    }
    
    // Extract tag name
    std::string extractTagName() {
        size_t start = pos;
        while (pos < xml.size() && !std::isspace(xml[pos]) && xml[pos] != '>' && xml[pos] != '/') {
            pos++;
        }
        return xml.substr(start, pos - start);
    }
    
    // Extract attribute
    std::pair<std::string, std::string> extractAttribute() {
        skipWhitespace();
        
        // Check if we're at the end of the tag
        if (pos >= xml.size() || xml[pos] == '>' || xml[pos] == '/') {
            return {"", ""};
        }
        
        // Extract attribute name
        size_t nameStart = pos;
        while (pos < xml.size() && xml[pos] != '=' && xml[pos] != '>' && !std::isspace(xml[pos])) {
            pos++;
        }
        std::string name = xml.substr(nameStart, pos - nameStart);
        
        skipWhitespace();
        
        // Check for equals sign
        if (pos >= xml.size() || xml[pos] != '=') {
            return {name, ""};
        }
        
        // Skip equals sign
        pos++;
        skipWhitespace();
        
        // Extract attribute value
        if (pos >= xml.size()) {
            return {name, ""};
        }
        
        char quote = xml[pos];
        if (quote != '"' && quote != '\'') {
            return {name, ""};
        }
        
        pos++; // Skip opening quote
        size_t valueStart = pos;
        
        // Find closing quote
        while (pos < xml.size() && xml[pos] != quote) {
            pos++;
        }
        
        std::string value = xml.substr(valueStart, pos - valueStart);
        
        if (pos < xml.size()) {
            pos++; // Skip closing quote
        }
        
        return {name, value};
    }
    
    // Extract text content between tags
    std::string extractTextContent(const std::string& endTag) {
        size_t start = pos;
        size_t endPos = xml.find("<" + endTag, pos);
        
        if (endPos == std::string::npos) {
            // Try with any closing tag
            endPos = xml.find("</", pos);
            if (endPos == std::string::npos) {
                return "";
            }
        }
        
        std::string content = xml.substr(start, endPos - start);
        
        // Check for CDATA
        std::regex cdata_regex("<\\!\\[CDATA\\[(.*?)\\]\\]>");
        std::smatch match;
        if (std::regex_search(content, match, cdata_regex)) {
            content = match[1].str();
        }
        
        // Trim whitespace
        content = std::regex_replace(content, std::regex("^\\s+|\\s+$"), "");
        
        return content;
    }
    
public:
    SimpleXmlParser(const std::string& xmlData) : xml(xmlData) {}
    
    // Parse XML and extract tag content
    std::string getTagContent(const std::string& tagName) {
        // Reset position
        pos = 0;
        
        std::string openTag = "<" + tagName;
        std::string closeTag = "</" + tagName + ">";
        
        size_t tagStart = xml.find(openTag, pos);
        if (tagStart == std::string::npos) {
            return "";
        }
        
        // Move to the end of the opening tag
        pos = xml.find(">", tagStart);
        if (pos == std::string::npos) {
            return "";
        }
        pos++; // Skip '>'
        
        // Find the closing tag
        size_t tagEnd = xml.find(closeTag, pos);
        if (tagEnd == std::string::npos) {
            return "";
        }
        
        return xml.substr(pos, tagEnd - pos);
    }
    
    // Parse XML and extract tag attributes
    std::map<std::string, std::string> getTagAttributes(const std::string& tagName) {
        std::map<std::string, std::string> attributes;
        
        // Reset position
        pos = 0;
        
        std::string openTag = "<" + tagName;
        
        size_t tagStart = xml.find(openTag, pos);
        if (tagStart == std::string::npos) {
            return attributes;
        }
        
        // Move to the start of the tag
        pos = tagStart + openTag.length();
        
        // Extract attributes until we reach the end of the tag
        while (pos < xml.size() && xml[pos] != '>') {
            auto attr = extractAttribute();
            if (!attr.first.empty()) {
                attributes[attr.first] = attr.second;
            }
            skipWhitespace();
        }
        
        return attributes;
    }
    
    // Find all occurrences of a tag and return their contents
    std::vector<std::string> getAllTagContents(const std::string& tagName) {
        std::vector<std::string> results;
        
        // Reset position
        pos = 0;
        
        std::string openTag = "<" + tagName;
        std::string closeTag = "</" + tagName + ">";
        
        while (true) {
            size_t tagStart = xml.find(openTag, pos);
            if (tagStart == std::string::npos) {
                break;
            }
            
            // Move to the end of the opening tag
            size_t contentStart = xml.find(">", tagStart);
            if (contentStart == std::string::npos) {
                break;
            }
            contentStart++; // Skip '>'
            
            // Find the closing tag
            size_t contentEnd = xml.find(closeTag, contentStart);
            if (contentEnd == std::string::npos) {
                break;
            }
            
            results.push_back(xml.substr(contentStart, contentEnd - contentStart));
            
            // Move past this tag for the next iteration
            pos = contentEnd + closeTag.length();
        }
        
        return results;
    }
    
    // Parse translated text nodes
    TranslatedText parseTranslatedText(const std::string& parentTag, const std::string& childTag) {
        TranslatedText result;
        
        std::string parentContent = getTagContent(parentTag);
        if (parentContent.empty()) {
            return result;
        }
        
        SimpleXmlParser childParser(parentContent);
        std::vector<std::string> childTags = childParser.getAllTagContents(childTag);
        
        for (const auto& childContent : childTags) {
            // Create a parser for each child tag to extract its attributes and content
            std::string childXml = "<" + childTag + ">" + childContent + "</" + childTag + ">";
            SimpleXmlParser tagParser(childXml);
            
            auto attrs = tagParser.getTagAttributes(childTag);
            std::string lang = attrs["lang"];
            
            // Clean up CDATA if present
            std::string content = childContent;
            std::regex cdata_regex("<\\!\\[CDATA\\[(.*?)\\]\\]>");
            std::smatch match;
            if (std::regex_search(content, match, cdata_regex)) {
                content = match[1].str();
            }
            
            result.translations[lang] = content;
        }
        
        return result;
    }
};

// Function to read file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
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

// Function to parse firmware package from XML
FirmwarePackage parseFirmwarePackage(const std::string& xml_data) {
    FirmwarePackage package;
    SimpleXmlParser parser(xml_data);
    
    // Parse package version
    auto rootAttrs = parser.getTagAttributes("cpq_package");
    package.version = rootAttrs["version"];
    
    // Parse filename
    package.filename = parser.getTagContent("filename");
    
    // Parse name
    package.name = parser.parseTranslatedText("name", "name_xlate");
    
    // Parse manufacturer name
    package.manufacturerName = parser.parseTranslatedText("manufacturer_name", "manufacturer_name_xlate");
    
    // Parse description
    package.description = parser.parseTranslatedText("description", "description_xlate");
    
    // Parse operating systems
    std::string osContent = parser.getTagContent("operating_systems");
    if (!osContent.empty()) {
        SimpleXmlParser osParser("<operating_systems>" + osContent + "</operating_systems>");
        std::vector<std::string> osList = osParser.getAllTagContents("operating_system");
        
        for (const auto& osXml : osList) {
            SimpleXmlParser singleOsParser("<operating_system>" + osXml + "</operating_system>");
            OperatingSystem os;
            os.name = singleOsParser.getTagContent("operating_system_xlate");
            package.operatingSystems.push_back(os);
        }
    }
    
    // Parse products
    std::string productsContent = parser.getTagContent("products");
    if (!productsContent.empty()) {
        SimpleXmlParser productsParser("<products>" + productsContent + "</products>");
        std::vector<std::string> productList = productsParser.getAllTagContents("product");
        
        for (const auto& productXml : productList) {
            SimpleXmlParser singleProductParser("<product>" + productXml + "</product>");
            auto attrs = singleProductParser.getTagAttributes("product");
            
            Product product;
            product.rom = attrs["rom"];
            package.products.push_back(product);
        }
    }
    
    // Parse version info
    auto versionAttrs = parser.getTagAttributes("version");
    package.version_info.value = versionAttrs["value"];
    package.version_info.revision = versionAttrs["revision"];
    package.version_info.type_of_change = versionAttrs["type_of_change"];
    
    // Parse release date
    auto releaseDateAttrs = parser.getTagAttributes("release_date");
    package.releaseDate.year = std::stoi(releaseDateAttrs["year"]);
    package.releaseDate.month = std::stoi(releaseDateAttrs["month"]);
    package.releaseDate.day = std::stoi(releaseDateAttrs["day"]);
    package.releaseDate.hour = std::stoi(releaseDateAttrs["hour"]);
    package.releaseDate.minute = std::stoi(releaseDateAttrs["minute"]);
    package.releaseDate.second = std::stoi(releaseDateAttrs["second"]);
    
    // Parse payload devices
    std::string payloadContent = parser.getTagContent("payload");
    if (!payloadContent.empty()) {
        SimpleXmlParser payloadParser("<payload>" + payloadContent + "</payload>");
        std::string devicesContent = payloadParser.getTagContent("Devices");
        
        if (!devicesContent.empty()) {
            SimpleXmlParser devicesParser("<Devices>" + devicesContent + "</Devices>");
            std::vector<std::string> deviceList = devicesParser.getAllTagContents("Device");
            
            for (const auto& deviceXml : deviceList) {
                SimpleXmlParser singleDeviceParser("<Device>" + deviceXml + "</Device>");
                
                FirmwareDevice device;
                device.deviceName = singleDeviceParser.getTagContent("DeviceName");
                device.version = singleDeviceParser.getTagContent("Version");
                device.target = singleDeviceParser.getTagContent("Target");
                
                std::string imagesContent = singleDeviceParser.getTagContent("FirmwareImages");
                if (!imagesContent.empty()) {
                    SimpleXmlParser imagesParser("<FirmwareImages>" + imagesContent + "</FirmwareImages>");
                    device.fileName = imagesParser.getTagContent("FileName");
                    
                    std::string durationStr = imagesParser.getTagContent("InstallDurationSec");
                    if (!durationStr.empty()) {
                        device.installDurationSec = std::stoi(durationStr);
                    }
                    
                    device.resetRequired = imagesParser.getTagContent("ResetRequired") == "True";
                    device.directFlashOk = imagesParser.getTagContent("DirectFlashOk") == "True";
                    device.uefiFlashable = imagesParser.getTagContent("UefiFlashable") == "True";
                }
                
                package.devices.push_back(device);
            }
        }
    }
    
    return package;
}

int main() {
    // Save the XML data to a file
    std::string xml_data = R"(<cpq_package version="2.0.1.0">
<filename>cp037722.exe</filename>
<divisions>
<division key="68">
<division_xlate lang="en">ServerOptions</division_xlate>
<division_xlate lang="cn">服务器选项</division_xlate>
<division_xlate lang="ja">サーバ オプション</division_xlate>
<division_xlate lang="ko">서버 옵션</division_xlate>
<division_xlate lang="tw">ServerOptions</division_xlate>
</division>
<division key="6">
<division_xlate lang="en">Server</division_xlate>
<division_xlate lang="cn">服务器</division_xlate>
<division_xlate lang="ja">サーバ</division_xlate>
<division_xlate lang="ko">서버</division_xlate>
<division_xlate lang="tw">伺服器</division_xlate>
</division>
</divisions>
<operating_systems>
<operating_system key="">
<operating_system_xlate>Microsoft Windows Server 2012 R2</operating_system_xlate>
</operating_system>
<operating_system key="">
<operating_system_xlate>Microsoft Windows Server 2016</operating_system_xlate>
</operating_system>
<operating_system key="">
<operating_system_xlate>Microsoft Windows Server 2019</operating_system_xlate>
</operating_system>
</operating_systems>
<products>
<product rom="I41"/>
<product rom="U34"/>
<product rom="U30"/>
<product rom="U32"/>
</products>
<name>
<name_xlate lang="en">Online Flash Component for Windows x64 - Gen10 NVMe Backplane PIC Firmware</name_xlate>
<name_xlate lang="ja">オンラインフラッシュコンポーネントfor Windows x64 - Gen10 NVMeバックプレーンPICファームウェア</name_xlate>
</name>
<manufacturer_name>
<manufacturer_name_xlate lang="en">Hewlett Packard Enterprise</manufacturer_name_xlate>
<manufacturer_name_xlate lang="ja">Hewlett Packard Enterprise</manufacturer_name_xlate>
</manufacturer_name>
<version value="1.20" revision="D" type_of_change="0"/>
<release_date year="2018" month="10" day="27" hour="8" minute="48" second="40"/>
<description>
<description_xlate lang="en">
<![CDATA[ This component updates the firmware on ProLiant Gen10 NVMe storage backplanes. ]]>
</description_xlate>
<description_xlate lang="ja">
<![CDATA[ このコンポーネントは、ProLiant Gen10 NVMe ストレージバックプレーンをアップデートします。 ]]>
</description_xlate>
</description>
<payload>
<Devices>
<Device>
<DeviceName>NVMe Backplane PIC</DeviceName>
<FirmwareImages>
<FileName>ISS_NVMe_BP_PIC_flashV1B20_signed.hex</FileName>
<InstallDurationSec>120</InstallDurationSec>
<ResetRequired>False</ResetRequired>
<DirectFlashOk>False</DirectFlashOk>
<UefiFlashable>False</UefiFlashable>
</FirmwareImages>
<Target>00000000-0000-0000-0000-000049535320</Target>
<Version>1.20</Version>
</Device>
</Devices>
</payload>
</cpq_package>)";

    saveXmlToFile(xml_data, "firmware_data.xml");
    
    // Parse the XML data
    FirmwarePackage package = parseFirmwarePackage(xml_data);
    
    // Print summary of the parsed data
    package.printSummary();
    
    std::cout << "\nTo use this program with your full XML data:" << std::endl;
    std::cout << "1. Replace the XML string in the main function with your complete XML data" << std::endl;
    std::cout << "2. Compile the program with: g++ -std=c++14 xml_firmware_parser_std.cpp -o xml_parser" << std::endl;
    std::cout << "3. Run the program: ./xml_parser" << std::endl;
    
    return 0;
}
