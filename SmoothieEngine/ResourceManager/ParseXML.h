#pragma once
#include <vector>
#include <string>
#include <fstream>

//using ElementVector = std::vector<Element>;
struct Element {
    std::string name;
    std::string textContent;
    std::vector<Element> children;
};

class ParseXML
{
    Element Tokenizer(std::ifstream& data);
    Element elements;
public:
    ParseXML(const std::string& file);
    ParseXML() {
        elements.name = "";
        elements.textContent = "";
    }
    //Goes throut root of xml file, trying to find element with elementName.
    //If element is found, it returns. If not, empty element is returned and error is printed.
    Element getElement(const std::string& elementName);

};
