#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include "pbPlots.h"
#include "supportLib.h"

//Insertion sort
void insertionSort(int arr[], int arrSize)
{
	int i, j, k;
	for (i = 1; i < arrSize; i++) {
		k = arr[i];		//Set k equal to the element at index i
		j = i - 1;		//Index j behind i

		//If element value at j is greater than element value at i
		//then put value at index j at index i and decrement j
		while (j >= 0 && arr[j] > k) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		//Set value at index j + 1 = i to k
		arr[j + 1] = k;
	}
}

//Merge two sub-arrays together
void merge(int arr[], int left, int mid, int right) {
	//Initialize number of elements of sub arrays left and right
	int sArrLeftSize = mid - left + 1;
	int sArrRightSize = right - mid;

	//Allocate memory for temporary array because C++ doesn't like array sizes unless their
	//defined by constants or macros >:(
	int *leftArray = new int[sArrLeftSize], *rightArray = new int[sArrRightSize];

	//Store values from array into left and right arrays
	for (int i = 0; i < sArrLeftSize; i++) {
		leftArray[i] = arr[left + i];
	}
	for (int j = 0; j < sArrRightSize; j++) {
		rightArray[j] = arr[mid + 1 + j];
	}

	//Declare indeices of two sub array and main array that is the sub arrays merged
	int indexSArrLeft = 0, indexSArrRight = 0, indexMergedArr = left;

	//index through left and right arrays and place them sorted into the merged array
	//by comparing the elements at each index
	while (indexSArrLeft < sArrLeftSize && indexSArrRight < sArrRightSize) {
		if (leftArray[indexSArrLeft] <= rightArray[indexSArrRight]) {
			arr[indexMergedArr] = leftArray[indexSArrLeft];
			indexSArrLeft++;
		}
		else {
			arr[indexMergedArr] = rightArray[indexSArrRight];
			indexSArrRight++;
		}
		indexMergedArr++;
	}
	//If there are any elements still in left array place them into merged array
	while (indexSArrLeft < sArrLeftSize) {
		arr[indexMergedArr] = leftArray[indexSArrLeft];
		indexSArrLeft++;
		indexMergedArr++;
	}
	//If there are any elements still in right array place them into merged array
	while (indexSArrRight < sArrRightSize) {
		arr[indexMergedArr] = rightArray[indexSArrRight];
		indexSArrRight++;
		indexMergedArr++;
	}
}

//Merge sort
void mergeSort(int arr[], int low, int high) {
	//If lowest index is greater than or equal to highest index return(Check Array has more than 1 element)
	if (low >= high) {
		return;
	}

	//Find middle array index
	int mid = (low + high) / 2;
	//Recursively merge sort by splitting array from middle index
	mergeSort(arr, low, mid);
	mergeSort(arr, mid + 1, high);
	//Merge sorted arrays together
	merge(arr, low, mid, high);
}

//Heapify a subtree as a Max heap
void heapify(int arr[], int arrSize, int node) {
	//Initialize largest as root node
	int largest = node;
	//initialize left child as index 2*node+1
	int left = 2 * node + 1;
	//initialize right child as index 2*node+2
	int right = 2 * node + 2;
	//If left child is larger than root node replace largest with left child
	if (left < arrSize && arr[left] > arr[largest]) {
		largest = left;
	}
	//If right child is larger than root node replace largest with right child
	if (right < arrSize && arr[right] > arr[largest]) {
		largest = right;
	}
	//If largest is not the root node swap node and largest elements and recursively heapify
	if (largest != node) {
		std::swap(arr[node], arr[largest]);
		heapify(arr, arrSize, largest);
	}
}

void heapSort(int arr[], int arrSize) {
	//Build the max heap using heapify function
	for (int i = arrSize / 2 - 1; i >= 0; i--) {
		heapify(arr, arrSize, i);
	}
	//Take elements from the max heap one at a time in sorted order
	for (int i = arrSize - 1; i > 0; i--) {
		//Swap elements at current root with last element in array
		std::swap(arr[0], arr[i]);
		//Max heapify the reduced array
		heapify(arr, i, 0);
	}
}
//Partition with last element in array as pivot
int partitionLast(int arr[], int low, int high) {
	int pivot = arr[high]; //Choose Last element as pivot

	int i = (low - 1);

	for (int j = low; j <= high; j++) {
		//If element at j is less than pivot value increment i by 1 then swap with element at i
		if (arr[j] < pivot) {
			i++;
			std::swap(arr[i], arr[j]);
		}
	}
	std::swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void quickSort(int arr[], int low, int high) {
	//Check that the lowest index is less than the highest(Array is larger than 1 element)
	if (low < high) {
		//Choose partition index using partition function with last element as pivot
		int partitionIndex = partitionLast(arr, low, high);

		//Recursively quicksort the partitioned arrays
		quickSort(arr, low, partitionIndex - 1);
		quickSort(arr, partitionIndex + 1, high);
	}
}

int partitionMedianof3(int arr[], int low, int high) {
	//Find middle element
	int mid = (high + low) / 2;

	//Sort elements at first, middle and last indices
	if (arr[mid] < arr[low]) {
		std::swap(arr[mid], arr[low]);
	}
	if (arr[high] < arr[low]) {
		std::swap(arr[high], arr[low]);
	}
	if (arr[high] < arr[mid]) {
		std::swap(arr[high], arr[mid]);
	}

	//Swap middle and second to last element
	std::swap(arr[mid], arr[high-1]);

	//Set pivot equal to second to last element
	//int pivot = arr[high-1];

	//Partition using second to last element as pivot
	//Ignore first and last elements since they are prepartitioned
	return partitionLast(arr, low + 1, high - 1);
}

//If the array to be quicksorted is less than or equal to 15 elements then use insertion sort
//Modified quicksort using median of three partition
void modifiedQuicksort(int arr[], int low, int high) {
	if (high <= 15) {
		insertionSort(arr, high + 1);
	}
	else {
		//Stadard quicksort except use partition pivot as median of first middle and last elements
		if (low < high) {
			int partitionIndex = partitionMedianof3(arr, low, high);

			modifiedQuicksort(arr, low, partitionIndex - 1);
			modifiedQuicksort(arr, partitionIndex + 1, high);
		}
	}
}

//Utility function to print the array and check it is sorted
void printArray(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		std::cout << arr[i] << " ";
	}
}

int timeAnalysisSort(int arr[], int arrSize, int sortType) {
	//Create temporary array so that unsorted array remain unsorted
	int *tempArr = new int[arrSize];
	//Populate temporary array with elements from unsorted array
	for (int i = 0; i <arrSize; i++) {
		tempArr[i] = arr[i];
	}

	auto start = std::chrono::high_resolution_clock::now();
	switch (sortType) {
	case 0 :
		insertionSort(tempArr, arrSize);
		std:: cout << "Time taken for insertion sort with array of size " << arrSize << ": ";
		break;
	case 1 :
		mergeSort(tempArr, 0, arrSize - 1);
		std::cout << "Time taken for merge sort with array of size " << arrSize << ": ";
		break;
	case 2 :
		heapSort(tempArr, arrSize);
		std::cout << "Time taken for heap sort with array of size " << arrSize << ": ";
		break;
	case 3 :
		quickSort(tempArr, 0, arrSize - 1);
		std::cout << "Time taken for quick sort with array of size " << arrSize << ": ";
		break;
	case 4 :
		modifiedQuicksort(tempArr, 0, arrSize - 1);
		std::cout << "Time taken for modified quick sort with array of size " << arrSize << ": ";
		break;
	default:
		std::cout << "Invalid sorting type. Time taken: ";
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << " microseconds." << std::endl;
	
	//delete temporary array
	delete[] tempArr;

	return duration.count();
}

void createPlot(std::vector<double> n[], std::vector<double> times[], char title) {
	RGBABitmapImageReference* imageRefunSorted = CreateRGBABitmapImageReference();
	RGBABitmapImageReference* imageRefSortedAsc = CreateRGBABitmapImageReference();
	RGBABitmapImageReference* imageRefSortedDes = CreateRGBABitmapImageReference();

	StringReference* errorMessage = new StringReference();

	ScatterPlotSeries* series0 = GetDefaultScatterPlotSeriesSettings();
	series0->xs = &n[0];
	series0->ys = &times[0];
	series0->linearInterpolation = false;
	series0->pointType = toVector(L"dots");
	series0->color = CreateRGBColor(1, 0, 0);
	ScatterPlotSeries* series1 = GetDefaultScatterPlotSeriesSettings();
	series1->xs = &n[1];
	series1->ys = &times[1];
	series1->linearInterpolation = false;
	series1->pointType = toVector(L"triangles");
	series1->color = CreateRGBColor(0, 1, 0);
	ScatterPlotSeries* series2 = GetDefaultScatterPlotSeriesSettings();
	series2->xs = &n[2];
	series2->ys = &times[2];
	series2->linearInterpolation = false;
	series2->pointType = toVector(L"circles");
	series2->color = CreateRGBColor(0, 0, 1);
	ScatterPlotSeries* series3 = GetDefaultScatterPlotSeriesSettings();
	series3->xs = &n[3];
	series3->ys = &times[3];
	series3->linearInterpolation = false;
	series3->pointType = toVector(L"crosses");
	series3->color = CreateRGBColor(0.5, 0, 0.5);
	ScatterPlotSeries* series4 = GetDefaultScatterPlotSeriesSettings();
	series4->xs = &n[4];
	series4->ys = &times[4];
	series4->linearInterpolation = false;
	series4->pointType = toVector(L"filled triangles");
	series4->color = CreateRGBColor(0, 0.5, 0.5);

	ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();
	settings->width = 1920;
	settings->height = 1080;
	settings->autoBoundaries = true;
	//settings->autoPadding = false;
	if (title == 'u') {
		settings->title = toVector(L"Time Analysis of Unsorted (Red Dot - Insertion, Green Triangle - Merge, Blue Circle - Heap, Purple Cross - Quick, Cyan Triangle - Modified Quick)");
	}
	else if (title == 's') {
		settings->title = toVector(L"Time Analysis of Sorted Arrays Ascending (Red Dot - Insertion, Green Triangle - Merge, Blue Circle - Heap, Purple Cross - Quick, Cyan Triangle - Modified Quick)");
	}
	else if (title == 'd') {
		settings->title = toVector(L"Time Analysis of Sorted Arrays Descending (Red Dot - Insertion, Green Triangle - Merge, Blue Circle - Heap, Purple Cross - Quick, Cyan Triangle - Modified Quick)");
	}
	else {
		settings->title = toVector(L"");
	}
	settings->xLabel = toVector(L"Array Size");
	settings->yLabel = toVector(L"Time [us]");
	settings->scatterPlotSeries->push_back(series0);
	settings->scatterPlotSeries->push_back(series1);
	settings->scatterPlotSeries->push_back(series2);
	settings->scatterPlotSeries->push_back(series3);
	settings->scatterPlotSeries->push_back(series4);
	
	bool success = true;

	if (title == 'u') {
		success = DrawScatterPlotFromSettings(imageRefunSorted, settings, errorMessage);
	}
	else if (title == 's') {
		success = DrawScatterPlotFromSettings(imageRefSortedAsc, settings, errorMessage);
	}
	else if (title == 'd') {
		success = DrawScatterPlotFromSettings(imageRefSortedDes, settings, errorMessage);
	}

	if (success) {
		if (title == 'u') {
			std::vector<double>* pngData = ConvertToPNG(imageRefunSorted->image);
			WriteToFile(pngData, "timeAnalysisUnsorted.png");
			DeleteImage(imageRefunSorted->image);
		}
		else if (title == 's') {
			std::vector<double>* pngData = ConvertToPNG(imageRefSortedAsc->image);
			WriteToFile(pngData, "timeAnalysisSortedAscending.png");
			DeleteImage(imageRefSortedAsc->image);
		}
		else if (title == 'd') {
			std::vector<double>* pngData = ConvertToPNG(imageRefSortedDes->image);
			WriteToFile(pngData, "timeAnalysisSortedDescending.png");
			DeleteImage(imageRefSortedDes->image);
		}
	}
	else {
		std::cerr << "Error: ";
		for (wchar_t c : *errorMessage->string) {
			std::cerr << c;
		}
		std::cerr << std::endl;
	}

	series0->xs->clear();
	series0->ys->clear();
	series1->xs->clear();
	series1->ys->clear();
	series2->xs->clear();
	series2->ys->clear();
	series3->xs->clear();
	series3->ys->clear();
	series4->xs->clear();
	series4->ys->clear();
	settings->scatterPlotSeries->clear();
}

int main() {

	int arrSizes[8] = {1000, 2000, 3000, 5000, 10000, 40000, 50000, 60000};

	std::vector<double> n[5];
	std::vector<double> times[5];

	//Time analysis of sorting unsorted arrays
	std::cout << "Time analysis of sorting unsorted arrays: " << std::endl;
	for (int i = 0; i < 8; i++) {
		//Set array size and declare array
		int const arrSize = arrSizes[i];
		int* unsortedArr = new int[arrSize];
		//Create a random number generator with a distribution of integers between 1 and 1000
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000);
		//populate array with random numbers between 0 and 1000
		for (int j = 0; j < arrSize; j++) {
			unsortedArr[j] = dist(rng);
		}


		for (int j = 0; j < 5; j++){
			n[j].push_back(arrSize);
			times[j].push_back(timeAnalysisSort(unsortedArr, arrSize, j));
		}

		delete[] unsortedArr;
	}
	
	createPlot(n, times, 'u');

	//SPECIAL CASES:
	//Time analysis of sorting sorted arrays in ascending order
	std::cout << "\nTime analysis of sorting sorted arrays in ascending order: " << std::endl;
	for (int i = 0; i < 8; i++) {
		//Set array size and declare array
		int const arrSize = arrSizes[i];
		int* sortedArr = new int[arrSize];
		//Create a random number generator with a distribution of integers between 1 and 1000
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000);
		//populate array with random numbers between 0 and 1000
		for (int j = 0; j < arrSize; j++) {
			sortedArr[j] = dist(rng);
		}

		//Sort array in ascending order before time analysis using standard library sort
		std::sort(sortedArr, sortedArr + arrSize);

		for (int j = 0; j < 5; j++) {
			n[j].push_back(arrSize);
			times[j].push_back(timeAnalysisSort(sortedArr, arrSize, j));
		}

		delete[] sortedArr;
	}

	createPlot(n, times, 's');

	//SPECIAL CASES:
	//Time analysis of sorting sorted arrays in descending order
	std::cout << "\nTime analysis of sorting sorted arrays in descending order: " << std::endl;
	for (int i = 0; i < 8; i++) {
		//Set array size and declare array
		int const arrSize = arrSizes[i];
		int* sortedArr = new int[arrSize];
		//Create a random number generator with a distribution of integers between 1 and 1000
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000);
		//populate array with random numbers between 0 and 1000
		for (int j = 0; j < arrSize; j++) {
			sortedArr[j] = dist(rng);
		}

		//Sort array in descending order before time analysis using standard library sort
		std::sort(sortedArr, sortedArr + arrSize, std::greater<int>());

		for (int j = 0; j < 5; j++) {
			n[j].push_back(arrSize);
			times[j].push_back(timeAnalysisSort(sortedArr, arrSize, j));
		}

		delete[] sortedArr;
	}

	createPlot(n, times, 'd');

	return 0;
}