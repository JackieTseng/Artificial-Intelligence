#include "BPLayer.cpp"

vector<Set> trainData; // Train Data Set
vector<Set> testData;  // Test Data Set

void readTrainData(const char* trainSet);
void readTestData(const char* testSet);
void Record(BPLayer a, double rate, int _hidden, double _learningRate, double _habitrate);
bool Setting(BPLayer& a);

/*
 * argv[1] -> mode
 * argv[2] -> hidden layer number
 * argv[3] -> learning rate
 * argv[4] -> habit rate
 */
int main(int argc, const char *argv[]) {
    int hidden = 167;
    int mode = 0;
    double a = 0.1, u = 0.4;
    if (argc == 2){
        mode = atoi(argv[1]);
    } else if (argc == 3){
        mode = atoi(argv[1]);
        hidden = atoi(argv[2]);
    } else if (argc == 4){
        mode = atoi(argv[1]);
        hidden = atoi(argv[2]);
        a = atof(argv[3]);
    } else if (argc == 5) {
        mode = atoi(argv[1]);
        hidden = atoi(argv[2]);
        a = atof(argv[3]);
        u = atof(argv[4]);
    }
    
    if (mode != 0 && mode != 1) {
        cout << "mode choose fail\n";
        return 0;
    }

    if (mode == 1 && argc > 2) {
        cout << "Set mode run shouldn't follow other parameters\n";
        return 0;
    }
    BPLayer e(64, hidden, 10, a, u);
    e.Initialize();

    // mode choose 
    if (!mode) {
        readTrainData("data/digitstra.txt");
        //readTrainData("../data/digitstra.txt");
        // Training
        for (int i = 0; i < trainData.size(); i++) {
            e.ForwardPropagate(trainData[i]);
            e.BackPropagate();
        }
    } else {
        // Seting
        if (!Setting(e)) {
            cout << "Setting File no exits\n";
            return 0;
        }
    }
    
    readTestData("data/digitstest.txt");
    //readTestData("../data/digitstest.txt");
    int all = testData.size();
    int count = 0;
    
    // Recognizing
    for (int i = 0; i < testData.size(); i++) {
        Result b = e.Recognize(testData[i]);
        if (testData[i].num == b.number) {
            count++;
        }
    }

    double _rate = (double)count / all * 100;
    Record(e, _rate, hidden, a, u);
    cout << _rate << "," << hidden << "," << a << "," << u << endl;
    return 0;
}

/*
 * Read date from input train Set
 */
void readTrainData(const char* trainSet) {
    fstream fin;
    fin.open(trainSet, ios_base::in);
    if(!fin) {
        cout << "Read TrainData Fail!" << endl;
        return;
    }
    char line[LINESIZE];
    char temp[3];
    int data[SETSIZE + 1];
    int numCount, charCount;
    
    // read and store the train data set
    while(fin.getline(line, LINESIZE)) {
        numCount = 0;
        for (int i = 0; i < strlen(line); i++) {
            charCount = 0;
            while (i != LINESIZE && line[i] != ',') {
                temp[charCount++] = line[i++];
            }
            temp[charCount] = '\0';
            data[numCount++] = atoi(temp);
        }
        Set currentSet;
        for (int i = 0; i < SETSIZE; i++) {
            currentSet.data[i] = data[i];
        }
        currentSet.num = data[SETSIZE];
        trainData.push_back(currentSet);
    }
    fin.close();
    return;
}

/*
 * Read data from input test Set
 */
void readTestData(const char* testSet) {
    fstream fin;
    fin.open(testSet, ios_base::in);
    if(!fin) {
        cout << "Read TestData Fail!" << endl;
        return;
    }
    char line[LINESIZE];
    char temp[3];
    int data[SETSIZE + 1];
    int numCount, charCount;
    
    // read and store the test data set
    while(fin.getline(line, LINESIZE)) {
        numCount = 0;
        for (int i = 0; i < strlen(line); i++) {
            charCount = 0;
            while (i != LINESIZE && line[i] != ',') {
                temp[charCount++] = line[i++];
            }
            temp[charCount] = '\0';
            data[numCount++] = atoi(temp);
        }
        Set currentSet;
        for (int i = 0; i < SETSIZE; i++) {
            currentSet.data[i] = data[i];
        }
        currentSet.num = data[SETSIZE];
        testData.push_back(currentSet);
    }
    fin.close();
    return;
}

/*
 * Record the current BPNetwork' data
 * @param BPLayer a
 *  the current trained network
 * @param rate
 *  the current recognization rate
 * @param _hidden
 *  the current hidden layer nodes number
 * @param _learningRate
 *  the current learning rate
 * @param _habitRate
 *  the current habit rate
 */
void Record(BPLayer a, double rate, int _hidden, double _learningRate, double _habitRate) {
    // Get the data from network
    Input* input = a.GetInput();
    Hidden* hidden = a.GetHidden();
    Output* output = a.GetOutput();

    ifstream result;
    result.open("data/result.txt");
    //result.open("../data/result.txt");
    char line[50];
    result.getline(line, 50);

    // if result.txt no exits
    if (strlen(line) == 0) {
        result.close();
        ofstream result;
        result.open("data/result.txt");
        //result.open("../data/result.txt");
        
        // Record the data
        result << "-----Rate-----\n";
        result << rate << endl;
        result << "-----hiddenNum-----\n";
        result << _hidden << endl;
        result << "-----learningRate " << _learningRate
               << " habitRate " << _habitRate << "-----\n";
        result << "-----Weight-----\n";
        for (int i = 0; i < a.GetInputLayerNum(); i++) {
            result << "-----Input Node " << i << " -----\n";
            for (int j = 0; j < a.GetHiddenLayerNum(); j++) {
                result << input[i].weights[j] << endl;
            }
        }
        for (int i = 0; i < a.GetHiddenLayerNum(); i++) {
            result << "-----Hidden Node " << i << " -----\n";
            for (int j = 0; j < a.GetOutputLayerNum(); j++) {
                result << hidden[i].weights[j] << endl;
            }
        }
        result.close();
        return;
    } else {
        result.getline(line, 50);
        double temp = atof(line);
        // if result.txt exits then compare
        // the recognization rate, current
        // rate larger tnen record
        if (temp > rate) {
            result.close();
            return;
        } else {
            result.close();
            ofstream result;

            // clear the content
            result.open("data/result.txt", ios::trunc);
            //result.open("../data/result.txt", ios::trunc);
            result << "-----Rate-----\n";
            result << rate << endl;
            result << "-----hidden-----\n";
            result << _hidden << endl;
            result << "-----learningRate " << _learningRate
                   << " habitRate " << _habitRate << "-----\n";
            result << "-----Weight-----\n";
            for (int i = 0; i < a.GetInputLayerNum(); i++) {
                result << "-----Input Node " << i << " -----\n";
                for (int j = 0; j < a.GetHiddenLayerNum(); j++) {
                    result << input[i].weights[j] << endl;
                }
            }
            for (int i = 0; i < a.GetHiddenLayerNum(); i++) {
                result << "-----Hidden Node " << i << " -----\n";
                for (int j = 0; j < a.GetOutputLayerNum(); j++) {
                    result << hidden[i].weights[j] << endl;
                }
            }
            result.close();
            return;
        }
    }
}

/*
 * Read the setting data and change network
 */
bool Setting(BPLayer& a) {
    ifstream fin;
    fin.open("data/result.txt");
    //fin.open("../data/result.txt");
    if(!fin) {
        cout << "Read Setting Data Fail!" << endl;
        return false;
    }
    Input* input;
    Hidden* hidden;
    int hiddenNum, rate;
    int flag = 0; // flag the states
    int i = 0, j = 0; // the indexes
    char line[100];
    double temp;
    // Read setting
    while (fin.getline(line, 100)) {
        // if no-digit or blank
        if (line[1] == '-' || strlen(line) == 0) {
            continue;
        } else {
            temp = atof(line);
            // read recognization rate
            if (flag == 0) {
                rate = temp;
                flag++;
            } else if (flag == 1) {
                // read hidden layer node number
                hiddenNum = temp;
                // construct the Layer
                input = new Input[64];
                for (int i = 0; i < 64; i++) {
                    input[i].weights = new double[hiddenNum];
                }
                hidden = new Hidden[hiddenNum];
                for (int i = 0; i < hiddenNum; i++) {
                    hidden[i].weights = new double[10];
                }
                flag++;
            } else if (flag == 2) {
                // read weights between input layer and hidden layer
                input[i].weights[j] = temp;
                if (++j == hiddenNum) {
                    j = 0;
                    if (++i == 64) {
                        flag++;
                        i = j = 0;
                    }
                }
            } else if (flag == 3) {
                // read weigths between hidden layer and output layer
                hidden[i].weights[j] = temp;
                if (++j == 10) {
                    j = 0;
                    if (++i == hiddenNum) {
                        flag++;
                    }
                }
            } else {
                continue;
            }
        }
    }
    // Set the network
    a.Setting(hiddenNum, input, hidden);
    return true;
}
