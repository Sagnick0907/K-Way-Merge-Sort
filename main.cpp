#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

#define ll long long

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR _mkdir
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(a) mkdir(a, 0777)
#endif

void removeDirectory(const string& path) {
#ifdef _WIN32
    string removeCommand = "rmdir /s /q " + path;
#else
    string removeCommand = "rm -r " + path;
#endif

    system(removeCommand.c_str());
}

int numOfTempFiles=0;
struct VectorComparator {
    bool operator()(const vector<ll>& a, const vector<ll>& b) const {
        return a[0] < b[0]; // Compare the first element of the vectors
    }
};

class priority_queue {
private:
    vector<vector<ll>> heap; // Vector storing vectors as elements
    int currsize;                      // Current number of elements in the heap

    // Helper function to dynamically resize the heap's underlying vector
    void resize() {
        // Capacity doubling not required for vectors
    }

    void heapifyUp(int idx, const VectorComparator& comp) {
        int parent = (idx - 1) / 2;
        while (idx > 0 && comp(heap[idx], heap[parent])) {
            swap(heap[idx], heap[parent]);
            idx = parent;
            parent = (idx - 1) / 2;
        }
    }

    void heapifyDown(int idx, const VectorComparator& comp) {
        int leftChild = 2 * idx + 1;
        int rightChild = 2 * idx + 2;
        int curr = idx;

        if (leftChild < currsize && comp(heap[leftChild], heap[curr])) {
            curr = leftChild;
        }

        if (rightChild < currsize && comp(heap[rightChild], heap[curr])) {
            curr = rightChild;
        }

        if (curr != idx) {
            swap(heap[idx], heap[curr]);
            heapifyDown(curr, comp);
        }
    }

public:
    // Constructor to initialize an empty heap
    priority_queue() : currsize(0) {}

    // Insert an element into the heap
    void push(const vector<ll>& value, const VectorComparator& comp) {
        heap.push_back(value);
        currsize++; // Increase the size of the heap
        heapifyUp(currsize - 1, comp);
    }

    // Extract the top element from the heap
    vector<ll> top() {
        if (currsize == 0) {
            // Return an empty vector or handle error as required
            return vector<ll>();
        }
        return heap[0];
    }

    // Remove the top element from the heap
    void pop(const VectorComparator& comp) {
        if (currsize == 0) {
            // Handle error or do nothing
            return;
        }

        swap(heap[0], heap[currsize - 1]);
        heap.pop_back();
        currsize--; // Decrease the size of the heap
        heapifyDown(0, comp);
    }

    // Check if the heap is empty
    bool empty() {
        return currsize == 0;
    }

    int size() {
        return currsize;
    }
};

// // Function to process the input file (not implemented in this example)
// void processFile(const char* inputFile, const char* outputFile) {
//     // Existing processFile function remains unchanged
//     // ...
// }

void splitIntoFiles(const char* inputFile, ll maxNumbersPerFile, vector<string>& tempFilesList) {
    ifstream input(inputFile);
    if (!input.is_open()) {
        cerr << "Unable to open the input file." << endl;
        return;
    }

    const string directoryName = "tempcache";
    MKDIR(directoryName.c_str());

    ll num;
    int fileCount = 1;
    int currentNumbers = 0;

    ofstream output;
    string outputFileName = "";
    vector<ll> numbers;

    while (input >> num) {
        numbers.push_back(num);
        currentNumbers++;

        if (currentNumbers % maxNumbersPerFile == 0) {
            sort(numbers.begin(), numbers.end());

            outputFileName = "./" + directoryName + "/tempfile" + to_string(fileCount++) + ".txt";
            tempFilesList.push_back(outputFileName);
            output.open(outputFileName);
            if (!output.is_open()) {
                cerr << "Error creating temp file: " << outputFileName << endl;
                break;
            }

            for (ll sortedNum : numbers) {
                output << sortedNum << " ";
            }

            output.close();
            cout << "Number of integers in " << tempFilesList.back() << ": " << numbers.size() << endl;
            cout.flush(); // Flush the output buffer
            numbers.clear();
            currentNumbers = 0;
        }
    }

    if (!numbers.empty()) {
        sort(numbers.begin(), numbers.end());
        outputFileName = "./" + directoryName + "/tempfile" + to_string(fileCount++) + ".txt";

        tempFilesList.push_back(outputFileName);
        output.open(outputFileName);
        if (!output.is_open()) {
            cerr << "Error creating temp file: " << outputFileName << endl;
        } else {
            for (ll sortedNum : numbers) {
                output << sortedNum << " ";
            }
            output.close();
            cout << "Number of integers in " << tempFilesList.back() << ": " << numbers.size() << endl;
            cout.flush(); // Flush the output buffer
        }
    }

    input.close();
    numOfTempFiles = fileCount;
}




vector<ll> internalMerge(const vector<vector<ll>>& inputVectors) {
    priority_queue pq;
    VectorComparator comp;

    for (size_t i = 0; i < inputVectors.size(); ++i) {
        if (!inputVectors[i].empty()) {
            pq.push({ inputVectors[i][0], static_cast<int>(i), 0 }, comp); // Added index 0
        }
    }

    vector<ll> mergedResult;

    while (!pq.empty()) {
        auto topEle = pq.top(); // [value, fileNum, idx]
        pq.pop(comp);

        mergedResult.push_back(topEle[0]);

        int idx = topEle[2];
        ll fileNum = topEle[1];
        if (idx + 1 < inputVectors[fileNum].size()) {
            pq.push({ inputVectors[fileNum][idx + 1], fileNum, idx + 1 }, comp); // Push next element
        }
    }

    return mergedResult;
}


// Function to merge the temporary files using internalMerge function
void mergeTempFiles(vector<string>& tempFilesList, const string& outputFileName, int k) {
    const char* directoryName = "tempcache";
    MKDIR(directoryName);

    int filesRemaining = tempFilesList.size();
    int mergeCount = 0;

    while (filesRemaining > 0) {
        int mergeFiles = min(k, filesRemaining);
        vector<vector<ll>> inputVectors(mergeFiles);
        // cout << "Files Remaining: " << filesRemaining<<endl;

        for (int i = 0; i < mergeFiles; ++i) {
            FILE* file = fopen(tempFilesList[mergeCount * k + i].c_str(), "r");
            if (file != nullptr) {
                ll num;
                while (fscanf(file, "%lld", &num) != EOF) {
                    inputVectors[i].push_back(num); // Store values in k input vectors
                    // cout << num << " "; // You can display each num read here for debugging
                }
                // cout << inputVectors[i].size()<<endl;
                fclose(file);
                remove(tempFilesList[mergeCount * k + i].c_str()); // Remove file after reading its contents
            }
        }

        vector<ll> mergedResult = internalMerge(inputVectors); // Perform k-way merge
        filesRemaining = filesRemaining- mergeFiles+1;
        // for(int i=0;i<mergedResult.size();i++){
        //     cout<<mergedResult[i]<<" ";
        // }
        // cout<<endl;

        if (filesRemaining == 1) { // Write to output file if only one file remaining
            FILE* output = fopen(outputFileName.c_str(), "w");
            for (ll num : mergedResult) {
                fprintf(output, "%lld ", num); // Write sorted values to output file
            }
            fclose(output);
            break;
        } else {
            char outputFileName[100];
            snprintf(outputFileName, sizeof(outputFileName), "%s/tempfile%d.txt", directoryName, mergeCount);
            FILE* output = fopen(outputFileName, "w");
            for (ll num : mergedResult) {
                fprintf(output, "%lld ", num); // Write sorted values to temporary output file
            }
            tempFilesList.push_back(outputFileName);
            fclose(output);
        }

        mergeCount++;
        
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    auto start = chrono::steady_clock::now(); 
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    ll maxNumbersPerFile = 2000000; // Set the maximum numbers per temp file
    vector<string> tempFilesList;

    // Call function to split the input file into smaller temporary files
    splitIntoFiles(inputFile, maxNumbersPerFile, tempFilesList);
    if(numOfTempFiles-1 <=0){
        return 0;
    }
    printf("Number of temporary files created: %d \n", numOfTempFiles-1);

    if (tempFilesList.size() == 1) {
        vector<ll> numbers;

        ifstream singleTempFile(tempFilesList[0]);
        ll num;
        while (singleTempFile >> num) {
            numbers.push_back(num);
        }
        singleTempFile.close();


        ofstream output(outputFile);
        if (!output.is_open()) {
            cerr << "Error creating output file: " << outputFile << endl;
            return 1;
        }

        for (ll num : numbers) {
            output << num << " ";
        }

        output.close();

        string tempcachePath = "./tempcache";
        removeDirectory(tempcachePath);

        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        double elapsed_milliseconds = elapsed_seconds.count() * 1000;

        // Convert milliseconds to minutes and seconds
        int minutes = static_cast<int>(elapsed_milliseconds) / 60000;
        int seconds = static_cast<int>(elapsed_milliseconds) / 1000 - (minutes * 60);

        cout << "Time taken by the program: " << minutes << " minutes and " << seconds << " seconds" << endl;

        return 0;
    }
    
    // // Display the list of temp files
    // for (const auto& file : tempFilesList) {
    //     cout << file << endl;
    // }

    int k = 20; // Number of temp files opened and merged at a time
    // Call function to merge the temporary files into a single output file
    mergeTempFiles(tempFilesList, outputFile, k);
    
    // printf("Merging Files done!!\n");

    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    double elapsed_milliseconds = elapsed_seconds.count() * 1000;

    // Convert milliseconds to minutes and seconds
    int minutes = static_cast<int>(elapsed_milliseconds) / 60000;
    int seconds = static_cast<int>(elapsed_milliseconds) / 1000 - (minutes * 60);

    string tempcachePath = "./tempcache";
    removeDirectory(tempcachePath);

    cout << "Time taken by the program: " << minutes << " minutes and " << seconds << " seconds" << endl;
    return 0;
}
