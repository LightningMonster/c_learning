#include <stdio.h>
#include <stdlib.h>

// Function for binary search
int binarySearch(int arr[], int n, int target) {
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = (low + high) / 2;

        if (arr[mid] == target) {
            return mid;  // Element found at index mid
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;  // Element not found
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Not enough arguments provided.\n");
        return 1;
    }

    int n = argc - 2;  // Array size (excluding executable and target)
    int arr[100], target, i;

    // Convert command-line arguments to array elements
    for (i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    // Last argument is the target element
    target = atoi(argv[argc - 1]);

    // Perform binary search
    int result = binarySearch(arr, n, target);

    if (result != -1) {
        printf("Element %d found at index %d\n", target, result);
    } else {
        printf("Element %d not found\n", target);
    }

    return 0;
}