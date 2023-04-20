//============================================================================
// Name        : CS-300 Project 2
// Author      : Ron Bal
// Date		   : 4/11/2023
//============================================================================

#include <iostream>
#include <time.h>
#include <vector>
#include "CSVparser.hpp"
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

double strToDouble(string str, char ch);

//crate course structure
struct Course {
private:
	string courseId;
	string courseName;
	vector<string> coursePrereqs;

public:
	Course() = default;
	Course(string& id, string& name, vector<string>& prereqs);
	string getCourseId();
	string getCourseName();
	vector<string> getCoursePrereqs();
	string courseToString();

};

//create node structure
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course newCourse) :
		Node() {
		course = newCourse;
	}
};

//create courses structure
struct Courses {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void postOrder(Node* node);
	void preOrder(Node* node);
	Node* removeNode(Node* node, string courseId);

public:
	Courses();
	virtual ~Courses();
	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Course course);
	void Remove(string courseId);
	Course Search(string courseId);
};


Course::Course(string& id, string& name, vector<string>& prereqs) {

	this->courseId = id;
	this->courseName = name;
	this->coursePrereqs = prereqs;
}

string Course::getCourseId() {
	return this->courseId;
}

string Course::getCourseName() {

	return this->courseName;
}

vector<string> Course::getCoursePrereqs() {

	return this->coursePrereqs;
}

string Course::courseToString() {

	return this->courseId + ", " + this->courseName;
}

Courses::Courses() {
	root = nullptr;
}

Courses::~Courses() {
	
}

void Courses::InOrder() {

	this->inOrder(root);
	
	
}

void Courses::PostOrder() {

	this->postOrder(root);
}

void Courses::PreOrder() {

	this->preOrder(root);
}

void Courses::Insert(Course course) {

	if (root == nullptr) {
	
		root = new Node(course);
	}
	else {
	
		this->addNode(root, course);
	}
}
void Courses::Remove(string courseId) {

	this->removeNode(root, courseId);
}


Course Courses::Search(string courseId) {

	Node* curNode = root;
	while (curNode != nullptr) {
		if (curNode->course.getCourseId().compare(courseId) == 0) {
		
			return curNode->course;
		}

		else if (courseId.compare(curNode->course.getCourseId()) < 0) {
		
			curNode = curNode->left;
		}
		else {
		
			curNode = curNode->right;
		}
	}
	Course course;
	return course;
}

void Courses::addNode(Node* node, Course course) {

	if (node != nullptr && node->course.getCourseId().compare(course.getCourseId()) > 0) {
	
		if (node->left == nullptr) {
		
			node->left = new Node(course);
			return;
		}
		else {
		
			this->addNode(node->left, course);
		}
	}
	else if (node != nullptr && node->course.getCourseId().compare(course.getCourseId()) < 0) {

		if (node->right == nullptr) {

			node->right = new Node(course);
			return;
		}
				
		else {

			this->addNode(node->right, course);

		}

	}
}

void Courses::inOrder(Node* node) {

	if (node != nullptr) {
	
		inOrder(node->left);

		cout << node->course.getCourseId() << ", "
			<< node->course.getCourseName() << endl;

		inOrder(node->right);
		
	}
}

void Courses::preOrder(Node* node) {
	if (node != nullptr) {
	
		cout << node->course.getCourseId() << ", "
			<< node->course.getCourseName() << endl;

		preOrder(node->left);
		preOrder(node->right);
	}

}

void Courses::postOrder(Node* node) {

	if (node != nullptr) {
	
		postOrder(node->left);

		postOrder(node->right);
		cout << node->course.getCourseId() << ", "
			<< node->course.getCourseName() << endl;
	}

}
Node* Courses::removeNode(Node* node, const string courseId) {
	if (node == nullptr) {
		return node;
	}
	// if course is smaller than current node then traverse left
	else if (courseId.compare(node->course.getCourseId()) < 0) {
		node->left = removeNode(node->left, courseId);
	}
	// else if course is larger than current node then traverse right
	else if (courseId.compare(node->course.getCourseId()) > 0) {
		node->right = removeNode(node->right, courseId);
	}
	// else match point found
	else {
		// leaf node
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// else if left 
		else if (node->left != nullptr && node->right == nullptr) {
			Node* tmp = node;
			node = node->left;
			delete tmp;
			tmp = nullptr;
		}
		// else if right 
		else if (node->left == nullptr && node->right != nullptr) {
			Node* tmp = node;
			node = node->right;
			delete tmp;
			tmp = nullptr;
		}
		// else both 
		else {
			// find the min of the right subtree nodes for bst
			Node* tmp = node;
			while (tmp->left != nullptr) {
				tmp = tmp->left;
			}
			node->course = tmp->course;
			node->right = removeNode(node->right, tmp->course.getCourseId());
		}
	}
	return node;
}



void displayCourse(Course course) {
	vector<string> coursePrereqs = course.getCoursePrereqs();
	string prereqs;
	// formats output for prereqs if applicable
	if (coursePrereqs.size() == 1) {
		prereqs = course.getCoursePrereqs()[0];
	}
	else if (coursePrereqs.size() > 1) {
		for (int i = 0; i < coursePrereqs.size() - 1; i++) {
			prereqs += coursePrereqs[i] + ", ";
		}
		prereqs += coursePrereqs.back();
	}
	else {
		prereqs = "n/a";
	}

	// prints course details
	cout << course.getCourseId() << ", "
		<< course.getCourseName() << endl;
	cout << "Prerequisites: " << prereqs << endl;
}

/**
 * Load an input file containing courses into a container
 *
 * @param inputFilePath the path to the input file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, Courses* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	ifstream file(csvPath);
	string currentLine;
	try {
		while (getline(file, currentLine)) {
			stringstream ss(currentLine);
			string word, id, name;
			vector<string> prereqs;
			int index = 0;
			while (!ss.eof()) {
				getline(ss, word, ',');
				word = regex_replace(word, regex(R"(\r\n|\r|\n)"), "");
				if (index == 0) {
					id = word;
				}
				else if (index == 1) {
					name = word;
				}
				else {
					prereqs.push_back(word);
				}
				index++;
			}
			// create a data structure and add to the collection of courses
			Course course = Course(id, name, prereqs);
			// add course to the tree
			bst->Insert(course);
		}
	}
	catch (ifstream::failure& e) {
		cerr << e.what() << endl;
	}

	file.close();
}
double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

int main() {
	// Define a binary search tree to hold all courses
	Courses* bst;
	bst = new Courses();
	Course course;
	string csvPath, courseKey;

	cout << "Welcome to the ABCU course planner." << endl << endl;

	int choice = 0;
	while (choice != 9) {
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl << endl;
		cout << "What would you like to do? ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "Enter the path the to input file:  ";
			cin >> csvPath;
			// Complete the method call to load the courses
			loadCourses(csvPath, bst);
			break;
		case 2:
			cout << "Here is a sample schedule:" << endl << endl;
			bst->InOrder();
			break;
		case 3:
			cout << "What course do you want to know about? ";
			cin >> courseKey;
			course = bst->Search(courseKey);

			if (!course.getCourseId().empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course Id " << courseKey << " not found." << endl;
			}
			break;
		case 9:
			cout << "Good bye." << endl;
			break;
		default:
			cout << choice << " is not a valid option." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
		cout << endl;
	}

	return 0;
}

