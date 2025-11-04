#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_KEY_LEN = 64;
const int BLOCK_SIZE = 4096;
const int MAX_ENTRIES_PER_BLOCK = 50;
const char* DATA_FILE = "database.dat";
const char* INDEX_FILE = "index.dat";

struct Entry {
    char key[MAX_KEY_LEN + 1];
    int value;
    
    Entry() : value(0) {
        memset(key, 0, sizeof(key));
    }
    
    Entry(const string& k, int v) : value(v) {
        memset(key, 0, sizeof(key));
        strncpy(key, k.c_str(), MAX_KEY_LEN);
    }
    
    bool operator<(const Entry& other) const {
        int cmp = strcmp(key, other.key);
        if (cmp != 0) return cmp < 0;
        return value < other.value;
    }
    
    bool operator==(const Entry& other) const {
        return strcmp(key, other.key) == 0 && value == other.value;
    }
};

class FileDatabase {
private:
    vector<Entry> cache;
    bool dirty;
    
    void loadFromFile() {
        cache.clear();
        ifstream file(DATA_FILE, ios::binary);
        if (!file.is_open()) {
            return;
        }
        
        Entry entry;
        while (file.read(reinterpret_cast<char*>(&entry), sizeof(Entry))) {
            cache.push_back(entry);
        }
        file.close();
        dirty = false;
    }
    
    void saveToFile() {
        if (!dirty) return;
        
        ofstream file(DATA_FILE, ios::binary | ios::trunc);
        for (const auto& entry : cache) {
            file.write(reinterpret_cast<const char*>(&entry), sizeof(Entry));
        }
        file.close();
        dirty = false;
    }
    
public:
    FileDatabase() : dirty(false) {
        loadFromFile();
    }
    
    ~FileDatabase() {
        saveToFile();
    }
    
    void insert(const string& key, int value) {
        Entry newEntry(key, value);
        
        // Check if entry already exists
        for (const auto& entry : cache) {
            if (entry == newEntry) {
                return; // Duplicate, don't insert
            }
        }
        
        cache.push_back(newEntry);
        dirty = true;
    }
    
    void remove(const string& key, int value) {
        Entry toRemove(key, value);

        auto it = std::find(cache.begin(), cache.end(), toRemove);
        if (it != cache.end()) {
            cache.erase(it);
            dirty = true;
        }
    }
    
    void find(const string& key) {
        vector<int> values;
        
        for (const auto& entry : cache) {
            if (strcmp(entry.key, key.c_str()) == 0) {
                values.push_back(entry.value);
            }
        }
        
        if (values.empty()) {
            cout << "null" << endl;
        } else {
            sort(values.begin(), values.end());
            for (size_t i = 0; i < values.size(); i++) {
                if (i > 0) cout << " ";
                cout << values[i];
            }
            cout << endl;
        }
    }
    
    void flush() {
        saveToFile();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    FileDatabase db;
    
    for (int i = 0; i < n; i++) {
        string cmd;
        cin >> cmd;
        
        if (cmd == "insert") {
            string key;
            int value;
            cin >> key >> value;
            db.insert(key, value);
        } else if (cmd == "delete") {
            string key;
            int value;
            cin >> key >> value;
            db.remove(key, value);
        } else if (cmd == "find") {
            string key;
            cin >> key;
            db.find(key);
        }
    }
    
    return 0;
}

