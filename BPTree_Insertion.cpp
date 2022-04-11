#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define f(i, a, b) for(ll i = a; i < b; i++)

int maxDataKeys;
int maxIndexKeys;

int currentNumOfDataNodes = 0;
int currentNumOfIndexNodes = 0;


class Node{
    int* key;
    Node** ptr;
    int size;
    int maxCapacity;
    bool isLeaf;
    friend class BPTree;

public:
    Node(int nodeSize){
        key = new int[nodeSize];
        ptr = new Node*[nodeSize + 1];
    }

};


class BPTree{
    Node* root;
    void insertInternal(int, Node*, Node*);
    Node* findParent(Node*, Node*);
    
public:
    BPTree(){
        root = NULL;
    }

    void insert(int);

    Node* getRoot(){
        return root;
    }

    void showStatus(){
        cout<<currentNumOfIndexNodes<<" "<<currentNumOfDataNodes<<" ";
        
        f(i, 0, root->size){
            cout<<root->key[i]<<" ";
        }cout<<endl;
    }
};

void BPTree::insert(int x){
        if(root == NULL){
            root = new Node(maxDataKeys);
            root->key[0] = x;
            root->isLeaf = true;
            root->size = 1;

            currentNumOfDataNodes++;
        }
        else{
            Node* currentNodePtr = root;
            Node* parent;

            //Reach to the correct Leaf Node
            while(currentNodePtr->isLeaf == false){
                int i;
                bool flag = false;

                f(j, 0, currentNodePtr->size){
                    if(x < currentNodePtr->key[j]){
                        i = j;
                        flag = true;
                        break;
                    }
                }

                if(!flag){
                    i = currentNodePtr->size;
                }

                parent = currentNodePtr;
                currentNodePtr = currentNodePtr->ptr[i];
            }
            // Found the correct Leaf Node


            // Now check if the LeafNode is full or not
            if(currentNodePtr->size < maxDataKeys){
                // In this case, insert the key directly to its correct place;
                int i = currentNodePtr->size;

                //Find the correct place(it is denoted by "i")

                f(j, 0, currentNodePtr->size){
                    if(x < currentNodePtr->key[j]){
                        i = j;
                        break;
                    }
                }

                        //-------------------ABOVE FOR LOOP may not work...If error occurs, comeback HERE.
                
                //Shift each element to the right by one place, until iterator reaches i;
                for(int j = currentNodePtr->size; j > i; j--){
                    currentNodePtr->key[j] = currentNodePtr->key[j - 1];
                }

                //insert new element "x" at the i-th place.
                currentNodePtr->key[i] = x;
                currentNodePtr->size++;
            }else{
                // This is the case when the leaf node is full.

                //Firstly, create a new node(to accomodate the splitting)
                Node* newLeafNode = new Node(maxDataKeys);

                currentNumOfDataNodes++;

                //Store all the keys from currentNodePtr and the new key-"x", to temp array, in CORRECT ORDER.
                int temp[maxDataKeys + 1];

                f(j, 0, maxDataKeys){
                    temp[j] = currentNodePtr->key[j];
                }

                    //Find the correct place for key-"x".

                int i = maxDataKeys;

                //Find the correct place(it is denoted by "i")

                f(j, 0, maxDataKeys){
                    if(x < temp[j]){
                        i = j;
                        break;
                    }
                }

                        //-------------------ABOVE FOR LOOP may not work...If error occurs, comeback HERE.
                
                //Shift each element to the right by one place, until iterator reaches i;
                for(int j = maxDataKeys; j > i; j--){
                    temp[j] = temp[j - 1];
                }

                //insert new element "x" at the i-th place.
                temp[i] = x;

                currentNodePtr->size = maxDataKeys / 2;
                newLeafNode->size = (maxDataKeys / 2) + 1;
                newLeafNode->isLeaf = true;

                //insert first m/2 keys from temp in currentNodePtr & remaining m/2 + 1 keys in newLeafNode.
                f(j, 0, currentNodePtr->size){
                    currentNodePtr->key[j] = temp[j];
                }

                f(j, currentNodePtr->size, maxDataKeys + 1){
                    newLeafNode->key[j - currentNodePtr->size] = temp[j];
                }


                //Now we have to assign these nodes(i.e. currentNodePtr & newLeafnode) to its parent
                if(currentNodePtr == root){
                    Node* newRoot = new Node(maxIndexKeys);
                    currentNumOfIndexNodes++;

                    newRoot->isLeaf = false;
                    newRoot->key[0] = newLeafNode->key[0];
                    newRoot->ptr[0] = currentNodePtr;
                    newRoot->ptr[1] = newLeafNode;
                    newRoot->size = 1;
                    
                    root = newRoot;
                }else{
                    insertInternal(newLeafNode->key[0], parent, newLeafNode);
                }




            }
            


        }
    }

void BPTree::insertInternal(int x, Node* currentNodePtr, Node* child){
    if(currentNodePtr->size < maxIndexKeys){
        int i = currentNodePtr->size;

        f(j, 0, currentNodePtr->size){
            if(x < currentNodePtr->key[j]){
                i = j;
                break;
            }
        }

        for(int j = currentNodePtr->size; j > i; j--){
            currentNodePtr->key[j] = currentNodePtr->key[j - 1];
        }
        currentNodePtr->key[i] = x;
        currentNodePtr->size++;
        
        for(int j = currentNodePtr->size + 1; j > i + 1; j--){
            currentNodePtr->ptr[j] = currentNodePtr->ptr[j - 1];
        }

        currentNodePtr->ptr[i + 1] = child;
    }else{
        //This part will get run only if the Prent(index) node is full.

        Node* newInternalNode = new Node(maxIndexKeys);
        currentNumOfIndexNodes++;

        int temp[maxIndexKeys + 1];
        Node* tempPtrs[maxIndexKeys + 2];

        f(j, 0, maxIndexKeys){
            temp[j] = currentNodePtr->key[j];
        }

        f(j, 0, maxIndexKeys + 1){
            tempPtrs[j] = currentNodePtr->ptr[j];
        }

        int i = maxIndexKeys;

        f(j, 0, maxIndexKeys){
            if(x < currentNodePtr->key[j]){
                i = j;
                break;
            }
        }

        for(int j = maxIndexKeys; j > i; j--){
            temp[j] = temp[j - 1];
        }

        temp[i] = x;

        for(int j = maxIndexKeys + 1; j > i + 1; j--){
            tempPtrs[j] = tempPtrs[j - 1];
        }

        tempPtrs[i + 1] = child;

        newInternalNode->isLeaf = false;

        currentNodePtr->size = maxIndexKeys / 2;
        newInternalNode->size = (maxIndexKeys / 2) + 1;


        //From here, there is a possibility of mistake.
        f(j, currentNodePtr->size + 1, maxIndexKeys + 1){
            newInternalNode->key[j - currentNodePtr->size] = temp[j];
        }

        f(j, currentNodePtr->size + 1, maxIndexKeys + 2){
            newInternalNode->ptr[j - currentNodePtr->size] = tempPtrs[j];
        }



        if(currentNodePtr == root){
            Node* newRoot = new Node(maxIndexKeys);
            currentNumOfIndexNodes++;

            newRoot->key[0] = temp[currentNodePtr->size];
            newRoot->ptr[0] = currentNodePtr;
            newRoot->ptr[1] = newInternalNode;
            newRoot->size = 1;
            newRoot->isLeaf = false;

            root = newRoot;
        }else{
            insertInternal(temp[currentNodePtr->size], findParent(root, currentNodePtr), newInternalNode);
        }



    }
}

Node* BPTree::findParent(Node* currentNodePtr, Node* child){
    Node* parent;

	// If cursor reaches the end of Tree
	if (currentNodePtr->isLeaf
		|| (currentNodePtr->ptr[0])->isLeaf) {
		return NULL;
	}

	// Traverse the current node with
	// all its child
	for (int i = 0;
		i < currentNodePtr->size + 1; i++) {

		// Update the parent for the
		// child Node
		if (currentNodePtr->ptr[i] == child) {
			parent = currentNodePtr;
			return parent;
		}

		// Else recursively traverse to
		// find child node
		else {
			parent
				= findParent(currentNodePtr->ptr[i],
							child);

			// If parent is found, then
			// return that parent node
			if (parent != NULL)
				return parent;
		}
	}

	// Return parent node
	return parent;
}




int main(){
    int d, t;
    cin>>d>>t;

    maxDataKeys = 2 * d;
    maxIndexKeys = 2 * t + 1;

    BPTree node;
    
    Node* root = node.getRoot();

    while(1){
        int x;
        cin>>x;

        if(x == 1){
            int numToInsert;
            cin>>numToInsert;

            node.insert(numToInsert);
        }else if(x == 2){
            node.showStatus();
        }else{
            cout<<"Quitting the program..."<<endl;
            break;
        }
    }

}