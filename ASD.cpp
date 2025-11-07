#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>

/*
8190.52 1000000
173.665 100000
4.82586 10000
0.172537 1000
*/

struct Date
{
    int day;
    int month;
    int year;

    bool operator > (const Date& date) const
    {
        if (year > date.year)
            return true;
        if (year < date.year)
            return false;
        if (month > date.month)
            return true;
        if (month < date.month)
            return false;
        if (day > date.day)
            return true;
        if (day < date.day)
            return false;
        return false;
    }

    bool operator < (const Date& date) const
    {
        if (year < date.year)
            return true;
        if (year > date.year)
            return false;
        if (month < date.month)
            return true;
        if (month > date.month)
            return false;
        if (day < date.day)
            return true;
        if (day > date.day)
            return false;
        return false;
    }

    bool operator == (const Date& date) const
    {
        return year == date.year and month == date.month and day == date.day;
    }
};

struct FIO
{
    std::string f;
    std::string i;
    std::string o;

    bool operator > (const FIO& fio) const
    {
        if (f > fio.f)
            return true;
        if (f < fio.f)
            return false;
        if (i > fio.i)
            return true;
        if (i < fio.i)
            return false;
        if (o > fio.o)
            return true;
        if (o < fio.o)
            return false;
        return false;
    }

    bool operator < (const FIO& fio) const
    {
        if (f < fio.f)
            return true;
        if (f > fio.f)
            return false;
        if (i < fio.i)
            return true;
        if (i > fio.i)
            return false;
        if (o < fio.o)
            return true;
        if (o > fio.o)
            return false;
        return false;
    }

    bool operator == (const FIO& fio) const
    {
        return f == fio.f and i == fio.i and o == fio.o;
    }
};

struct Key
{
    Date date;
    FIO fio;
    int num;
};

struct Node
{
    Key key;
    Node* next;
    Node* prev;
    //31.12.2012 Strelkin Maksim Andreevich
    Node(std::string line)
    {
        next = nullptr;
        prev = nullptr;

        std::string::iterator iter = line.begin();

        std::string substr(iter, iter + 2);
        key.date.day = std::stoi(substr);

        substr = std::string(iter + 3, iter + 5);
        key.date.month = std::stoi(substr);

        substr = std::string(iter + 6, iter + 10);
        key.date.year = std::stoi(substr);

        substr = "";

        iter = line.begin() + 11;
        while (*iter != ' ')
            substr += *iter++;
        key.fio.f = substr;
        iter++;
        substr = "";

        while (*iter != ' ')
            substr += *iter++;
        key.fio.i = substr;
        iter++;
        substr = "";

        while (*iter != ' ')
            substr += *iter++;
        key.fio.o = substr;
        iter++;
        substr = "";

        while (iter != line.end())
            substr += *iter++;
        key.num = std::stoi(substr);
    }
};

class List
{
    typedef Node* link;
private:
    link head, tail;
    std::map<int, link> indexTable;
public:
    List()
    {
        head = nullptr;
        tail = nullptr;

        //indexTable.insert(std::pair<int, link>(0, head));
    };

    void fillList(std::ifstream &data, int n)
    {
        if (head == nullptr)
        {
            std::string line;
            if (n >= 1)
            {
                std::getline(data, line);
                link newNode = new Node(line);
                head = newNode;
                tail = newNode;
                head->next = nullptr;
                head->prev = nullptr;

                if (n >= 2)
                {
                    std::getline(data, line);
                    link newNode = new Node(line);
                    head->next = newNode;
                    tail = newNode;
                    tail->prev = head;
                    tail->next = nullptr;

                    link current = tail;
                    for (int i = 3; i <= n; i++)
                    {
                        std::getline(data, line);
                        current->next = new Node(line);
                        tail = current->next;
                        tail->prev = current;
                        current = current->next;
                        tail->next = nullptr;
                    }
                }
            }
            data.close();
        }
        int step = sqrt(n) * 2;
        link current = head;
        indexTable.insert(std::pair<int, link>(0, head));
        for (int i = 0; i < n; i++)
        {
            if (i % step == 0 and i >= step)
                indexTable.insert(std::pair<int, link>(i, current));
            current = current->next;
        }
        indexTable.insert(std::pair<int, link>(n-1, tail));
    }

    void showIndexTable()
    {
        for (std::map<int, link>::iterator i = indexTable.begin(); i != indexTable.end(); i++)
        {
            std::cout << (*i).first << ", " << (*i).second << '\n';
        }
    }

    void swap(link nodeA, link nodeB)
    {
        if (head != nullptr and head != tail and nodeA != nodeB)
        {
            //if (indexTable.count())
            if (nodeA == head)
                head = nodeB;
            else if (nodeB == head)
                head = nodeA;
            if (nodeA == tail)
                tail = nodeB;
            else if (nodeB == tail)
                tail = nodeA;

            link temp;
            temp = nodeA->next;
            nodeA->next = nodeB->next;
            nodeB->next = temp;

            if (nodeA->next)
                nodeA->next->prev = nodeA;
            if (nodeB->next)
                nodeB->next->prev = nodeB;

            temp = nodeA->prev;
            nodeA->prev = nodeB->prev;
            nodeB->prev = temp;

            if (nodeA->prev)
                nodeA->prev->next = nodeA;
            if (nodeB->prev)
                nodeB->prev->next = nodeB;
        }
        for (std::map<int, link>:: iterator i = indexTable.begin(); i != indexTable.end(); i++)
        {
            if (i->second == nodeA)
                i->second = nodeB;
            else if (i->second == nodeB)
                i->second = nodeA;
        }
    }

    link getNode(int n)
    {
        if (indexTable.empty())
            return nullptr;

        std::map<int, link>::iterator right = indexTable.lower_bound(n);
        std::map<int, link>::iterator left = right;

        if (right != indexTable.begin())
            --left;

        int leftDist = (left->first <= n) ? n - left->first : INT_MAX;
        int rightDist = (right != indexTable.end()) ? right->first - n : INT_MAX;

        link current;
        int startIndex;

        if (leftDist <= rightDist) {
            current = left->second;
            startIndex = left->first;
            for (int i = startIndex; i < n; i++)
                current = current->next;
        }
        else {
            current = right->second;
            startIndex = right->first;
            for (int i = startIndex; i > n; i--)
                current = current->prev;
        }

        return current;
    }

    void heapify(int n, int i)
    {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        link lNode = nullptr, rNode = nullptr, larNode = nullptr;

        if (l < n)
            lNode = getNode(l);
        if (r < n)
            rNode = getNode(r);
        larNode = getNode(largest);
        
        if (l < n and (lNode->key.date > larNode->key.date or lNode->key.date == larNode->key.date and lNode->key.fio < larNode->key.fio))
        {
            largest = l;
            larNode = lNode;
        }
        if (r < n and (rNode->key.date > larNode->key.date or rNode->key.date == larNode->key.date and rNode->key.fio < larNode->key.fio))
        {
            largest = r;
            larNode = rNode;
        }

        if (largest != i)
        {
            swap(getNode(i), getNode(largest));
            
            heapify(n, largest);
        }
    }

    void heapSort(int n)
    {
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(n, i);

        for (int i = n - 1; i >= 0; i--)
        {
            swap(getNode(0), getNode(i));
            
            heapify(i, 0);
        }
    }

    friend std::ostream& operator << (std::ostream& stream, List& list)
    {
        if (list.head == nullptr)
            stream << "list is empty";
        else {

            for (link current = list.head; current != nullptr; current = current->next)
            {
                stream << (current->key.date.day < 10 ? "0" : "") << current->key.date.day << '.';
                stream << (current->key.date.month < 10 ? "0" : "") << current->key.date.month << '.';
                stream << current->key.date.year << ' ';
                stream << current->key.fio.f << ' ';
                stream << current->key.fio.i << ' ';
                stream << current->key.fio.o << ' ';
                stream << current->key.num << '\n';
            }
        }
        return stream;
    }
    
};
//5.39349
//
const int SIZE = 100000;

int main()
{
    std::ifstream data("data100000.txt");
    std::ofstream out("dataout.txt");
    List list;
    list.fillList(data, SIZE);
    //std::cout << list;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    list.heapSort(SIZE);
    //std::cout << list;
    std::chrono::steady_clock::time_point finish = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = finish - start;
    out << list;
    out << "elapsed seconds: " << elapsed_seconds.count();
    std::cout << "elapsed seconds: " << elapsed_seconds.count();
    return 0;
}