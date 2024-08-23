#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;


//Structure for Course data
struct Course {
	string courseId;
	string title;
	vector<string> preReqs;

};

//Node structure for BST
struct Node{
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a bid
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    void removeAll(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);

};
//removeall function removes all nodes from bst
 void BinarySearchTree::removeAll(Node* node) {
    if (node != nullptr) {//base case
        removeAll(node->left);//recurse down left children
        removeAll(node->right);//recurse down right children
        delete node;//deletes nodes
    }
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //set root as nullptr by default
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    //destructor calls remove all function to free all memory allocated by BST
    removeAll(root);

}

void BinarySearchTree::InOrder() {
    //calls function to print all items in alphanumeric order
    inOrder(root);
}





//creates a new node and records course data in it
void BinarySearchTree::Insert(Course course) {

    if (root == nullptr) {
        root = new Node(course);
    }
    else
    {
        addNode(root, course);
    }
}

//calls function to remove
void BinarySearchTree::Remove(string bidId) {
    removeNode(root, bidId);
}

//searches and returns course info based on a courseId
Course BinarySearchTree::Search(string courseId) {

    Course course;
    Node* currentNode = root;
    
    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {//course is found
            course = currentNode->course;
            return course;
        }
        else
        {
            if (currentNode->course.courseId > courseId) {//move down left child if courseId is less
                currentNode = currentNode->left;
            }
            else if (currentNode->course.courseId < courseId) {//Move down right child if courseId is greater
                currentNode = currentNode->right;
            }
        }
    }

    cout << "Searching: " << courseId << endl;
    return course;//return course, this could be empty
}

//adds node with given course information to the BST
void BinarySearchTree::addNode(Node* node, Course course) {

//recurse down BST until an empty space is found starting with the root
    if (node->course.courseId > course.courseId) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else
        {
            addNode(node->right, course);
        }
    }

}
//prints all items in BST in Alpha Numeric order using recursion
void BinarySearchTree::inOrder(Node* node) {

    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseId << "|" <<
            node->course.title << "|";
        for (int i = 0; i < node->course.preReqs.size(); i++) {
            cout << node->course.preReqs[i] << "|";
        }
        cout << endl;
        inOrder(node->right);
    }


}

//removes a specific node based on given node and courseId using recursion
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    if (node == nullptr) {
        return node;
    }

    if (courseId < node->course.courseId) {
        node->left = removeNode(node->left, courseId);//recurse down left child
    }
    else if (courseId > node->course.courseId) {
        node->right = removeNode(node->right, courseId);//recurse down right child
    }
    else {//if node is found
        if (node->left == nullptr) {//checks for leaf node
            Node* temp = node->right;
            delete node;
            cout << "Node deleted" << endl;
            return temp;
        }
        else if (node->right == nullptr) {//checks for leaf node
            Node* temp = node->left;
            delete node;
            cout << "Node deleted" << endl;
            return temp;

        }

        Node* temp = node->right;
        while (temp->left != nullptr) {//not leaf node, rearranges nodes to remove node
            temp = temp->left;
        }


        node->course = temp->course;


        node->right = removeNode(node->right, temp->course.courseId);
    }
    return node;
}









//loads course data from a CSV file into a bst data structure
void loadCourse(BinarySearchTree* bst) {
	fstream fin("CS_300_ABCU_Advising_Program_Input.csv");

	vector<string> row;
	string line;
	string word;
    cout << "Following Bids Inserted:" << endl;
	while (getline(fin, line)) {//goes through each line of the doc
		
		row.clear();//reset row vector
		
		stringstream ss(line);

		while (getline(ss, word, ',')) {//parses whole line and separates into a number of strings by comma
			row.push_back(word);
			
		}

        Course* newCourse = new Course();
        //assign course info from line to course object
        newCourse->courseId = row[0];
        newCourse->title = row[1];
        for (int i = 2; i < row.size(); i++) {
            if (row[i] != "") {
                newCourse->preReqs.push_back(row[i]);
            }
        }

        //inserts course object into BST
        bst->Insert(*newCourse);

        
		for (int i = 0; i < row.size(); i++) {
			if (row[i] != "") {//do not print out blank spaces
				cout << row[i] << "|";
                
			}
		}
		cout << endl;

	}
}


int main(int argc, char* argv[]) {
    BinarySearchTree* bst;
    bst = new BinarySearchTree();//initialize bst
    int choice = 0;
    while (choice != 9) {//menu structure
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            loadCourse(bst);
        }
        else if (choice == 2) {
            bst->InOrder();
        }
        else if (choice == 3) {
            string input;
            cout << "Please enter the course you would like to display:";
            cin >> input;
            Course currCourse = bst->Search(input);
            if (!currCourse.courseId.empty()) {//checks if a course with the given id exists
                cout << "ID: " << currCourse.courseId << endl;
                cout << "Title: " << currCourse.title << endl;
                cout << "Prerequisites: ";
                if (currCourse.preReqs.size() > 0) {
                    for (int i = 0; i < currCourse.preReqs.size(); i++) {
                        cout << currCourse.preReqs[i];
                        if (i + 1 < currCourse.preReqs.size()) {
                            cout << ", ";
                        }
                    }
                }
                else {
                    cout << "None";
                }
                cout << endl;
            }
            else {
                cout << "Course not found" << endl;
            }

        }
        else if(choice == 9) {
            break;
        }
        else{
            cout << choice << " is not a valid option" << endl;
        }
        
    }

    cout << "Good bye." << endl;

    return 0;

}