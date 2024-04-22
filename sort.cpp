#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to swap two elements
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Sequential Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(int arr[], int n) {
    int first, last;
    for (int i = 0; i < n - 1; i++) {
        if (i % 2 == 0) {
            #pragma omp parallel for private(first, last)
            for (int j = 0; j < n / 2; j++) {
                first = 2 * j;
                last = 2 * j + 1;
                if (arr[first] > arr[last]) {
                    swap(arr[first], arr[last]);
                }
            }
        } else {
            #pragma omp parallel for private(first, last)
            for (int j = 1; j < n / 2; j++) {
                first = 2 * j - 1;
                last = 2 * j;
                if (arr[first] > arr[last]) {
                    swap(arr[first], arr[last]);
                }
            }
        }
    }
}

// Merge two sorted subarrays
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Sequential Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int *a,n;

    cout<<"\n enter total no of elements: ";
    cin>>n;
    a=new int[n];
    cout<<"\n enter elements: ";
    for(int i=0;i<n;i++)
    {
   	 cin>>a[i];
    }
    
    parallelBubbleSort(a,n);
    
    cout<<"\n sorted array is: ";
    for(int i=0;i<n;i++)
    {
   	 cout<<a[i]<<" ";
    }

    srand(time(0));
    const int size = 10000;
    int arr[size], arr_copy[size];

    // Initialize array with random values
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
        arr_copy[i] = arr[i];
    }
    
    cout<<"\n";
    // Measure time for sequential bubble sort
    double start = omp_get_wtime();
    bubbleSort(arr, size);
    double end = omp_get_wtime();
    cout<<"\n";
    cout << "Sequential Bubble Sort Time: " << end - start << " seconds" << endl;

    // Measure time for parallel bubble sort
    start = omp_get_wtime();
    parallelBubbleSort(arr_copy, size);
    end = omp_get_wtime();
    cout<<"\n";
    cout << "Parallel Bubble Sort Time: " << end - start << " seconds" << endl;

    // Reset array for merge sort
    for (int i = 0; i < size; i++) {
        arr[i] = arr_copy[i];
    }

    // Measure time for sequential merge sort
    start = omp_get_wtime();
    mergeSort(arr, 0, size - 1);
    end = omp_get_wtime();
    cout<<"\n";
    cout << "Sequential Merge Sort Time: " << end - start << " seconds" << endl;

    // Measure time for parallel merge sort
    start = omp_get_wtime();
    parallelMergeSort(arr_copy, 0, size - 1);
    end = omp_get_wtime();
    cout<<"\n";
    cout << "Parallel Merge Sort Time: " << end - start << " seconds" << endl;
    cout<<"\n";

    return 0;
}
