#include<bits/stdc++.h>
using namespace std;
#define ll unsigned long long int
#include "bTree.h"
struct data{
    unsigned int key;
    string name;
};
int main(){
    btree:
        btree::BTree<data> Tree(4);
        vector<data *> elements;
        vector<unsigned int> keys = {10, 80, 30, 90, 85, 40, 50, 60, 70, 95, 100};
        for (unsigned int i = 0; i < keys.size(); i++)
        {
            data *d1 = new data();
            d1->key = keys[i];
            d1->name = "name" + to_string(i);
            Tree.insert(d1);
            //elements.push_back(d1);
        }
        Tree.Traverse([](data *element) -> void
                      { cout << element->key << " " << element->name << endl; });

        data* element = Tree.root->children[1]->data[1];
        cout <<" element key is "<< element->key<<endl;
      
        return 0;
}