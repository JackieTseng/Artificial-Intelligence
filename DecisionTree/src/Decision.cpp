#include <cstdio>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <iostream>

#define ATTRNUM 16
#define LINESIZE 43
using namespace std;

double threshold;
struct decisionTreeNode {
    int attribute;
    char decision;
    decisionTreeNode* childY;
    decisionTreeNode* childN;
    decisionTreeNode* childU;
    decisionTreeNode() {
        attribute = 0;
        decision = ' ';
        childU = childN = childY = NULL;
    }
};

struct record {
    char party;
    char attr[ATTRNUM];
};

vector<record> trainset;
vector<record> checkset;
//use the 16 bits of a int to represent the attribution selection record 
typedef int attributeRecord;
attributeRecord initAttr;

void readDataset(const char* trainsetFile, const char* checksetFile) {
	fstream fin;
    //read the train set
    fin.open(trainsetFile, ios_base::in);
    if (!fin) {
        printf("read dataset failed!\n");
    	return;
    }
    char line[LINESIZE];
    int count = 0;
    //All attribute are not selected at the beginning
    initAttr = 0x00000000;
    while(fin.getline(line, LINESIZE)) {
        count++;
    	char temp;
    	record currentRecord;
    	currentRecord.party = line[0];
    	if (line[0] == 'r') {
    		temp = 11;
    	} else {
    		temp = 9;
    	}
    	int index = 1;
    	for (int i = 0; i < ATTRNUM; ++i) {
            //get the vote result and skip the comma
    		currentRecord.attr[i] = line[temp];
    		temp += 2;
    	}
        trainset.push_back(currentRecord);
    }
    fin.close();
    fin.clear();
    //read the check set
    fin.open(checksetFile, ios_base::in);
    if(!fin) {
        printf("read checkset failed!\n");
        return;
    }
    count = 0;
    while(fin.getline(line, LINESIZE)) {
        count++;
        char temp;
        record currentRecord;
        currentRecord.party = line[0];
        if (line[0] == 'r') {
            temp = 11;
        } else {
            temp = 9;
        }
        int index = 1;
        for (int i = 0; i < ATTRNUM; ++i) {
            currentRecord.attr[i] = line[temp];
            temp += 2;
        }
        checkset.push_back(currentRecord);
    }
    fin.close();

}


inline double functionI(double pv1, double pv2) {
    return -pv1 * log2(pv1) - pv2 * log2(pv2);
}

int chooseAttribute(vector<record> dataset, attributeRecord attrUsed, double I) {
    double maxGain = -10000.0;
    //record the position of the best attribute
    int pos = 0;
    vector<record>::iterator it;
    int total = dataset.size();
    for (int i = 0; i < ATTRNUM; ++i) {
        if (((1 << i) & attrUsed) == 0) {
            double rem = 0.0;
            int yes[2] = {0}, no[2] = {0}, unknown[2] = {0};
            //count the numbers by partys and attitude
            for (it = dataset.begin(); it != dataset.end(); ++it) {
                if (it -> attr[i] == 'y') {
                    if (it -> party == 'd')
                        yes[0]++;
                    else
                        yes[1]++;
                } else if (it -> attr[i] == 'n') {
                    if (it -> party == 'd')
                        no[0]++;
                    else
                        no[1]++;
                } else {
                    if (it -> party == 'd')
                        unknown[0]++;
                    else
                        unknown[1]++;
                }
            }
            //Calculate Remainder
            if (yes[0] != 0 && yes[1] != 0)
                rem += (double)(yes[0] + yes[1]) / (double)total * functionI((double)yes[0] / (double)(yes[0] + yes[1]), (double)yes[1] / (double)(yes[0] + yes[1]));
            if (no[0] != 0 && no[1] != 0)
                rem += (double)(no[0] + no[1]) / (double)total * functionI((double)no[0] / (double)(no[0] + no[1]), (double)no[1] / (double)(no[0] + no[1]));
            if (unknown[0] != 0 && unknown[1] != 0)
                rem += (double)(unknown[0] + unknown[1]) / (double)total * functionI((double)unknown[0] / (double)(unknown[0] + unknown[1]), (double)unknown[1] / (double)(unknown[0] + unknown[1]));
            if (I - rem > maxGain) {
                maxGain = I - rem;
                pos = i;
            }
        }
    }
    //return the attribute with max gain
    return pos;
}

inline bool noAttrLeft(attributeRecord attrUsed) {
    return (attrUsed == 0x0000FFFF) ? true : false;
}

decisionTreeNode* treeLearning(vector<record> dataset, attributeRecord attrUsed, char defaultVal, double parentI) {
    decisionTreeNode* root = new decisionTreeNode;
    if (dataset.empty()) {
        root -> decision = defaultVal;
        return root;
    }
    int demNum = 0;
    int repNum = 0;
    int total = dataset.size();
    vector<record>::iterator it;
    for (it = dataset.begin(); it != dataset.end(); ++it) {
        if (it -> party == 'd') {
            demNum++;
        } else {
            repNum++;
        }
    }
    char m = demNum >= repNum ? 'd' : 'r';
    if (noAttrLeft(attrUsed)) {
        root -> decision = m;
        return root;
    }
    double I;
    if (demNum == 0 || repNum == 0) {
        I = 0.0;
    } else {
        double demRate = (double)demNum / (double)total;
        double repRate = (double)repNum / (double)total;
        I = functionI(demRate, repRate);
    }
    //If threshold is 0.0, the process only stop until the gain becomes zero
    if (threshold == 0.0) {
        if ((I - threshold) < 1e-14) {
            root -> decision = m;
            return root;
        }
    } else {
        if (I <= threshold) {
            root -> decision = m;
            return root;
        }
    }
    int bestAttr = chooseAttribute(dataset, attrUsed, I);
    root -> attribute = bestAttr;
    //build the subtree
    //get the data set for each of the three subtree
    vector<record> yesSet, noSet, unknownSet;
    for (it = dataset.begin(); it != dataset.end(); ++it) {
        if (it -> attr[bestAttr] == 'y') {
            yesSet.push_back(*it);
        } else if (it -> attr[bestAttr] == 'n') {
            noSet.push_back(*it);
        } else {
            unknownSet.push_back(*it);
        }
    }
    attributeRecord yesAttrUsed, noAttrUsed, unknownAttrUsed;
    yesAttrUsed = noAttrUsed = unknownAttrUsed = (1 << bestAttr) | attrUsed;
    //Recursion
    root -> childY = treeLearning(yesSet, yesAttrUsed, m, I);
    root -> childN = treeLearning(noSet, noAttrUsed, m, I);
    root -> childU = treeLearning(unknownSet, unknownAttrUsed, m, I);
    return root;
}
//Get a decision from a builded tree, according to the data
char getDecision(decisionTreeNode* tree, record data) {
    if (tree -> decision != ' ')
        return tree -> decision;
    char vote = data.attr[tree -> attribute];
    if (vote == 'y')
        return getDecision(tree -> childY, data);
    else if (vote == 'n')
        return getDecision(tree -> childN, data);
    else
        return getDecision(tree -> childU, data);
}
//function for output checking
void printTree(decisionTreeNode* root) {
    if (root -> decision != ' ') {
        printf("decision %c\n", root -> decision);
        return;
    }
    printf("attribute %d\n", root -> attribute + 1);
    printf("Yes to\n");
    printTree(root -> childY);
    printf("No to\n");
    printTree(root -> childN);
    printf("Unknown to\n");
    printTree(root -> childU);
}

int main(int argc, char** argv) {
    if (argc != 3 && argc != 4)
        return -1;
	readDataset(argv[1], argv[2]);
    if (argc == 3) {
        threshold = 0.3;
    } else {
        threshold = atof(argv[3]);
    }
    //build tree
    decisionTreeNode* root  = treeLearning(trainset, initAttr, 'd', 1.0);
    vector<record>::iterator it;
    int total = 0, wrong = 0;
    //Predict and check according to the check set
    for (it = checkset.begin(); it != checkset.end(); ++it) {
        total++;
        char predict = getDecision(root, *it);
        if (predict != it -> party) {
            wrong++;
        }
    }
    //Calculate the correct rate
    printf("%f%%\n", (double)(total - wrong) * 100.0 / (double)total);
	return 0;
}
