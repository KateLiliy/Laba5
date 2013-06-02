/**
\file AhoCorasick.cpp 
*/

/**
\fn int main() 
*/

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
using namespace std;

 /** 
@class  AhoCorasick
 ласс AhoCorasick содержит конструктор c параметрами
*/

class AhoCorasick ///создаем класс AhoCorasick
{
public:
	
 /// @param substr Ц параметр 1
 /// @param begin - параметр 2
 /// @param end - параметр 3
    typedef void (*Callback) (const char* substr, int begin, int end);
 
    ~AhoCorasick()
    {
        queue<BorNode*> q;
        for(map<char, BorNode*>::const_iterator iter = root.links.begin();
            iter != root.links.end(); ++iter)
            q.push(iter->second);
        while(!q.empty())
        {
            BorNode* current_node = q.front();
            q.pop();
            for(map<char, BorNode*>::const_iterator iter = current_node->links.begin();
                iter != current_node->links.end(); ++iter)
                q.push(iter->second);
            delete current_node;
        }
    }
 
 
  /** 
  @function Add string 
  */
    
    void AddString(const char* str)
		///@param str- параметр
    {
        BorNode* node = &root;
        for(const char* s = str; *s; ++s)
        {
            map<char, BorNode*>::iterator iter = node->links.find(*s);
            if(iter != node->links.end())
                node = iter->second;
            else
            {
                BorNode* new_node = new BorNode;
                node->links[*s] = new_node;
                node = new_node;
            }
        }
        node->out = words.size();
        words.push_back(str);
    }
 
    /** 
  @function Init 
  calculate bad functions
  */
    void Init()
    {
        root.fail = &root;
        queue<BorNode*> q;
        q.push(&root);
        while(!q.empty())
        {
            BorNode* current_node = q.front();
            q.pop();
            for(map<char, BorNode*>::const_iterator iter = current_node->links.begin();
                iter != current_node->links.end(); ++iter)
            {
                BorNode* child = iter->second;
                char symb = iter->first;
                q.push(child);
 
                BorNode* parent_fail = current_node->fail;
                while(true)
                {
                    map<char, BorNode*>::const_iterator it = parent_fail->links.find(symb);
                    if(it != parent_fail->links.end())
                    {
                        child->fail = it->second != child ? it->second : &root;
                        if(child->out < 0)
                            child->out = child->fail->out;
                        break;
                    }
                    if(parent_fail == &root)
                    {
                        child->fail = &root;
                        break;
                    }
                    else
                        parent_fail = parent_fail->fail;
                }
            }
        }
    }
	
	/** 
  @function Search 
  */

    void Search(const char* str, Callback callback)
    {
        BorNode* current_node = &root;
        for(int pos = 1; *str; ++str, ++pos)
        {
            map<char, BorNode*>::const_iterator iter = current_node->links.find(*str);
            while(iter == current_node->links.end())
            {
                current_node = current_node->fail;
                iter = current_node->links.find(*str);
                if(current_node == current_node->fail)
                    break;
            }
            if(iter != current_node->links.end())
            {
                current_node = iter->second;
 
                if(current_node->out >= 0)
                    callback(words[current_node->out].c_str(), pos - words[current_node->out].length(), pos - 1);
            }
        }
    }
 
private:
    struct BorNode
    {
        BorNode() : fail(NULL), out(-1) {}
 
        map<char, BorNode*> links;
        BorNode* fail;
        int out;
    };
 
    BorNode root;
    vector<string> words;
};

/** 
  @function Print 
  */

void print(const char* str, int start, int end)
	///@param str - параметр 1
	///@param start - параметр 2
	///@param end - параметр 3
{
   cout << "Naidena podstroka " << str << " (nachalo " << start << ", konec " << end << ")\n";
}
 /** 
@function main
√лавна€ функци€
*/

int main()
{
    AhoCorasick ak;
    ak.AddString("test");					///»нициализаци€
    ak.AddString("rok");					
    ak.AddString("sto");					
    ak.AddString("st");						
    ak.Init();
    ak.Search("testovaya_stroka", print);
}