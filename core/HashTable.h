#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
using namespace std;

// ============================
//  HASH TABLE - O(1) LOOKUP
//  Dùng để: Tìm kiếm cực nhanh theo mã (key)
// ============================

template <typename T>
class HashTable
{
private:
    // Node trong danh sách liên kết (xử lý collision)
    struct Node
    {
        string key; // Mã (VD: "SB001", "KH001")
        T *value;   // Con trỏ tới đối tượng
        Node *next; // Con trỏ tới node tiếp theo

        Node(const string &k, T *v) : key(k), value(v), next(nullptr) {}
    };

    vector<Node *> table; // Bảng băm
    int size;             // Kích thước bảng
    int count;            // Số phần tử hiện có

    // Hàm băm: Chuyển string thành số nguyên
    int hash(const string &key) const
    {
        unsigned long h = 0;
        for (char c : key)
        {
            h = h * 31 + c; // 31 là số nguyên tố tốt cho hash
        }
        return h % size;
    }

public:
    // Constructor
    HashTable(int s = 100) : size(s), count(0), table(s, nullptr) {}

    // Thêm/Cập nhật phần tử - O(1) trung bình
    void insert(const string &key, T *value)
    {
        int idx = hash(key);

        // Kiểm tra xem key đã tồn tại chưa
        Node *curr = table[idx];
        while (curr)
        {
            if (curr->key == key)
            {
                curr->value = value; // Cập nhật nếu đã có
                return;
            }
            curr = curr->next;
        }

        // Thêm mới vào đầu danh sách
        Node *newNode = new Node(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        count++;
    }

    // Tìm kiếm - O(1) trung bình
    T *search(const string &key) const
    {
        int idx = hash(key);
        Node *curr = table[idx];

        while (curr)
        {
            if (curr->key == key)
                return curr->value;
            curr = curr->next;
        }
        return nullptr; // Không tìm thấy
    }

    // Xóa phần tử - O(1) trung bình
    bool remove(const string &key)
    {
        int idx = hash(key);
        Node *curr = table[idx];
        Node *prev = nullptr;

        while (curr)
        {
            if (curr->key == key)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    table[idx] = curr->next;

                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false; // Không tìm thấy
    }

    // Kiểm tra key có tồn tại không - O(1)
    bool contains(const string &key) const
    {
        return search(key) != nullptr;
    }

    // Lấy số lượng phần tử
    int getCount() const
    {
        return count;
    }

    // Xóa toàn bộ bảng
    void clear()
    {
        for (int i = 0; i < size; i++)
        {
            Node *curr = table[i];
            while (curr)
            {
                Node *temp = curr;
                curr = curr->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    // Lấy danh sách tất cả các key
    vector<string> getAllKeys() const
    {
        vector<string> keys;
        for (int i = 0; i < size; i++)
        {
            Node *curr = table[i];
            while (curr)
            {
                keys.push_back(curr->key);
                curr = curr->next;
            }
        }
        return keys;
    }

    // Destructor - Giải phóng bộ nhớ
    ~HashTable()
    {
        clear();
    }

    // Copy constructor (deep copy)
    HashTable(const HashTable &other) : size(other.size), count(0), table(other.size, nullptr)
    {
        for (int i = 0; i < size; i++)
        {
            Node *curr = other.table[i];
            while (curr)
            {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    // Copy assignment
    HashTable &operator=(const HashTable &other)
    {
        if (this != &other)
        {
            clear();
            size = other.size;
            table.resize(size, nullptr);

            for (int i = 0; i < size; i++)
            {
                Node *curr = other.table[i];
                while (curr)
                {
                    insert(curr->key, curr->value);
                    curr = curr->next;
                }
            }
        }
        return *this;
    }

    // Hiển thị thống kê (Debug)
    void printStats() const
    {
        cout << "=== Hash Table Stats ===" << endl;
        cout << "Size: " << size << endl;
        cout << "Count: " << count << endl;
        cout << "Load Factor: " << (double)count / size << endl;

        int maxChain = 0, emptyBuckets = 0;
        for (int i = 0; i < size; i++)
        {
            int chainLen = 0;
            Node *curr = table[i];
            while (curr)
            {
                chainLen++;
                curr = curr->next;
            }
            if (chainLen == 0)
                emptyBuckets++;
            if (chainLen > maxChain)
                maxChain = chainLen;
        }
        cout << "Max Chain Length: " << maxChain << endl;
        cout << "Empty Buckets: " << emptyBuckets << " (" << (double)emptyBuckets / size * 100 << "%)" << endl;
    }
};

#endif // HASHTABLE_H
