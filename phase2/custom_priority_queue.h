#include <iostream>
#include <vector>
using namespace std;

class minpriorityqueue
{
    vector<vector<string>> pq;

public:
    minpriorityqueue() {}

    bool isEmpty()
    {
        return pq.size() == 0;
    }

    int getsize()
    {
        return pq.size();
    }

    vector<string> getmin()
    {
        vector<string> temp;
        if (isEmpty())
            return temp;
        return pq[0];
    }

    void insert(vector<string> key)
    {
        pq.push_back(key);
        int childi = pq.size() - 1;
        int parenti;
        while (childi > 0)
        {
            parenti = (childi - 1) / 2;
            if (stoi(pq[parenti][4]) > stoi(pq[childi][4]))
                swap(pq[childi], pq[parenti]);
            else if (stoi(pq[parenti][4]) == stoi(pq[childi][4]))
            {
                if (stoi(pq[parenti][0]) > stoi(pq[childi][0]))
                {
                    swap(pq[childi], pq[parenti]);
                }
                else if (stoi(pq[parenti][0]) == stoi(pq[childi][0]))
                {
                    int result = pq[parenti][1].compare(pq[childi][1]);
                    if (result > 0)
                    {
                        swap(pq[childi], pq[parenti]);
                    }
                    else
                        break;
                }
                else
                    break;
            }
            else
                break;
            childi = parenti;
        }
    }

    void removemin()
    {
        if (isEmpty())
            return;
        swap(pq[0], pq[pq.size() - 1]);
        pq.pop_back();
        int parenti = 0;
        int childi;
        while (true)
        {
            if (2 * parenti + 1 >= pq.size())
                break;
            if (2 * parenti + 2 >= pq.size())
            {
                childi = 2 * parenti + 1;
            }
            else if (stoi(pq[2 * parenti + 1][4]) > stoi(pq[2 * parenti + 2][4]))
            {
                childi = 2 * parenti + 2;
            }
            else if (stoi(pq[2 * parenti + 1][4]) == stoi(pq[2 * parenti + 2][4]))
            {
                if (stoi(pq[2 * parenti + 1][0]) > stoi(pq[2 * parenti + 2][0]))
                {
                    childi = 2 * parenti + 2;
                }
                else if (stoi(pq[2 * parenti + 1][0]) == stoi(pq[2 * parenti + 2][0]))
                {
                    int result = pq[2 * parenti + 1][1].compare(pq[2 * parenti + 2][1]);
                    if (result > 0)
                    {
                        childi = 2 * parenti + 2;
                    }
                    else
                    {
                        childi = 2 * parenti + 1;
                    }
                }
                else
                {
                    childi = 2 * parenti + 1;
                }
            }
            else
            {
                childi = 2 * parenti + 1;
            }
            if (stoi(pq[parenti][4]) > stoi(pq[childi][4]))
                swap(pq[childi], pq[parenti]);
            else if (stoi(pq[parenti][4]) == stoi(pq[childi][4]))
            {
                if (stoi(pq[parenti][0]) > stoi(pq[childi][0]))
                {
                    swap(pq[childi], pq[parenti]);
                }
                else if (stoi(pq[parenti][0]) == stoi(pq[childi][0]))
                {
                    int result = pq[parenti][1].compare(pq[childi][1]);
                    if (result > 0)
                    {
                        swap(pq[childi], pq[parenti]);
                    }
                    else
                        break;
                }
                else
                    break;
            }
            else
                break;
            parenti = childi;
        }
    }
};

class maxpriorityqueue
{
    vector<vector<string>> pq;

public:
    maxpriorityqueue() {}

    bool isEmpty()
    {
        return pq.size() == 0;
    }

    int getsize()
    {
        return pq.size();
    }

    vector<string> getmax()
    {
        vector<string> temp;
        if (isEmpty())
            return temp;
        return pq[0];
    }

    void insert(vector<string> key)
    {
        pq.push_back(key);
        int childi = pq.size() - 1;
        int parenti;
        while (childi > 0)
        {
            parenti = (childi - 1) / 2;
            if (stoi(pq[parenti][4]) < stoi(pq[childi][4]))
                swap(pq[childi], pq[parenti]);
            else if (stoi(pq[parenti][4]) == stoi(pq[childi][4]))
            {
                if (stoi(pq[parenti][0]) > stoi(pq[childi][0]))
                {
                    swap(pq[childi], pq[parenti]);
                }
                else if (stoi(pq[parenti][0]) == stoi(pq[childi][0]))
                {
                    int result = pq[parenti][1].compare(pq[childi][1]);
                    if (result > 0)
                    {
                        swap(pq[childi], pq[parenti]);
                    }
                    else
                        break;
                }
                else
                    break;
            }
            else
                break;
            childi = parenti;
        }
    }

    void removemax()
    {
        if (isEmpty())
            return;
        swap(pq[0], pq[pq.size() - 1]);
        pq.pop_back();
        int parenti = 0;
        int childi;
        while (true)
        {
            if (2 * parenti + 1 >= pq.size())
                break;
            if (2 * parenti + 2 >= pq.size())
            {
                childi = 2 * parenti + 1;
            }
            else if (stoi(pq[2 * parenti + 1][4]) < stoi(pq[2 * parenti + 2][4]))
            {
                childi = 2 * parenti + 2;
            }
            else if (stoi(pq[2 * parenti + 1][4]) == stoi(pq[2 * parenti + 2][4]))
            {
                if (stoi(pq[2 * parenti + 1][0]) > stoi(pq[2 * parenti + 2][0]))
                {
                    childi = 2 * parenti + 2;
                }
                else if (stoi(pq[2 * parenti + 1][0]) == stoi(pq[2 * parenti + 2][0]))
                {
                    int result = pq[2 * parenti + 1][1].compare(pq[2 * parenti + 2][1]);
                    if (result > 0)
                    {
                        childi = 2 * parenti + 2;
                    }
                    else
                        break;
                }
                else
                {
                    childi = 2 * parenti + 1;
                }
            }
            else
            {
                childi = 2 * parenti + 1;
            }
            if (stoi(pq[parenti][4]) < stoi(pq[childi][4]))
                swap(pq[childi], pq[parenti]);
            else if (stoi(pq[parenti][4]) == stoi(pq[childi][4]))
            {
                if (stoi(pq[parenti][0]) > stoi(pq[childi][0]))
                {
                    swap(pq[childi], pq[parenti]);
                }
                else if (stoi(pq[parenti][0]) == stoi(pq[childi][0]))
                {
                    int result = pq[parenti][1].compare(pq[childi][1]);
                    if (result > 0)
                    {
                        swap(pq[childi], pq[parenti]);
                    }
                    else
                        break;
                }
                else
                    break;
            }
            else
                break;
            parenti = childi;
        }
    }
};