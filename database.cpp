#include "database.h"
#include <iostream>
#include <cstring>

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value) {
    Entry *entry = new Entry;
    entry->type = type;
    entry->key = key;
    
    // 값의 복사를 위해 메모리를 할당하고 복사한다.
    if (type == STRING) {
        entry->value = new std::string(*(std::string*)value);
    } else if (type == ARRAY) {
        Array *arr = (Array*)value;
        Array *newArr = new Array;
        newArr->size = arr->size;
        newArr->type = arr->type;
        
        // double 타입의 배열인 경우에는 double* 형식의 메모리를 할당한다.
        // double의 메모리 크기와 void*의 크기가 다르기 때문에.
        if (arr->type == DOUBLE) {
            newArr->items = new double[arr->size];
            // 값 복사
            std::memcpy(newArr->items, arr->items, arr->size * sizeof(double));
        } else if (arr->type == STRING){
            newArr->items = new std::string[arr->size];
            // 값 복사
            std::memcpy(newArr->items, arr->items, arr->size * sizeof(std::string));
        } else {
            // 기존과 동일하게 void* 형식의 메모리를 할당한다.
            newArr->items = new void*[arr->size];
            std::memcpy(newArr->items, arr->items, arr->size * sizeof(void*));
        }
        //복사된 array를 entry의 value에 저장.
        entry->value = newArr;
    } else {
        entry->value = value;
    }
    
    return entry;
}

// 데이터베이스를 초기화한다.
void init(Database &database) {
    database.entries = new Entry*[1];
    database.size = 0;
    database.capacity = 1;
}

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry) {
    // 중복된 키가 있는지 확인한다.
    for (int i = 0; i < database.size; ++i) {
        if (database.entries[i]->key == entry->key) {
            // 중복된 키가 있다면 해당 엔트리를 덮어쓴다.
            delete database.entries[i]; // 기존 엔트리 제거
            database.entries[i] = entry; // 새로운 엔트리로 대체
            return;
        }
    }

    // 중복된 키가 없으면 새로운 엔트리를 추가한다.
    if (database.size == database.capacity) {
        // 데이터베이스의 크기가 용량을 초과하면 용량을 두 배로 늘린다.
        Entry **newEntries = new Entry*[database.capacity * 2];
        std::memcpy(newEntries, database.entries, database.capacity * sizeof(Entry*));
        delete[] database.entries;
        database.entries = newEntries;
        database.capacity *= 2;
    }
    
    database.entries[database.size++] = entry;
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry get(Database &database, std::string &key) {
    for (int i = 0; i < database.size; ++i) {
        if (database.entries[i]->key == key) {
            return *(database.entries[i]);
        }
    }
    // 찾지 못한 경우 빈 Entry 반환
    return Entry{STRING, "", nullptr};
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key) {
    for (int i = 0; i < database.size; ++i) {
        if (database.entries[i]->key == key) {
            // 해당 엔트리를 삭제하고 배열을 재조정한다.
            delete database.entries[i];
            //삭제한 엔트리 뒤에 있는 엔트리들을 한칸씩 앞당긴다.
            for (int j = i; j < database.size - 1; ++j) {
                database.entries[j] = database.entries[j + 1];
            }
            --database.size;
            return;
        }
    }
}

// 데이터베이스를 해제한다.
void destroy(Database &database) {
    // 데이터베이스에 저장된 각 엔트리를 삭제한다.
    for (int i = 0; i < database.size; ++i) {
        delete database.entries[i];
    }
    // 엔트리 배열을 삭제한다.
    delete[] database.entries;
    // 초기화한다.
    database.size = 0;
    database.capacity = 0;
}

void printList(Database &myDB){
// 데이터베이스에 있는 모든 엔트리를 출력한다.
    std::cout << "Database entries:" << std::endl;
    for (int i = 0; i < myDB.size; ++i) {
        Entry *entry = myDB.entries[i];
        std::cout << entry->key << ": ";
        switch (entry->type) {
            case INT:
                std::cout << *static_cast<int*>(entry->value);
                break;
            case DOUBLE:
                std::cout << *static_cast<double*>(entry->value);
                break;
            case STRING:
                std::cout << *static_cast<std::string*>(entry->value);
                break;
            //array가 다차원일 경우를 대비해, printArray로 구현한다.
            case ARRAY:
                printArray((Array*)entry->value);
                break;
        }
        std::cout << std::endl;
    }
}

void setArray(Array& arr) {
    //array를 만드는 함수
    std::cout << "Enter array size: ";
    int size;
    std::cin >> size;
    arr.size = size;

    std::cout << "Enter array type (int/double/string/array): ";
    std::string arrayTypeStr;
    std::cin >> arrayTypeStr;

    if (arrayTypeStr == "int") {
        arr.type = INT;
        int *items = new int[size];
        for (int i = 0; i < size; ++i) {
            std::cout << "Enter integer value for item[" << i << "]: ";
            std::cin >> items[i];
        }
        arr.items = items;
    } else if (arrayTypeStr == "double") {
        arr.type = DOUBLE;
        double *items = new double[size];
        for (int i = 0; i < size; ++i) {
            std::cout << "Enter double value for item[" << i << "]: ";
            std::cin >> items[i];
        }
        arr.items = items;
    } else if (arrayTypeStr == "string") {
        arr.type = STRING;
        std::string *items = new std::string[size];
        std::cin.ignore(); //getline을 위한 개행 문자 무시
        for (int i = 0; i < size; ++i) {
            std::cout << "Enter string value for item[" << i << "]: ";
            std::getline(std::cin, items[i]);
        }
        arr.items = items;
    } else if (arrayTypeStr == "array") {
        arr.type = ARRAY;
        Array **items = new Array*[size];
        for (int i = 0; i < size; ++i) {
            std::cout << "Enter nested array for item[" << i << "]" << std::endl;
            items[i] = new Array;
            setArray(*items[i]); // 재귀적으로 배열 설정
        }
        arr.items = items;
    } else {
        std::cout << "Invalid array type!" << std::endl;
    }
}

void printArray(Array* arr){
    //array를 출력하는 함수.
    if (!arr) {
        std::cout << "NULL";
        return;
    }

    switch (arr->type) {
        case INT: {
            //arr에 있는 item을 int*로 형변환.
            int* items = static_cast<int*>(arr->items);
            std::cout << "[";
            for (int i = 0; i < arr->size; ++i) {
                if (i != 0) std::cout << ", ";
                std::cout << items[i];
            }
            std::cout << "]";
            break;
        }
        case DOUBLE: {
            //arr에 있는 item을 double*로 형변환.
            double* items = static_cast<double*>(arr->items);
            std::cout << "[";
            for (int i = 0; i < arr->size; ++i) {
                if (i != 0) std::cout << ", ";
                std::cout << items[i];
            }
            std::cout << "]";
            break;
        }
        case STRING: {
            //arr에 있는 item을 string*로 형변환.
            std::string* items = static_cast<std::string*>(arr->items);
            std::cout << "[";
            for (int i = 0; i < arr->size; ++i) {
                if (i != 0) std::cout << ", ";
                std::cout << items[i];
            }
            std::cout << "]";
            break;
        }
        case ARRAY: {
            //arr에 있는 item을 Array**로 형변환.
            //arr의 타입이 array라는 것은 다차원 배열이라는 뜻이므로 접근의 용이성을 위해 **를 사용.
            Array** nestedArr = static_cast<Array**>(arr->items);
            std::cout << "[";
            for (int i = 0; i < arr->size; ++i) {
                if (i != 0) std::cout << ", ";
                printArray(nestedArr[i]);
            }
            std::cout << "]";
            break;
        }
        default:
            std::cout << "Invalid array type!" << std::endl;
            break;
    }
}