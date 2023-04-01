//-----------------------------------------------------------------------------
// Dictionary.cpp
// Michael Takami mtakami PA7
// Dictionary ADT implementation
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<cassert>
#include "Dictionary.h"

// Class Constructors & Destructors ----------------------------------------

// Node()
// Node constructor definition
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
} 

// Dictionary()
// Creates new Dictionary in the empty state. 
Dictionary::Dictionary(){
	nil = nullptr;
	root = nullptr;
	current = nullptr;
	num_pairs = 0;
}

// Dictionary()
// Copy constructor.
Dictionary::Dictionary(const Dictionary& D) {
    nil = nullptr;
    root = nullptr;
    current = nullptr;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}


// ~Dictionary()
// Destructor - deallocates any used memory
Dictionary::~Dictionary() {
    postOrderDelete(root);
    root = nullptr;
}


// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nullptr) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(getValue(R->key)) + "\n";
        inOrderString(s, R->right);
    }
}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string consists of keys only, separated by "\n", with the order determined
// by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const{
	if(R != nullptr){
		s += R->key + "\n";
		preOrderString(s, R->left);
		preOrderString(s, R->right);
	}
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R == nullptr || R == N) {
        return;
    }
    setValue(R->key, R->val);
    preOrderCopy(R->left, N);
    preOrderCopy(R->right, N);
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R, sets R to nil.
void Dictionary::postOrderDelete(Node* R){
    if (R == nullptr) {
        return;
    }
    postOrderDelete(R->left);
    postOrderDelete(R->right);
    delete R;
    R = nullptr;
}


// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the addrout of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Dictionary::Node* R, keyType k) const {
    if (R == nullptr || R->key == k) {
        return R;
    }
    if (k < R->key) {
        return search(R->left, k);
    }
    return search(R->right, k);
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Dictionary::Node* R) {
    if (R == nullptr) {
        return nullptr;
    }
    while (R->left != nullptr) {
        R = R->left;
    }
    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Dictionary::Node* R) {
	if (R == nil) {
		return nil;
	}
	while (R->right != nil) {
		R = R->right;
	}
	return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Dictionary::Node* N){
	if (N == nullptr || N->right == nullptr) {
        return nullptr;
    }
    Dictionary::Node* next = N->right;
    while (next->left != nullptr) {
        next = next->left;
    }
    return next;
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Dictionary::Node* N) {
if (N == nil) {
        return nil;
    } 
    else if (N->left != nil) {
        return findMax(N->left);
    } 
    else {
        Node *par = N->parent;
        while (par != nil && N == par->left) {
            N = par;
            par = par->parent;
        }
        return par;
    }
}



// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
	return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const {
    Node* found = search(root, k);
    return (found != nullptr);
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const {
    Node* sea = search(root, k);
    if (sea == nullptr) {
        throw std::out_of_range("Dictionary does not contain key k!");
    }
    return sea->val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const {
	return current != nullptr;
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const {
    if (current == nullptr) {
        throw std::invalid_argument("currentKey(): current iterator is not defined!");
    }
    return current->key;
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if (!hasCurrent()) {
        throw std::invalid_argument("currentVal(): current iterator is not defined!");
    }
    return current->val;
}

// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear() {
    postOrderDelete(root);
    nil = nullptr;
    root = nullptr;
    current = nullptr;
    num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v) {
    Node* node = search(root, k);
    if (node != nullptr) {
        node->val = v;
    } else {
        
        Node* parent = nullptr;
        Node* current = root;
        Node* insert = new Node(k, v);

        while (current != nullptr) {
            parent = current;
            if (k < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        insert->parent = parent;

        if (parent == nullptr) {
            root = insert;
        } else if (k < parent->key) {
            parent->left = insert;
        } else {
            parent->right = insert;
        }

        num_pairs++;
    }
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) {
    if (!contains(k)) {
        throw std::invalid_argument("remove(): BST does not contain key trying to be deleted!");
    }
    Node* cur = root;
    Node* par = nullptr;
    while (cur->key != k) {
        par = cur;
        if (k < cur->key) {
            cur = cur->left;
        } 
        else {
            cur = cur->right;
        }
    }
    if (cur == current) {
        current = nullptr;
    }
    Node* child;
    if (cur->left == nil) {
        child = cur->right;
    } 
    else if (cur->right == nil) {
        child = cur->left;
    } 
    else {
        Node* suc = cur->right;
        Node* sucPar = cur;
        while (suc->left != nil) {
            sucPar = suc;
            suc = suc->left;
        }
        if (sucPar != cur) {
            sucPar->left = suc->right;
            suc->right = cur->right;
        }
        suc->left = cur->left;
        child = suc;
    }
    if (par == nullptr) {
        root = child;
    } 
    else if (cur == par->left) {
        par->left = child;
    } 
    else {
        par->right = child;
    }
    delete cur;
    num_pairs--;
}


// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin() {
	if(root != nil){
		current = findMin(root);
	}
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end() {
    if (root != nil) {
        current = findMax(root);
    }
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next() {
	if (current == nullptr) {
		return;
	}
	if (current != findMax(root)) {
		current = findNext(current);
	}
	else {
		current = nullptr;
	}
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev() {
    if (!hasCurrent()) {
        throw std::logic_error("Tree Error: calling on fucntion prev(), Current not set");
    }
    current = findPrev(current);
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const {
    if(size() != D.size()){
        return false;
    }
    std::string d1, d2;
    inOrderString (d1, root);
    D.inOrderString (d2, D.root);
    return d1 == d2;
}

// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<(std::ostream& stream, const Dictionary& D) {
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals().
bool operator==( const Dictionary& A, const Dictionary& B ) {
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=(const Dictionary& D) {
   if(this != &D){
      clear();
      preOrderCopy(D.root, D.nil);
   }
   return *this;
}
