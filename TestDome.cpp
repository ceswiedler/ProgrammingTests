#include "stdafx.h"

#include <string>
#include <sstream>
#include <queue>
#include <iomanip>
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <map>
#include <chrono>
#include <unordered_map>
#include <conio.h>
#include <assert.h>

using namespace std;

#pragma warning(disable: 4018)

struct Node
{
    std::string name;
    Node* leftChild;
    Node* rightChild;
    Node* rightSibling;

    Node() { leftChild = rightChild = rightSibling = nullptr; }
};

class NotImplemented : public std::logic_error
{
public:
    NotImplemented() : std::logic_error("Not implemented") {}
};

class Base
{
public:
    std::string cd(std::string sourcePath, std::string destPath)
    {
        throw NotImplemented();
    }

    int first_repeated_pos(const char* a_string, const char* b_string)
    {
        throw NotImplemented();
    }

    void turnOnBits(unsigned char* array, size_t firstBit, size_t lastBit)
    {
        throw NotImplemented();
    }

    void connectSiblings(Node* root)
    {
        throw NotImplemented();
    }
};

class ChristopherSwiedler : public Base
{
public:
#ifndef _MAX_PATH
#define _MAX_PATH 4096
#endif

    void addPart(std::vector<char*>& parts, char* part)
    {
        if (*part != 0)
        {
            if (strcmp(part, "..") == 0)
            {
                if (parts.size() > 0)
                {
                    parts.pop_back();
                }
            }
            else if (strcmp(part, ".") != 0)
            {
                parts.push_back(part);
            }
        }
    }

    std::string cd(std::string sourcePath, std::string destPath)
    {
        if (destPath.length() == 0 || destPath == ".")
        {
            return sourcePath;
        }

        std::string fullPath;

        if (destPath[0] == '/')
        {
            fullPath = destPath;
        }
        else
        {
            fullPath = sourcePath + "/" + destPath;
        }

        if (fullPath.length() > _MAX_PATH)
        {
            throw std::runtime_error("Invalid length");
        }

        bool absolute = (fullPath[0] == '/');

        // parse the buffer into path parts

        char inputBuffer[_MAX_PATH + 1];
        strncpy(inputBuffer, fullPath.c_str(), fullPath.length());
        inputBuffer[fullPath.length()] = 0;

        std::vector<char*> parts;

        char* partBegin = inputBuffer;
        char* partEnd = inputBuffer;

        for (partEnd = inputBuffer; *partEnd != 0; ++partEnd)
        {
            if (*partEnd == '/')
            {
                *partEnd = 0;
                addPart(parts, partBegin);

                partBegin = partEnd + 1;
            }
        }

        // add final part

        *partEnd = 0;
        addPart(parts, partBegin);

        // using another buffer, reconstruct the normalized path. it should theoretically be possible
        // to use the same buffer again since all we're doing is removing path elements,
        // but symlinks and such could length the path

        char outputBuffer[_MAX_PATH + 1];

        partBegin = outputBuffer;
        if (absolute)
        {
            *partBegin = '/';
            ++partBegin;
        }

        for (size_t i = 0; i < parts.size(); ++i)
        {
            char* part = parts[i];
            size_t len = strlen(part);
            strncpy(partBegin, part, len);
            partBegin += len;

            if (i < parts.size() - 1)
            {
                *partBegin = '/';
                ++partBegin;
            }
        }

        *partBegin = 0;

        return outputBuffer;
    }

    int first_repeated_pos(const char* a_string, const char* b_string)
    {
        if (a_string == nullptr || b_string == nullptr)
        {
            return -1;
        }

        // array to track first position of letters in a_string, initialized to all -1

        int a_letter_indices[256];
        memset(a_letter_indices, 0xff, sizeof(a_letter_indices));

        // find the lowest index where each char in a_string appears

        int a_index = 0;
        while (a_string[a_index] != 0)
        {
            unsigned char a_char = a_string[a_index];
            if (a_letter_indices[a_char] == -1)
            {
                // not seen before, record
                a_letter_indices[a_char] = a_index;
            }

            ++a_index;
        }

        int lowest_repeated_index = -1;

        // find letters in b_string which appeared in a_string

        int b_index = 0;
        while (b_string[b_index] != 0)
        {
            unsigned char b_char = b_string[b_index];

            if (a_letter_indices[b_char] != -1)
            {
                // the letter was in a_string. which was earlier?
                int lower_repeated_index = std::min(a_letter_indices[b_char], b_index);

                if (lowest_repeated_index == -1 || lowest_repeated_index > lower_repeated_index)
                {
                    // record as our new lowest
                    lowest_repeated_index = lower_repeated_index;
                    return lowest_repeated_index;
                }
            }

            ++b_index;
        }

        return -1;
    }

    void turnOnBits(unsigned char* array, size_t firstBit, size_t lastBit)
    {
        size_t firstByte = firstBit / 8;
        size_t lastByte = lastBit / 8;

        unsigned char firstByteVal = ~(0xFF >> (8 - (firstBit % 8)));
        unsigned char lastByteVal = 0xFF >> (8 - ((lastBit % 8) + 1));

        if (firstByte == lastByte)
        {
            array[firstByte] = firstByteVal & lastByteVal;
            return;
        }

        array[firstByte] = firstByteVal;

        for (size_t i = firstByte + 1; i <= lastByte - 1; ++i)
        {
            array[i] = 0xFF;
        }

        array[lastByte] = lastByteVal;
    }

    void connectSiblings(Node* root)
    {
        std::queue<Node*> q;
        q.push(root);

        // null marker to represent end of current level
        q.push(nullptr);

        // Do Level order of tree using nullptr markers
        while (!q.empty())
        {
            Node *p = q.front();
            q.pop();

            if (p == nullptr)
            {
                // if p is nullptr and queue is not empty, push nullptr to mark 
                // nodes at this level are visited
                if (!q.empty())
                {
                    q.push(nullptr);
                }
            }
            else
            {
                // next element in queue represents next 
                // node at current Level 
                p->rightSibling = q.front();

                // push left and right children of current
                // node
                if (p->leftChild)
                    q.push(p->leftChild);
                if (p->rightChild)
                    q.push(p->rightChild);
            }
        }
    }
};

class TestDome : public Base
{
public:
    vector<string> split(const char *str, char c = ' ')
    {
        vector<string> result;
        do
        {
            const char *begin = str;

            while (*str != c && *str)
                str++;

            result.push_back(string(begin, str));
        } while (0 != *str++);
        return result;
    }

    std::string cd(std::string sourcePath, std::string newPath)
    {
        std::string s;
        if (newPath[0] != '/')
            s = sourcePath;

        vector<string> v = split(newPath.c_str(), '/');

        for (unsigned int i = 0; i < v.size(); i++) {
            string x = v[i];
            if (x.length() == 0) {
                if (i == 0)
                    s = "/";
                else
                    s += "/";
                continue;
            }
            if (x == "..") {
                // back up
                size_t pos = s.find_last_of("/");
                s.erase(pos);
                if (s.length() == 0) s = "/";
            }
            else {
                // only add if not trailing
                if (s.find_last_of("/") != s.length() - 1)
                    s += "/";
                s += x;
            }
        }

        return s;
    }

    int first_repeated_pos(const char* a, const char* b)
    {
        if (a == 0 || b == 0) return -1;
        int pos[256] = {};
        for (int i = 0; a[i]; i++)
            if (pos[a[i]] == 0) pos[a[i]] = i + 1;

        for (int i = 0; b[i]; i++)
            if (pos[b[i]]) return (pos[b[i]] > i + 1 ? i : pos[b[i]] - 1);

        return -1;
    }
};

class Exponential : public Base
{
public:
    int first_repeated_pos(const char* a_string, const char* b_string)
    {
        size_t aLen = strlen(a_string);
        size_t bLen = strlen(b_string);

        int minMatch = -1;
        char lastA = 0;

        for (int aPos = 0; aPos < aLen; ++aPos)
        {
            if (a_string[aPos] == lastA)
            {
                continue;
            }

            lastA = a_string[aPos];

            for (int bPos = 0; bPos < bLen; ++bPos)
            {
                if (a_string[aPos] == b_string[bPos])
                {
                    if (minMatch == -1 || aPos < minMatch || bPos < minMatch)
                    {
                        minMatch = std::min(aPos, bPos);
                    }
                }
            }
        }

        return minMatch;
    }

};

#pragma region Tests

template <typename T>
void AssertPath(const char* source, const char* cd, const char* correctAnswer)
{
    T t;
    std::string destination = t.cd(source, cd);
    if (destination.length() > 1 && destination[destination.length() - 1] == '/')
    {
        destination = destination.substr(0, destination.length() - 1);
    }

    if (destination != correctAnswer)
    {
        std::cout << "Failure: Path(" << source << ").cd(" << cd << ") is " << destination << " instead of " << correctAnswer << std::endl;
    }
    else
    {
        //std::cout << "Success : Path(" << source << ").cd(" << cd << ") == " << destination << std::endl;
    }
}

template <typename T>
void AssertPathTiming(const char* source, const char* cd, int count)
{
    auto start = std::chrono::high_resolution_clock::now();
    T t;

    for (int i = 0; i < count; ++i)
    {
        t.cd(source, cd);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "Path timing: " << (duration.count() * 1000) << " ms" << std::endl;
}

template <typename T>
void AssertFirstRepeatedPos(const char* first, const char* second, int expectedIndex)
{
    const int longLength = 100000;

    // construct strings with alternating chars to prevent 'optimizations' that handle
    // repeated characters

    std::string longFirst;
    for (int i = 0; i < longLength / 2; ++i)
    {
        longFirst += "!?";
    }

    longFirst += first;

    std::string longSecond;
    for (int i = 0; i < longLength / 2; ++i)
    {
        longSecond += "@#";
    }

    longSecond += second;

    T t;
    int index = t.first_repeated_pos(longFirst.c_str(), longSecond.c_str());

    if (index - longLength != expectedIndex)
    {
        std::cout << "Failure: first repeated position of " << first << " and " << second << " is " << index - longLength << " instead of " << expectedIndex << std::endl;
    }
    else
    {
        //std::cout << "Success: first repeated position of " << first << " and " << second << " is " << index << std::endl;
    }
}

template <typename T>
void AssertFirstRepeatedPosTiming(const char* first, const char* second, int expectedIndex, int count)
{
    const int longLength = 100000;

    // construct strings with alternating chars to prevent 'optimizations' that handle
    // repeated characters

    std::string longFirst;
    for (int i = 0; i < longLength / 2; ++i)
    {
        longFirst += "!?";
    }

    longFirst += first;

    std::string longSecond;
    for (int i = 0; i < longLength / 2; ++i)
    {
        longSecond += "@#";
    }

    longSecond += second;

    T t;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        int index = t.first_repeated_pos(longFirst.c_str(), longSecond.c_str());

        if (index - longLength != expectedIndex)
        {
            std::cout << "Failure: first repeated position of " << first << " and " << second << " is " << index - longLength << " instead of " << expectedIndex << std::endl;
            return;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "FirstRepeatedPos timing: " << (duration.count() * 1000) << " ms" << std::endl;
}

bool isBitOn(unsigned char* array, size_t testBit)
{
    size_t byteOffset = testBit / 8;
    unsigned char byteMask = 1 << (testBit % 8);

    return (array[byteOffset] & byteMask) == byteMask;
}

std::string printBits(unsigned char* array, size_t firstBit, size_t lastBit)
{
    std::stringstream result;
    size_t lastPossibleBit = (((lastBit / 8) + 1) * 8) - 1;

    for (int i = 0; i <= lastPossibleBit; ++i)
    {
        result << isBitOn(array, i) ? "1" : "0";
    }

    return result.str();
}

std::string printCorrectBits(size_t firstBit, size_t lastBit)
{
    std::stringstream result;
    size_t lastPossibleBit = (((lastBit / 8) + 1) * 8) - 1;

    for (size_t i = 0; i < firstBit; ++i)
    {
        result << "0";
    }

    for (size_t i = firstBit; i <= lastBit; ++i)
    {
        result << "1";
    }

    for (size_t i = lastBit + 1; i <= lastPossibleBit; ++i)
    {
        result << "0";
    }

    return result.str();
}

template <typename T>
void AssertBitsOn(size_t firstBit, size_t lastBit)
{
    size_t byteCount = (lastBit / 8) + 1;
    unsigned char* array = new unsigned char[byteCount];
    memset(array, 0, byteCount);

    T t;
    t.turnOnBits(array, firstBit, lastBit);

    for (size_t i = 0; i < firstBit; ++i)
    {
        if (isBitOn(array, i) != false)
        {
            std::cout << "Failure: array bit " << i << " is on when it should be off (first " << firstBit << ", last " << lastBit << "): " << printBits(array, firstBit, lastBit) << " instead of " << printCorrectBits(firstBit, lastBit) << std::endl;
            return;
        }
    }

    for (size_t i = firstBit; i <= lastBit; ++i)
    {
        if (isBitOn(array, i) != true)
        {
            std::cout << "Failure: array bit " << i << " is off when it should be on: (first " << firstBit << ", last " << lastBit << "): " << printBits(array, firstBit, lastBit) << " instead of " << printCorrectBits(firstBit, lastBit) << std::endl;
            return;
        }
    }

    size_t lastPossibleBit = (((lastBit / 8) + 1) * 8) - 1;

    for (size_t i = lastBit + 1; i < lastPossibleBit; ++i)
    {
        if (isBitOn(array, i) != false)
        {
            std::cout << "Failure: array bit " << i << " is on when it should be off: (first " << firstBit << ", last " << lastBit << "): " << printBits(array, firstBit, lastBit) << " instead of " << printCorrectBits(firstBit, lastBit) << std::endl;
            return;
        }
    }

    std::cout << "Success: (first " << firstBit << ", last " << lastBit << "): " << printBits(array, firstBit, lastBit) << std::endl;
}

std::string printSiblings(Node* root)
{
    std::stringstream ss;

    Node* nextLevel = nullptr;
    Node* cur = root;

    do
    {
        while (cur != nullptr)
        {
            ss << cur->name;

            if (nextLevel == nullptr)
            {
                nextLevel = cur->leftChild ? cur->leftChild : cur->rightChild;
            }

            cur = cur->rightSibling;
        }

        ss << std::endl;
        cur = nextLevel;
        nextLevel = nullptr;
    } while (cur != nullptr);

    return ss.str();
}

Node* createTree1()
{
    Node* a = new Node();
    Node* b = new Node();
    Node* c = new Node();
    Node* d = new Node();
    Node* e = new Node();
    Node* f = new Node();

    a->name = "A";
    a->leftChild = b;
    a->rightChild = c;

    b->name = "B";
    b->leftChild = d;
    b->rightChild = e;

    c->name = "C";
    c->rightChild = f;

    d->name = "D";

    e->name = "E";

    f->name = "F";

    return a;
}

Node* createTree2()
{
    Node* a = new Node();
    Node* b = new Node();
    Node* c = new Node();
    Node* d = new Node();
    Node* e = new Node();
    Node* f = new Node();
    Node* g = new Node();
    Node* h = new Node();
    Node* i = new Node();

    a->name = "A";
    a->leftChild = b;
    a->rightChild = c;

    b->name = "B";
    b->leftChild = d;
    b->rightChild = e;

    c->name = "C";
    c->rightChild = f;

    d->name = "D";

    e->name = "E";
    e->rightChild = g;

    f->name = "F";
    f->rightChild = h;

    g->name = "G";

    h->name = "H";
    h->rightChild = i;

    i->name = "I";

    return a;
}

template <typename T>
void AssertSiblings()
{
    T t;

    Node* root1 = createTree1();
    t.connectSiblings(root1);

    std::cout << printSiblings(root1) << std::endl;

    Node* root2 = createTree2();
    t.connectSiblings(root2);

    std::cout << printSiblings(root2) << std::endl;
}


#pragma endregion 

template <typename T>
void Run(const char* name)
{
    std::cout << name << std::endl << std::endl;

    try
    {
        AssertPath<T>("/aa/bbb", "c", "/aa/bbb/c");
        AssertPath<T>("/aa/bbb/", "c/dddd", "/aa/bbb/c/dddd");
        AssertPath<T>("/", "ccc", "/ccc");
        AssertPath<T>("/aaa/bbb", "..", "/aaa");
        AssertPath<T>("/aa/bbb", "../..", "/");
        AssertPath<T>("/aaa/bbb", "../c", "/aaa/c");
        AssertPath<T>("/aa/bbb", "../../ccc", "/ccc");
        AssertPath<T>("/aa/bb", "../../ccc/dddd", "/ccc/dddd");
        AssertPath<T>("/aa/bbb", "../ccc/../dddd", "/aa/dddd");
        AssertPath<T>("/aa/bbb", "../../ccc/../ddd", "/ddd");
        AssertPath<T>("/aa/bbb", "/ccc", "/ccc");
        AssertPath<T>("/aaa/bbbb", "/ccc/dd/../eee", "/ccc/eee");

        AssertPathTiming<T>("/sfsdg/sdfkjkjq1/asdh123/askdjkaj", "../../alksjd/1kj2jkjas/akjakj1/../../akjjkaj/aksdja", 10000);
    }
    catch (const NotImplemented&)
    {
    }

    try
    {
        AssertFirstRepeatedPos<T>("abc", "dec", 2);
        AssertFirstRepeatedPos<T>("abcbabcbabcasjajz", "xxxzxxxxxxxxxxxxxxxxxxx", 3);

        AssertFirstRepeatedPosTiming<T>("aaaac", "bbbbc", 4, 100);
    }
    catch (const NotImplemented&)
    {
    }

    try
    {
        AssertBitsOn<T>(2, 12);
        AssertBitsOn<T>(12, 19);
        AssertBitsOn<T>(12, 20);
        AssertBitsOn<T>(12, 21);
        AssertBitsOn<T>(0, 8);
        AssertBitsOn<T>(0, 12);
        AssertBitsOn<T>(2, 42);
        AssertBitsOn<T>(4, 4);
        AssertBitsOn<T>(2, 7);
        AssertBitsOn<T>(42, 42);
    }
    catch (const NotImplemented&)
    {
    }

    try
    {
        AssertSiblings<T>();
    }
    catch (const NotImplemented&)
    {
    }

    std::cout << std::endl;
}

#define RUN(Tester) Run<Tester>(#Tester)

int main(int argc, const char* argv[])
{
    RUN(ChristopherSwiedler);
    RUN(TestDome);

    std::cout << "Press any key...";
    _getch();
}
