#include <iostream>
#include "database.h"

int main() {
    Database myDB;
    init(myDB);
    
    std::string command;
    std::string key;
    Type type;
    std::string value;
    
    while (true) {
        std::cout << "Enter command (list/add/get/del/exit): ";
        std::cin >> command;
        
        //list: 데이터베이스에 있는 모든 엔트리를 출력한다.
        if (command == "list") {
            printList(myDB);
            std::cout << std::endl;
        //add: key와 value를 입력받고, 엔트리로 만들어 데이터베이스에 추가한다.
        } else if (command == "add") {
            // Key를 입력 받는다.
            std::cout << "Enter key: ";
            std::cin >> key;

            // Type을 입력 받는다.
            std::cout << "Enter type (int/double/string/array): ";
            std::string typeStr;
            std::cin >> typeStr;
            Type type;
            if (typeStr == "int") {
                type = INT;
            } else if (typeStr == "double") {
                type = DOUBLE;
            } else if (typeStr == "string") {
                type = STRING;
            } else if (typeStr == "array") {
                type = ARRAY;
            } else {
                std::cout << "Invalid type!" << std::endl;
                continue;
            }

            // 타입에 따라 값을 입력 받아 엔트리를 생성하고 데이터베이스에 추가한다.
            if (type == INT) {
                int intValue;
                std::cout << "Enter value: ";
                std::cin >> intValue;
                add(myDB, create(type, key, &intValue));
            } else if (type == DOUBLE) {
                double doubleValue;
                std::cout << "Enter value: ";
                std::cin >> doubleValue;
                add(myDB, create(type, key, &doubleValue));
            } else if (type == STRING) {
                std::string stringValue;
                std::cin.ignore(); // 개행 문자를 제거하기 위함.
                std::cout << "Enter value: ";
                std::getline(std::cin, stringValue);
                add(myDB, create(type, key, &stringValue));
            } else if (type == ARRAY) {
                Array* arr = new Array;
                arr->type=type;
                setArray(*arr);
                add(myDB, create(type, key, arr));
            }
        //get: key를 입력받고, 그 key에 해당하는 value를 출력한다.
        } else if (command == "get") {
          std::cout << "Enter key: ";
          std::cin >> key;
          Entry entry = get(myDB, key);
          if (entry.key != "") {
              std::cout << entry.key << ": ";
              switch (entry.type) {
                  case INT:
                      std::cout << *static_cast<int*>(entry.value);
                      break;
                  case DOUBLE:
                      std::cout << *static_cast<double*>(entry.value);
                      break;
                  case STRING:
                      std::cout << *static_cast<std::string*>(entry.value);
                      break;
                  case ARRAY:
                      printArray((Array*)entry.value);
                      break;
              }
              std::cout << std::endl;
          } else {
              std::cout << "Entry not found!" << std::endl;
          }
        //del: key에 해당하는 엔트리를 찾고 그 엔트리를 제거한다.
        } else if (command == "del") {
            std::cout << "Enter key: ";
            std::cin >> key;
            remove(myDB, key);
        //exit: 데이터베이스를 없애고, 프로그램을 종료한다.
        } else if (command == "exit") {
            destroy(myDB);
            break;
        } else {
            // 잘못된 명령어일 경우 에러 메시지를 출력한다.
            std::cout << "Error: Invalid command!" << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}