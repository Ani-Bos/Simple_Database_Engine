#ifndef BTREE_H
#define BTREE_H
namespace btree
{   template<typename T>
    void shiftRight(T* arr , int index , int& size){
        for (int i = size-1; i >= index;i--){
            arr[i+1] = arr[i];
        }
        size++;
    }

    template<typename T>
    void ShiftLeft(T* arr , int idx , int & size){
        for(;idx<=size-1;idx++){
            arr[idx] = arr[idx + 1];
        }
        size--;
    }
    template<typename T>
    class Node{
      public:
          Node<T> **children;//pointer to pointer array of children pointer
          T **data;//array of data pointers so ** 2 is used which has keys

          int m; //deghree of tree
          int divisor; //index of divisor when it will be split
          int curr_number_of_keys;
          int min_number_of_keys;//min no of key = ceil(m/2)-1
          bool isLeaf; //no left and right pointers
          Node(int m1 , bool leaf = true){
              this->m = m1;
              this->divisor = ceil(m / 2.0) - 1;
              this->curr_number_of_keys=0;
              this->min_number_of_keys = ceil(m / 2.0) - 1;
              this->isLeaf = leaf;
              children = new Node<T> *[m + 1];
              data = new T *[m];
          }

          Node<T> *split(T *&upshiftelement){
            // picking middle key
            //keys left to middle stays in current node
            //while keys right to midlle node joins new right node
            //returnzs new right node
              Node<T> *right = new Node<T>(m, isLeaf);
              upshiftelement = data[divisor];
              data[divisor] = nullptr;

              for(int i=divisor+1;i<curr_number_of_keys;i++){
                  right->data[i - divisor - 1] = data[i];
                  data[i] = nullptr;
                  right->curr_number_of_keys++;

              }
              curr_number_of_keys -= right->curr_number_of_keys + 1;

              if(!isLeaf){
                  for (int i = divisor + 1; i <= m; i++)
                  {
                      right->children[i - divisor - 1] = children[i];
                      children[i] = nullptr;
                      right->curr_number_of_keys++;
                  }
              }
              return right;
          }

          Node<T>* insertElement(T *element, T *&upshiftelement){

            //if overflown means exceeded the minum number of keys so node has to broken in middle so child pointer also need to reference correctly and also update reference for parent

          //we want to insert an element right so tree must be autombalancing
            int i = 0;
            while(i<curr_number_of_keys && element->key > data[i]->key){
                i++;
            }
            if(isLeaf){
                shiftRight<T *>(data, i, curr_number_of_keys);
                data[i] = element;

                if (curr_number_of_keys == m)
                {
                    // opverflow break shift
                    return split(upshiftelement);
                }
            }
            else{
                //pass down to child
                Node<T> *right = children[i]->insertElement(element, upshiftelement);

                if(right){
                    //overflow in child nopw split based on divisot and righht branch
                    shiftRight<T *>(data, i, curr_number_of_keys);
                    data[i] = upshiftelement;

                    //add child
                    int j = curr_number_of_keys;
                    shiftRight<Node<T> *>(children, i + 1, j);
                    children[i + 1] = right;

                    if(curr_number_of_keys==m){
                        return split(upshiftelement);
                    }
                }
            }
            upshiftelement = nullptr;
            return nullptr;
                    }

          T *&search(int key){
              int i = 0;
              for (; i < curr_number_of_keys && key>=data[i]->key;i++){
                if(key==data[i]->key){
                    //found the key
                    return data[i];
                }
              }
              //if no children means leaf return nullpt
              if(isLeaf){
                  return nullptr;
              }
              return children[i]->search(key);
          }

          void Traverse(void(itemViewer)(T *data)){
            for(int i=0;i<curr_number_of_keys;i++){
                if(!isLeaf){
                    //it means it has left and right children
                    children[i]->Traverse(itemViewer);
                }
                itemViewer(data[i]);
            }
            if(!isLeaf){
                //get rightmost children
                children[curr_number_of_keys] ->Traverse(itemViewer);
            }
          }
          void clear(){
            
              for (int i = 0; i < curr_number_of_keys;i++){
                if(!isLeaf){
                    children[i]->clear();
                    children[i] = nullptr;
                }
                data[i] = nullptr;
              }
              if(!isLeaf){
                  children[curr_number_of_keys]->clear();
                  children[curr_number_of_keys] = nullptr;
              }
              children = nullptr;
              data = nullptr;
          }
         
          

          T* getPredecessor(int idx){
              Node<T *> current_node = children[idx];
              while(!current_node->isLeaf){
                //get last child
                current_node = current_node->children[current_node->curr_number_of_keys]
              }
              return current_node->data[current_node->curr_number_of_keys-1]
          }

          T* getSuccessor(int idx){
              Node<T *> current_node = children[idx];
              while (!current_node->isLeaf)
              {
                  // get first child
                  current_node = current_node->children[0]
              }
              return current_node->data[0]
          }

          void mergeChildren(int leftidx){
              Node<T *> child = children[leftidx];
              Node<T *> siblings = children[leftidx + 1];

              int original_no_of_keys = child->curr_number_of_keys;
              if(original_no_of_keys==child->min_number_of_keys-1){
                  original_no_of_keys++;
              }
              //data insertion
              for (int i = 0; i < siblings->curr_number_of_keys;i++){
                  child->data[child->curr_number_of_keys++] = siblings->data[i];
              }

              //for children
              if(!child->isLeaf){
                  for (int i = 0; i < siblings->curr_number_of_keys; i++)
                  {
                      child->children[original_no_of_keys + i] = siblings->children[i];
                  }
              }
              //left shift children
              int _n = n + 1;
              ShiftLeft<T *>(children, leftidx + 1, _n);
          }

          void borrow(int base , bool toLeft){
              bool isLeaf = children[base]->isLeaf;

              //if toLeft , parent is data[base-1]
              //if to right parent is data[base] 

              if(toLeft){
                //shift data over
                shiftRight<T *>(children[base]->data, 0, children[base]->curr_no_of_keys);
                //insert parent element
                children[base]->data[0] = data[base - 1];

                //subnstyitue greatest key from left sibling'
                int _n = children[base - 1]->curr_number_of_keys;
                data[base - 1] = children[base - 1]->data[_n - 1];

                //upate sibling
                children[base - 1]->data[_n - 1] = nullptr;
                children[base - 1]->curr_number_of_keys--;

                if(!isLeaf){
                    //bring over last child from left
                    int noChildren = children[base]->curr_number_of_keys;
                    shiftRight<Node<T *>>(children[base]->children, 0, noChildren);

                    children[base]->children[0] = children[base - 1]->children[_n];
                    children[base - 1]->children[_n] = nullptr;
                }


              }


              else{
                //insert parent element
                children[base]->data[children[base]->curr_number_of_keys] = data[base];
                children[base]->curr_number_of_keys++;

                //substitute smallest key from right subling
                int _n = children[base + 1]->curr_number_of_keys;
                data[base] = children[base + 1]->data[0];

                //update sibling
                ShiftLeft<T *>(children[base + 1]->data, 0, children[bas + 1]->curr_number_of_keys);
                children[base + 1]->data[children[base + 1] -.curr_number_of_keys] = nullptr;
                //remove last key extra from shift

                if(!isLeaf){
                    int nochildren = children[base]->curr_number_of_keys;
                    children[base]->children[nochildren] = children[base + 1]->children[0];
                    ShiftLeft<Node<T *>>(children[base + 1]->children, 0, _n);
                    children[base + 1]->children[_n] = nullptr;
                    //remove last child from shift
                }

              }
          }
          // 0 --deletion succesful
          //-1--> childnot found
          //1-->cant delete as violate minimum condition

          int deleteKey(int key){
               //find location of key
               bool found = false;
               int i = 0;
               for (i = 0; i < curr_number_of_keys && key >= data[i]->key;i++){
                  if(key==data[i]->key){
                    //matlab found
                    found = true;
                    break;                  }
               }
                //key in data
               if(found){
                 //boith leaf and non leaf
                 if(isLeaf){
                    //remove ddata
                    data[i] = nullptr;
                    ShiftLeft<T *>(data, i, curr_number_of_keys);
                 }
                 //if not a leaf node
                 else{
                    //left child: chiuldren[i]  and right child: children[i+1]


                    //substityute from left
                    int n1 = children[i]->curr_number_of_keys
                    if(n1>children[i]->min_number_of_keys){
                        T *replaceElement = getPredecessor(i);
                        //replcae key recursively
                        children[i]->deletekey(replaceElement->key);
                        //borrow largest keyn from left
                        data[i] = replaceElement;
                        return 0;
                    }


                    //substitute from right

                     n1 = children[i+1]->curr_number_of_keys 
                     if (n1 > children[i]->min_number_of_keys)
                    {
                        T *replaceElement = getSuccessor(i+1);
                        // replcae key recursively
                        children[i+1]->deletekey(replaceElement->key);
                        // borrow smallest keyn from right
                        data[i] = replaceElement;
                        return 0;
                    }


                    //both children have minimum no keys so no substitution possbl

                    data[i] = nullptr;
                    shiftLeft<T *>(data, i, curr_number_of_keys);
                    mergeChildren(i); 
                 }
                 // return if violates minimum
                 return curr_number_of_keys >= min_number_of_keys ? 0 : 1;
               }
               else{
                //if key was not found
                 if(isLeaf){
                     return -1;
                     //no mo9re children to search
                 }

                 //go to children
                 int result = children[i]->deleteElement(key);
                 if(result<1){
                    //matalb key successfully deleted or not found
                    return result;
                 }
                 else{
                    //for 1 i.e child violating minumum no of keys constraint
                    //look at siblings if either have more than minimum no of keys then borrow else merge with siblings and parent

                    bool hasLeft = i > 0;
                    bool hasRight = i < curr_number_of_keys + 1;
                    //+1 because data is already remo0ved

                    if(hasLeft && children[i-1]->curr_number_of_keys>childdren[i]->min_number_of_keys){
                        //has left sibling with more than minimum keys
                        borrow(i, true);
                        return 0;
                    }

                    if (hasRight && children[i + 1]->curr_number_of_keys > children[i]->min_number_of_keys)
                    {
                        // has right sibling with more than minimum keys
                        borrow(i, false);
                        return 0;
                    }

                    // if both failed then merge children or parents
                    if(hasLeft){
                        //parent is data[i-1]
                        //insert into target child
                        shiftRight<T *>(children[i]->data,0,children[i]->curr_number_of_keys);

                        children[i]->data[0] = data[i-1];

                        mergeChildren(i-1);
                        shiftLeft<T *>(data,i-1,curr_number_of_keys);
                        //returning if violates minimum
                        return curr_number_of_keys >= min_number_of_keys ? 0 : 1;
                    }

                    if (hasRight)
                    {
                        // parent is data[i]
                        // insert into target right child
                        shiftRight<T *>(children[i+1]->data, 0, children[i+1]->curr_number_of_keys);

                        children[i+1]->data[0] = data[i];

                        mergeChildren(i);
                        shiftLeft<T *>(data, i, curr_number_of_keys);
                        // returning if violates minimum
                        return curr_number_of_keys >= min_number_of_keys ? 0 : 1;
                    }
                 }
                 return -1;
               }
             
          }
    };
    template<typename T>
    class BTree{
      public:
        int m; //degree of tree
        Node<T> *root; // root node of tree
        BTree(int m1){
            this->m = m1;
            root = new Node<T>(m, true);
            root->min_number_of_keys = 1;

        }
        void Traverse(void(itemViewer)(T *data)){
            return root->Traverse(itemViewer);
        }

        void insert(vector<T *> elements){
            for(auto &x : elements){
                insert(x);
            }
        }
        void insert(T *element){
            T *upshiftElement = nullptr;
            Node<T> *right = root->insertElement(element, upshiftElement);
            if(right){
                //need to create new root
                Node<T>* newRoot = new Node<T>(m, false);
                newRoot->data[0] = upshiftElement;
                newRoot->curr_number_of_keys = 1;
                //setting its vchildtrm
                newRoot->children[0] = root;
                newRoot->children[1] = right;

                root = newRoot;
            }
        }

        T *&operator[](int key){
            return root->search(key);
        }

        T*& search(int key){
            return root->search(key);
        }
        bool erase(int key){
            int result = root->deleteKey(key);
            if(result!=0){
            // not deleted
                return false;
            }
          //when root node shrinks acctually
           if(root->curr_number_of_keys==0){
              //no more keys, tree shrink
              root =root->children[0];
           }
           return true;
        }
        void clear(){
            root->clear();
        }
    };
} 

#endif
